/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/whitebit/protocol/json/contract_type.hpp"
#include "roq/whitebit/protocol/json/event_type.hpp"
#include "roq/whitebit/protocol/json/options_type.hpp"
#include "roq/whitebit/protocol/json/order_status.hpp"
#include "roq/whitebit/protocol/json/order_type.hpp"
#include "roq/whitebit/protocol/json/side.hpp"
#include "roq/whitebit/protocol/json/status.hpp"
#include "roq/whitebit/protocol/json/time_in_force.hpp"

#include "roq/option_type.hpp"
#include "roq/order_status.hpp"
#include "roq/order_type.hpp"
#include "roq/security_type.hpp"
#include "roq/side.hpp"
#include "roq/time_in_force.hpp"
#include "roq/trading_status.hpp"
#include "roq/update_type.hpp"

#include "roq/map.hpp"

namespace roq {

template <>
template <>
std::optional<UpdateType> Map<whitebit::protocol::json::EventType>::helper() const;

template <>
template <>
std::optional<OptionType> Map<whitebit::protocol::json::OptionsType>::helper() const;

template <>
template <>
std::optional<OrderStatus> Map<whitebit::protocol::json::OrderStatus>::helper() const;

template <>
template <>
std::optional<OrderType> Map<whitebit::protocol::json::OrderType>::helper() const;

template <>
template <>
std::optional<Side> Map<whitebit::protocol::json::Side>::helper() const;

template <>
template <>
std::optional<TradingStatus> Map<whitebit::protocol::json::Status>::helper() const;

template <>
template <>
std::optional<TimeInForce> Map<whitebit::protocol::json::TimeInForce>::helper() const;

// ===

template <>
template <>
std::optional<SecurityType> Map<whitebit::protocol::json::ContractType, whitebit::protocol::json::OptionsType>::helper() const;

// ===

template <>
template <>
std::optional<whitebit::protocol::json::OrderType> Map<OrderType>::helper() const;

template <>
template <>
std::optional<whitebit::protocol::json::Side> Map<Side>::helper() const;

template <>
template <>
std::optional<whitebit::protocol::json::TimeInForce> Map<TimeInForce>::helper() const;

}  // namespace roq
