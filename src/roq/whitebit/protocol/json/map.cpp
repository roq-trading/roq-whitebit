/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/whitebit/protocol/json/map.hpp"

using namespace std::literals;

namespace roq {

namespace {
template <typename... Args>
using Helper = detail::MapHelper<Args...>;
}

// whitebit::json => roq

// whitebit::protocol::json::Type ==> roq::SecurityType

template <>
template <>
constexpr Helper<whitebit::protocol::json::Type>::operator std::optional<roq::SecurityType>() const {
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::Type::type_t;
    case UNDEFINED_INTERNAL:
      return SecurityType::UNDEFINED;
    case UNKNOWN_INTERNAL:
      return SecurityType::UNDEFINED;
    case SPOT:
      return SecurityType::SPOT;
    case FUTURES:
      return SecurityType::FUTURES;
    case TRADFI_FUTURES:
      return SecurityType::FUTURES;
  }
  return {};
}

static_assert(Helper{whitebit::protocol::json::Type{whitebit::protocol::json::Type::UNDEFINED_INTERNAL}} == roq::SecurityType::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::Type{whitebit::protocol::json::Type::SPOT}} == roq::SecurityType::SPOT);
static_assert(Helper{whitebit::protocol::json::Type{whitebit::protocol::json::Type::FUTURES}} == roq::SecurityType::FUTURES);
static_assert(Helper{whitebit::protocol::json::Type{whitebit::protocol::json::Type::TRADFI_FUTURES}} == roq::SecurityType::FUTURES);

template <>
template <>
std::optional<roq::SecurityType> Map<whitebit::protocol::json::Type>::helper() const {
  return Helper{args_};
}

// whitebit::protocol::json::TradeType ==> roq::Side

template <>
template <>
constexpr Helper<whitebit::protocol::json::TradeType>::operator std::optional<roq::Side>() const {
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::TradeType::type_t;
    case UNDEFINED_INTERNAL:
      return roq::Side::UNDEFINED;
    case UNKNOWN_INTERNAL:
      return roq::Side::UNDEFINED;
    case BUY:
      return roq::Side::BUY;
    case SELL:
      return roq::Side::SELL;
  }
  return {};
}

static_assert(Helper{whitebit::protocol::json::TradeType{whitebit::protocol::json::TradeType::UNDEFINED_INTERNAL}} == roq::Side::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::TradeType{whitebit::protocol::json::TradeType::BUY}} == roq::Side::BUY);
static_assert(Helper{whitebit::protocol::json::TradeType{whitebit::protocol::json::TradeType::SELL}} == roq::Side::SELL);

template <>
template <>
std::optional<roq::Side> Map<whitebit::protocol::json::TradeType>::helper() const {
  return Helper{args_};
}

// roq ==>

// roq::Side ==> whitebit::protocol::json::TradeType

template <>
template <>
constexpr Helper<roq::Side>::operator std::optional<whitebit::protocol::json::TradeType>() const {
  switch (std::get<0>(args_)) {
    using enum roq::Side;
    case UNDEFINED:
      return whitebit::protocol::json::TradeType::UNDEFINED_INTERNAL;
    case BUY:
      return whitebit::protocol::json::TradeType::BUY;
    case SELL:
      return whitebit::protocol::json::TradeType::SELL;
  }
  return {};
}

static_assert(Helper{roq::Side::UNDEFINED} == whitebit::protocol::json::TradeType{whitebit::protocol::json::TradeType::UNDEFINED_INTERNAL});
static_assert(Helper{roq::Side::BUY} == whitebit::protocol::json::TradeType{whitebit::protocol::json::TradeType::BUY});
static_assert(Helper{roq::Side::SELL} == whitebit::protocol::json::TradeType{whitebit::protocol::json::TradeType::SELL});

template <>
template <>
std::optional<whitebit::protocol::json::TradeType> Map<roq::Side>::helper() const {
  return Helper{args_};
}

}  // namespace roq
