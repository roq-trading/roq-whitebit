/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <string_view>

#include "roq/trace_info.hpp"

#include "roq/core/json/buffer_stack.hpp"

#include "roq/whitebit/protocol/json/amend_order2.hpp"
#include "roq/whitebit/protocol/json/auth2.hpp"
#include "roq/whitebit/protocol/json/cancel_order2.hpp"
#include "roq/whitebit/protocol/json/error.hpp"
#include "roq/whitebit/protocol/json/ping.hpp"
#include "roq/whitebit/protocol/json/place_order2.hpp"

namespace roq {
namespace whitebit {
namespace protocol {
namespace json {

struct Parser2 final {
  struct Handler {
    virtual void operator()(Trace<protocol::json::Ping> const &) = 0;
    virtual void operator()(Trace<protocol::json::Auth2> const &) = 0;
    virtual void operator()(Trace<protocol::json::Error> const &) = 0;
    virtual void operator()(Trace<protocol::json::PlaceOrder2> const &) = 0;
    virtual void operator()(Trace<protocol::json::AmendOrder2> const &) = 0;
    virtual void operator()(Trace<protocol::json::CancelOrder2> const &) = 0;
  };

  static bool dispatch(Handler &, std::string_view const &message, core::json::BufferStack &, TraceInfo const &, bool allow_unknown_event_types);
};

}  // namespace json
}  // namespace protocol
}  // namespace whitebit
}  // namespace roq
