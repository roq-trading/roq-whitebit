/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "parser_tester.hpp"

using namespace roq;
using namespace roq::whitebit;

using namespace std::literals;
using namespace std::chrono_literals;

using value_type = protocol::json::BookTickerUpdate;

TEST_CASE("simple", "[json_book_ticker_update]") {
  auto message = R"({)"
                 R"("method": "bookTicker_update", )"
                 R"("params": [[)"
                 R"(1785732678.2802551, )"
                 R"(1785732678.2810941, )"
                 R"("BTC_PERP", )"
                 R"(22217102542, )"
                 R"("62965.8", )"
                 R"("0.005", )"
                 R"("62974.2", )"
                 R"("0.017")"
                 R"(])"
                 R"(], )"
                 R"("id": null)"
                 R"(})"sv;
  auto helper = [](value_type const &obj) {
    // CHECK(obj.topic == "tickers.BTCUSDT"sv);
    // CHECK(obj.timestamp == 1682083233096ms);
  };
  ParserTester<value_type>::dispatch(helper, message, 8192, 1);
}
