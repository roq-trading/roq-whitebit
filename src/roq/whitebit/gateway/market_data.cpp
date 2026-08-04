/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/whitebit/gateway/market_data.hpp"

#include "roq/logging.hpp"

#include "roq/mask.hpp"

#include "roq/utils/safe_cast.hpp"
#include "roq/utils/update.hpp"

#include "roq/utils/exceptions/unhandled.hpp"

#include "roq/utils/metrics/factory.hpp"

#include "roq/whitebit/protocol/json/map.hpp"

using namespace std::literals;

namespace roq {
namespace whitebit {
namespace gateway {

// === CONSTANTS ===

namespace {
auto const NAME = "md"sv;

auto const SUPPORTS = Mask{
    SupportType::MARKET_STATUS,
    SupportType::TOP_OF_BOOK,
    SupportType::MARKET_BY_PRICE,
    SupportType::TRADE_SUMMARY,
    SupportType::STATISTICS,
};

uint64_t const REQUEST_ID = 1'000'000;

size_t const DEPTH_25 = 25;
size_t const DEPTH_50 = 50;

size_t const MAX_DECODE_BUFFER_DEPTH = 1;
}  // namespace

// === HELPERS ===

namespace {
auto create_name(auto stream_id) {
  return fmt::format("{}:{}"sv, stream_id, NAME);
}

auto create_connection(auto &handler, auto &settings, auto &context) {
  auto uri = settings.ws.uri;
  auto config = web::socket::Client::Config{
      // connection
      .interface = {},
      .uris = {&uri, 1},
      .host = settings.ws.host,
      .validate_certificate = settings.net.tls_validate_certificate,
      // connection manager
      .connection_timeout = settings.net.connection_timeout,
      .disconnect_on_idle_timeout = settings.net.disconnect_on_idle_timeout,
      .always_reconnect = true,
      // proxy
      .proxy = {},
      // http
      .user_agent = ROQ_PACKAGE_NAME,
      .request_timeout = {},
      .ping_frequency = settings.ws.ping_freq,
      // implementation
      .decode_buffer_size = settings.misc.decode_buffer_size,
      .encode_buffer_size = settings.misc.encode_buffer_size,
  };
  return web::socket::Client::create(handler, context, config, []() { return std::string(); });
}

auto is_spot(auto api) {
  return api == tools::API::SPOT;
}

auto get_mbp_depth(auto &settings, auto api) -> size_t {
  auto result = settings.ws.mbp_depth;
  if (!result) {
    switch (api) {
      using enum tools::API;
      case UNDEFINED:
        break;
      case SPOT:
        return DEPTH_50;
      case LINEAR:
        return DEPTH_50;
      case INVERSE:
        return DEPTH_50;
      case OPTION:
        return DEPTH_25;
    }
    log::fatal("Unexpected"sv);
  }
  return result;
}

auto create_mbp_topic(size_t depth) {
  return fmt::format("orderbook.{}"sv, depth);
}

struct create_metrics final : public utils::metrics::Factory {
  create_metrics(auto &settings, auto &group, auto const &function) : utils::metrics::Factory{settings.app.name, group, function} {}
};
}  // namespace

// === IMPLEMENTATION ===

MarketData::MarketData(Handler &handler, io::Context &context, uint16_t stream_id, Shared &shared, size_t index)
    : handler_{handler}, stream_id_{stream_id}, name_{create_name(stream_id_)}, index_{index}, ping_frequency_{shared.settings.ws.ping_freq},
      spot_{is_spot(shared.api.api)}, mbp_depth_{get_mbp_depth(shared.settings, shared.api.api)}, mbp_topic_{create_mbp_topic(mbp_depth_)},
      connection_{create_connection(*this, shared.settings, context)}, decode_buffer_{shared.settings.misc.decode_buffer_size, MAX_DECODE_BUFFER_DEPTH},
      request_id_{stream_id_ * REQUEST_ID},
      counter_{
          .disconnect = create_metrics(shared.settings, name_, "disconnect"sv),
      },
      profile_{
          .parse = create_metrics(shared.settings, name_, "parse"sv),
          .response = create_metrics(shared.settings, name_, "response"sv),
          .book_ticker_update = create_metrics(shared.settings, name_, "book_ticker_update"sv),
          .depth_update = create_metrics(shared.settings, name_, "depth_update"sv),
          .trades_update = create_metrics(shared.settings, name_, "trades_update"sv),
          .market_update = create_metrics(shared.settings, name_, "market_update"sv),
          .market_today_update = create_metrics(shared.settings, name_, "market_today_update"sv),
      },
      latency_{
          .ping = create_metrics(shared.settings, name_, "ping"sv),
          .heartbeat = create_metrics(shared.settings, name_, "heartbeat"sv),
      },
      shared_{shared} {
}

void MarketData::operator()(Event<Start> const &) {
  (*connection_).start();
}

void MarketData::operator()(Event<Stop> const &) {
  (*connection_).stop();
}

void MarketData::operator()(Event<Timer> const &event) {
  auto now = event.value.now;
  (*connection_).refresh(now);
  if (ready() && next_ping_ < now) {
    send_ping(now);
  }
}

void MarketData::operator()(metrics::Writer &writer) const {
  writer
      // counter
      .write(counter_.disconnect, metrics::Type::COUNTER)
      // profile
      .write(profile_.parse, metrics::Type::PROFILE)
      .write(profile_.response, metrics::Type::PROFILE)
      .write(profile_.book_ticker_update, metrics::Type::PROFILE)
      .write(profile_.depth_update, metrics::Type::PROFILE)
      .write(profile_.trades_update, metrics::Type::PROFILE)
      .write(profile_.market_update, metrics::Type::PROFILE)
      .write(profile_.market_today_update, metrics::Type::PROFILE)
      // latency
      .write(latency_.ping, metrics::Type::LATENCY)
      .write(latency_.heartbeat, metrics::Type::LATENCY);
}

void MarketData::subscribe(size_t start_from) {
  if (ready()) {
    subscribe(shared_.symbols.get_slice(index_, start_from));
  }
}

void MarketData::operator()(web::socket::Client::Connected const &) {
}

void MarketData::operator()(web::socket::Client::Disconnected const &) {
  ++counter_.disconnect;
  (*this)(ConnectionStatus::DISCONNECTED);
}

void MarketData::operator()(web::socket::Client::Ready const &) {
  (*this)(ConnectionStatus::READY);
  subscribe();
}

void MarketData::operator()(web::socket::Client::Close const &) {
}

void MarketData::operator()(web::socket::Client::Latency const &latency) {
  TraceInfo trace_info;
  auto external_latency = ExternalLatency{
      .stream_id = stream_id_,
      .account = {},
      .latency = latency.sample,
  };
  create_trace_and_dispatch(shared_.dispatcher, trace_info, external_latency);
  latency_.ping.update(latency.sample);
}

void MarketData::operator()(web::socket::Client::Text const &text) {
  parse(text.payload);
}

void MarketData::operator()(web::socket::Client::Binary const &) {
  log::fatal("Unexpected"sv);
}

void MarketData::operator()(ConnectionStatus connection_status, std::string_view const &reason) {
  connection_status_ = connection_status;
  TraceInfo trace_info;
  auto stream_status = StreamStatus{
      .stream_id = stream_id_,
      .account = {},
      .supports = SUPPORTS,
      .transport = Transport::TCP,
      .protocol = Protocol::WS,
      .encoding = {Encoding::JSON},
      .priority = Priority::PRIMARY,
      .connection_status = connection_status_,
      .reason = reason,
      .interface = (*connection_).get_interface(),
      .authority = (*connection_).get_current_authority(),
      .path = (*connection_).get_current_path(),
      .proxy = (*connection_).get_proxy(),
  };
  log::info("stream_status={}"sv, stream_status);
  create_trace_and_dispatch(shared_.dispatcher, trace_info, stream_status);
}

void MarketData::subscribe(std::span<Symbol const> const &symbols) {
  if (std::empty(symbols)) {
    return;
  }
  subscribe("bookTicker_subscribe"sv, symbols);
  subscribe("depth_subscribe"sv, symbols, 100);  // XXX FIXME TODO limit=100 from settings
  subscribe("trades_subscribe"sv, symbols);
  subscribe("market_subscribe"sv, symbols);
  subscribe("marketToday_subscribe"sv, symbols);
}

void MarketData::subscribe(std::string_view const &method, std::span<Symbol const> const &symbols) {
  assert(!std::empty(symbols));
  auto message = fmt::format(
      R"({{)"
      R"("id":{},)"
      R"("method":"{}",)"
      R"("params":["{}"])"
      R"(}})"sv,
      ++request_id_,
      method,
      fmt::join(symbols, R"(",")"sv));
  log::debug("{}"sv, message);
  (*connection_).send_text(message);
}

void MarketData::subscribe(std::string_view const &method, std::span<Symbol const> const &symbols, size_t limit) {
  assert(!std::empty(symbols));
  for (auto &symbol : symbols) {
    auto message = fmt::format(
        R"({{)"
        R"("id":{},)"
        R"("method":"{}",)"
        R"("params":[)"
        R"("{}",)"
        R"({},)"
        R"("0",)"  // price_interval
        R"(true)"  // subscription is **added**
        R"(])"
        R"(}})"sv,
        ++request_id_,
        method,
        symbol,
        limit);
    log::debug("{}"sv, message);
    (*connection_).send_text(message);
  }
}

void MarketData::send_ping(std::chrono::nanoseconds now) {
  assert(ping_frequency_.count() > 0);
  next_ping_ = now + ping_frequency_;
  auto message = fmt::format(
      R"({{)"
      R"("id":0,)"
      R"("method":"ping",)"
      R"("params":[])"
      R"(}})"sv);
  log::debug("{}"sv, message);
  (*connection_).send_text(message);
}

void MarketData::parse(std::string_view const &message) {
  profile_.parse([&]() {
    auto log_message = [&]() { log::warn(R"(*** PLEASE REPORT *** message="{}")"sv, message); };
    try {
      TraceInfo trace_info;
      if (!protocol::json::Parser::dispatch(*this, message, decode_buffer_, trace_info, shared_.settings.experimental.allow_unknown_event_types)) {
        log_message();
      }
    } catch (...) {
      log_message();
      utils::exceptions::Unhandled::terminate();
    }
  });
}

void MarketData::operator()(Trace<protocol::json::Pong> const &event) {
  auto &[trace_info, pong] = event;
  log::debug("pong={}"sv, pong);
  log::info<4>("pong={}"sv, pong);
  (*connection_).touch(trace_info.source_receive_time);
}

void MarketData::operator()(Trace<protocol::json::Response> const &event) {
  profile_.response([&]() {
    auto &[trace_info, response] = event;
    log::debug("response={}"sv, response);
    log::info<3>("response={}"sv, response);
    (*connection_).touch(trace_info.source_receive_time);
    if (response.error.code) {
      log::error("response={}"sv, response);
    }
  });
}

void MarketData::operator()(Trace<protocol::json::BookTickerUpdate> const &event) {
  profile_.book_ticker_update([&]() {
    auto &[trace_info, book_ticker_update] = event;
    log::debug("book_ticker_update={}"sv, book_ticker_update);
    log::info<3>("book_ticker_update={}"sv, book_ticker_update);
    (*connection_).touch(trace_info.source_receive_time);
    /*
    auto &data = book_ticker_update.data;
    switch (shared_.api.api) {
      using enum tools::API;
      case UNDEFINED:
        break;
      case SPOT:
        // note! using orderbook.1
        break;
      case LINEAR:
      case INVERSE: {
        auto top_of_book = TopOfBook{
            .stream_id = stream_id_,
            .exchange = shared_.settings.exchange,
            .symbol = data.symbol,
            .layer{
                .bid_price = data.bid1_price,
                .bid_quantity = data.bid1_size,
                .ask_price = data.ask1_price,
                .ask_quantity = data.ask1_size,
            },
            .update_type = UpdateType::INCREMENTAL,
            .exchange_time_utc = {},
            .exchange_sequence = tickers.cross_sequence,
            .sending_time_utc = tickers.timestamp,
        };
        create_trace_and_dispatch(shared_.dispatcher, trace_info, top_of_book, true);
        break;
      }
      case OPTION: {
        auto top_of_book = TopOfBook{
            .stream_id = stream_id_,
            .exchange = shared_.settings.exchange,
            .symbol = data.symbol,
            .layer{
                .bid_price = data.bid_price,
                .bid_quantity = data.bid_size,
                .ask_price = data.ask_price,
                .ask_quantity = data.ask_size,
            },
            .update_type = UpdateType::INCREMENTAL,
            .exchange_time_utc = {},
            .exchange_sequence = tickers.cross_sequence,
            .sending_time_utc = tickers.timestamp,
        };
        create_trace_and_dispatch(shared_.dispatcher, trace_info, top_of_book, true);
        break;
      }
    }
    std::array<Statistics, 4> statistics{{
        {
            .type = StatisticsType::HIGHEST_TRADED_PRICE,
            .value = data.high_price24h,
            .begin_time_utc = {},
            .end_time_utc = {},
        },
        {
            .type = StatisticsType::LOWEST_TRADED_PRICE,
            .value = data.low_price24h,
            .begin_time_utc = {},
            .end_time_utc = {},
        },
        {
            .type = StatisticsType::CLOSE_PRICE,
            .value = data.last_price,
            .begin_time_utc = {},
            .end_time_utc = {},
        },
        {
            .type = StatisticsType::TRADE_VOLUME,
            .value = data.volume24h,
            .begin_time_utc = {},
            .end_time_utc = {},
        },
    }};
    auto statistics_update = StatisticsUpdate{
        .stream_id = stream_id_,
        .exchange = shared_.settings.exchange,
        .symbol = data.symbol,
        .statistics = statistics,
        .update_type = UpdateType::INCREMENTAL,
        .exchange_time_utc = {},
        .exchange_sequence = tickers.cross_sequence,
        .sending_time_utc = tickers.timestamp,
    };
    create_trace_and_dispatch(shared_.dispatcher, trace_info, statistics_update, true);
    */
  });
}

void MarketData::operator()(Trace<protocol::json::DepthUpdate> const &event) {
  profile_.depth_update([&]() {
    auto &[trace_info, depth_update] = event;
    log::debug("depth_update={}"sv, depth_update);
    log::info<3>("depth_update={}"sv, depth_update);
    (*connection_).touch(trace_info.source_receive_time);
    /*
    auto &data = depth_update.data;
    if (depth == 1) {
      auto helper = [](auto &levels) -> std::pair<double, double> {
        double price = NaN;
        double quantity = NaN;
        // first non-zero quantity
        for (auto &item : levels) {
          if (utils::compare(item.quantity, 0.0) > 0) {
            price = item.price;
            quantity = item.quantity;
            break;
          }
        }
        return {price, quantity};
      };
      auto [bid_price, bid_quantity] = helper(data.bids);
      auto [ask_price, ask_quantity] = helper(data.asks);
      auto top_of_book = TopOfBook{
          .stream_id = stream_id_,
          .exchange = shared_.settings.exchange,
          .symbol = data.symbol,
          .layer{
              .bid_price = bid_price,
              .bid_quantity = bid_quantity,
              .ask_price = ask_price,
              .ask_quantity = ask_quantity,
          },
          .update_type = map(depth_update.type),
          .exchange_time_utc = depth_update.timestamp,
          .exchange_sequence = data.cross_sequence,
          .sending_time_utc = {},
      };
      create_trace_and_dispatch(shared_.dispatcher, trace_info, top_of_book, true);
    } else {
      shared_.bids.clear();
      shared_.asks.clear();
      auto emplace_back = [](auto &result, auto &item) {
        auto mbp_update = MBPUpdate{
            .price = item.price,
            .quantity = item.quantity,
            .implied_quantity = NaN,
            .number_of_orders = {},
            .update_action = {},
            .price_level = {},
        };
        result.emplace_back(std::move(mbp_update));
      };
      for (auto &item : data.bids) {
        emplace_back(shared_.bids, item);
      }
      for (auto &item : data.asks) {
        emplace_back(shared_.asks, item);
      }
      auto market_by_price_update = MarketByPriceUpdate{
          .stream_id = stream_id_,
          .exchange = shared_.settings.exchange,
          .symbol = data.symbol,
          .bids = shared_.bids,
          .asks = shared_.asks,
          .update_type = map(depth_update.type),
          .exchange_time_utc = depth_update.timestamp,
          .exchange_sequence = data.cross_sequence,
          .sending_time_utc = {},
          .price_precision = {},
          .quantity_precision = {},
          .checksum = {},
      };
      try {
        create_trace_and_dispatch(shared_.dispatcher, trace_info, market_by_price_update, true, shared_.final_bids, shared_.final_asks);
      } catch (BadState &) {
        // resubscribe(symbol);
      }
    }
    */
  });
}

void MarketData::operator()(Trace<protocol::json::TradesUpdate> const &event) {
  profile_.trades_update([&]() {
    auto &[trace_info, trades_update] = event;
    log::debug("trades_update={}"sv, trades_update);
    log::info<3>("trades_update={}"sv, trades_update);
    (*connection_).touch(trace_info.source_receive_time);
    /*
    auto &trades = shared_.trades;
    trades.clear();
    std::chrono::milliseconds timestamp = {};
    auto dispatch = [&](auto &symbol) {
      if (std::empty(symbol)) {
        assert(std::empty(trades));
      }
      if (std::empty(trades)) {
        return;
      }
      auto trade_summary = TradeSummary{
          .stream_id = stream_id_,
          .exchange = shared_.settings.exchange,
          .symbol = symbol,
          .trades = trades,
          .exchange_time_utc = timestamp,
          .exchange_sequence = {},
          .sending_time_utc = trades_update.timestamp,
      };
      create_trace_and_dispatch(shared_.dispatcher, trace_info, trade_summary, true);
      trades.clear();
      timestamp = {};
    };
    std::string_view previous;
    for (auto &item : trades_update.data) {
      if (item.symbol != previous) {
        dispatch(previous);
        previous = item.symbol;
      }
      auto trade_2 = Trade{
          .side = map(item.side),
          .price = item.price,
          .quantity = item.quantity,
          .trade_id = item.trade_id,
          .taker_order_id = {},
          .maker_order_id = {},
      };
      trades.emplace_back(std::move(trade_2));
      utils::update_max(timestamp, item.timestamp);
    }
    dispatch(previous);
    */
  });
}

void MarketData::operator()(Trace<protocol::json::MarketUpdate> const &event) {
  profile_.market_update([&]() {
    auto &[trace_info, market_update] = event;
    log::debug("market_update={}"sv, market_update);
    log::info<3>("market_update={}"sv, market_update);
    (*connection_).touch(trace_info.source_receive_time);
  });
}

void MarketData::operator()(Trace<protocol::json::MarketTodayUpdate> const &event) {
  profile_.market_today_update([&]() {
    auto &[trace_info, market_today_update] = event;
    log::debug("market_today_update={}"sv, market_today_update);
    log::info<3>("market_today_update={}"sv, market_today_update);
    (*connection_).touch(trace_info.source_receive_time);
  });
}

}  // namespace gateway
}  // namespace whitebit
}  // namespace roq
