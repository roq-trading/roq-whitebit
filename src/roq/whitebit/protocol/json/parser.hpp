/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <string_view>

#include "roq/trace_info.hpp"

#include "roq/core/json/buffer_stack.hpp"

#include "roq/whitebit/protocol/json/error.hpp"
#include "roq/whitebit/protocol/json/pong.hpp"
#include "roq/whitebit/protocol/json/subscribe.hpp"

// public
#include "roq/whitebit/protocol/json/book_ticker_update.hpp"
#include "roq/whitebit/protocol/json/depth_update.hpp"
#include "roq/whitebit/protocol/json/trades_update.hpp"

// private
#include "roq/whitebit/protocol/json/auth.hpp"
#include "roq/whitebit/protocol/json/execution.hpp"
#include "roq/whitebit/protocol/json/order.hpp"
#include "roq/whitebit/protocol/json/position.hpp"
#include "roq/whitebit/protocol/json/wallet.hpp"

namespace roq {
namespace whitebit {
namespace protocol {
namespace json {

struct Parser final {
  struct Handler {
    virtual void operator()(Trace<protocol::json::Pong> const &) = 0;
    // response
    virtual void operator()(Trace<protocol::json::Auth> const &) = 0;
    virtual void operator()(Trace<protocol::json::Subscribe> const &) = 0;
    virtual void operator()(Trace<protocol::json::Error> const &) = 0;
    // public stream
    virtual void operator()(Trace<protocol::json::BookTickerUpdate> const &) = 0;
    virtual void operator()(Trace<protocol::json::DepthUpdate> const &) = 0;
    virtual void operator()(Trace<protocol::json::TradesUpdate> const &) = 0;
    // private stream
    virtual void operator()(Trace<protocol::json::Wallet> const &) = 0;
    virtual void operator()(Trace<protocol::json::Position> const &) = 0;
    virtual void operator()(Trace<protocol::json::Order> const &) = 0;
    virtual void operator()(Trace<protocol::json::Execution> const &) = 0;
  };

  static bool dispatch(Handler &, std::string_view const &message, core::json::BufferStack &, TraceInfo const &, bool allow_unknown_event_types);
};

}  // namespace json
}  // namespace protocol
}  // namespace whitebit
}  // namespace roq
