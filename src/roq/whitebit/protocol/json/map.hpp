/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/whitebit/protocol/json/trade_type.hpp"
#include "roq/whitebit/protocol/json/type.hpp"

#include "roq/security_type.hpp"
#include "roq/side.hpp"

#include "roq/map.hpp"

namespace roq {

template <>
template <>
std::optional<SecurityType> Map<whitebit::protocol::json::Type>::helper() const;

template <>
template <>
std::optional<Side> Map<whitebit::protocol::json::TradeType>::helper() const;

// ===

template <>
template <>
std::optional<whitebit::protocol::json::TradeType> Map<Side>::helper() const;

}  // namespace roq
