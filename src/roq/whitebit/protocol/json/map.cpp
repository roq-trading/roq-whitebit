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

// whitebit::protocol::json::EventType ==> roq::UpdateType

template <>
template <>
constexpr Helper<whitebit::protocol::json::EventType>::operator std::optional<roq::UpdateType>() const {
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::EventType::type_t;
    case UNDEFINED_INTERNAL:
      return UpdateType::UNDEFINED;
    case UNKNOWN_INTERNAL:
      return UpdateType::UNDEFINED;
    case ERROR:
      return UpdateType::UNDEFINED;
    case SNAPSHOT:
      return UpdateType::SNAPSHOT;
    case DELTA:
      return UpdateType::INCREMENTAL;
    case COMMAND_RESP:
      return UpdateType::UNDEFINED;
  }
  return {};
}

static_assert(Helper{whitebit::protocol::json::EventType{whitebit::protocol::json::EventType::UNDEFINED_INTERNAL}} == roq::UpdateType::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::EventType{whitebit::protocol::json::EventType::SNAPSHOT}} == roq::UpdateType::SNAPSHOT);
static_assert(Helper{whitebit::protocol::json::EventType{whitebit::protocol::json::EventType::DELTA}} == roq::UpdateType::INCREMENTAL);
static_assert(Helper{whitebit::protocol::json::EventType{whitebit::protocol::json::EventType::COMMAND_RESP}} == roq::UpdateType::UNDEFINED);

template <>
template <>
std::optional<roq::UpdateType> Map<whitebit::protocol::json::EventType>::helper() const {
  return Helper{args_};
}

// whitebit::protocol::json::OptionsType ==> roq::OptionType

template <>
template <>
constexpr Helper<whitebit::protocol::json::OptionsType>::operator std::optional<roq::OptionType>() const {
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::OptionsType::type_t;
    case UNDEFINED_INTERNAL:
      return roq::OptionType::UNDEFINED;
    case UNKNOWN_INTERNAL:
      return roq::OptionType::UNDEFINED;
    case CALL:
      return roq::OptionType::CALL;
    case PUT:
      return roq::OptionType::PUT;
  }
  return {};
}

static_assert(Helper{whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::UNDEFINED_INTERNAL}} == roq::OptionType::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::CALL}} == roq::OptionType::CALL);
static_assert(Helper{whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::PUT}} == roq::OptionType::PUT);

template <>
template <>
std::optional<roq::OptionType> Map<whitebit::protocol::json::OptionsType>::helper() const {
  return Helper{args_};
}

// whitebit::protocol::json::OrderStatus ==> roq::OrderStatus

template <>
template <>
constexpr Helper<whitebit::protocol::json::OrderStatus>::operator std::optional<roq::OrderStatus>() const {
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::OrderStatus::type_t;
    case UNDEFINED_INTERNAL:
      return roq::OrderStatus::UNDEFINED;
    case UNKNOWN_INTERNAL:
      return roq::OrderStatus::UNDEFINED;
    case CREATED:
      return roq::OrderStatus::WORKING;
    case NEW:
      return roq::OrderStatus::WORKING;
    case REJECTED:
      return roq::OrderStatus::REJECTED;
    case PARTIALLY_FILLED:
      return roq::OrderStatus::WORKING;
    case PARTIALLY_FILLED_CANCELED:
      return roq::OrderStatus::CANCELED;
    case FILLED:
      return roq::OrderStatus::COMPLETED;
    case CANCELLED:
      return roq::OrderStatus::CANCELED;
    case UNTRIGGERED:
      return roq::OrderStatus::UNDEFINED;
    case TRIGGERED:
      return roq::OrderStatus::UNDEFINED;
    case DEACTIVATED:
      return roq::OrderStatus::UNDEFINED;
    case ACTIVE:
      return roq::OrderStatus::UNDEFINED;
  }
  return {};
}

static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::UNDEFINED_INTERNAL}} == roq::OrderStatus::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::CREATED}} == roq::OrderStatus::WORKING);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::NEW}} == roq::OrderStatus::WORKING);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::REJECTED}} == roq::OrderStatus::REJECTED);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::PARTIALLY_FILLED}} == roq::OrderStatus::WORKING);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::PARTIALLY_FILLED_CANCELED}} == roq::OrderStatus::CANCELED);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::FILLED}} == roq::OrderStatus::COMPLETED);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::CANCELLED}} == roq::OrderStatus::CANCELED);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::UNTRIGGERED}} == roq::OrderStatus::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::TRIGGERED}} == roq::OrderStatus::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::DEACTIVATED}} == roq::OrderStatus::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::OrderStatus{whitebit::protocol::json::OrderStatus::ACTIVE}} == roq::OrderStatus::UNDEFINED);

template <>
template <>
std::optional<roq::OrderStatus> Map<whitebit::protocol::json::OrderStatus>::helper() const {
  return Helper{args_};
}

// whitebit::protocol::json::OrderType ==> roq::OrderType

template <>
template <>
constexpr Helper<whitebit::protocol::json::OrderType>::operator std::optional<roq::OrderType>() const {
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::OrderType::type_t;
    case UNDEFINED_INTERNAL:
      return roq::OrderType::UNDEFINED;
    case UNKNOWN_INTERNAL:
      return roq::OrderType::UNDEFINED;
    case UNKNOWN:
      return roq::OrderType::UNDEFINED;
    case MARKET:
      return roq::OrderType::MARKET;
    case LIMIT:
      return roq::OrderType::LIMIT;
    case BLOCK_TRADE_LIMIT:
      return roq::OrderType::LIMIT;
  }
  return {};
}

static_assert(Helper{whitebit::protocol::json::OrderType{whitebit::protocol::json::OrderType::UNDEFINED_INTERNAL}} == roq::OrderType::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::OrderType{whitebit::protocol::json::OrderType::MARKET}} == roq::OrderType::MARKET);
static_assert(Helper{whitebit::protocol::json::OrderType{whitebit::protocol::json::OrderType::LIMIT}} == roq::OrderType::LIMIT);

template <>
template <>
std::optional<roq::OrderType> Map<whitebit::protocol::json::OrderType>::helper() const {
  return Helper{args_};
}

// whitebit::protocol::json::Side ==> roq::Side

template <>
template <>
constexpr Helper<whitebit::protocol::json::Side>::operator std::optional<roq::Side>() const {
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::Side::type_t;
    case UNDEFINED_INTERNAL:
      return roq::Side::UNDEFINED;
    case UNKNOWN_INTERNAL:
      return roq::Side::UNDEFINED;
    case NONE:
      return roq::Side::UNDEFINED;
    case BUY:
      return roq::Side::BUY;
    case SELL:
      return roq::Side::SELL;
  }
  return {};
}

static_assert(Helper{whitebit::protocol::json::Side{whitebit::protocol::json::Side::UNDEFINED_INTERNAL}} == roq::Side::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::Side{whitebit::protocol::json::Side::NONE}} == roq::Side::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::Side{whitebit::protocol::json::Side::BUY}} == roq::Side::BUY);
static_assert(Helper{whitebit::protocol::json::Side{whitebit::protocol::json::Side::SELL}} == roq::Side::SELL);

template <>
template <>
std::optional<roq::Side> Map<whitebit::protocol::json::Side>::helper() const {
  return Helper{args_};
}

// whitebit::protocol::json::Status ==> roq::TradingStatus

template <>
template <>
constexpr Helper<whitebit::protocol::json::Status>::operator std::optional<roq::TradingStatus>() const {
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::Status::type_t;
    case UNDEFINED_INTERNAL:
      return roq::TradingStatus::UNDEFINED;
    case UNKNOWN_INTERNAL:
      return roq::TradingStatus::UNDEFINED;
    case PRE_LAUNCH:
      return roq::TradingStatus::UNDEFINED;
    case TRADING:
      return roq::TradingStatus::OPEN;
    case SETTLING:
      return roq::TradingStatus::UNDEFINED;
    case DELIVERING:
      return roq::TradingStatus::UNDEFINED;
    case CLOSED:
      return roq::TradingStatus::CLOSE;
  }
  return {};
}

static_assert(Helper{whitebit::protocol::json::Status{whitebit::protocol::json::Status::UNDEFINED_INTERNAL}} == roq::TradingStatus::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::Status{whitebit::protocol::json::Status::PRE_LAUNCH}} == roq::TradingStatus::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::Status{whitebit::protocol::json::Status::TRADING}} == roq::TradingStatus::OPEN);
static_assert(Helper{whitebit::protocol::json::Status{whitebit::protocol::json::Status::SETTLING}} == roq::TradingStatus::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::Status{whitebit::protocol::json::Status::DELIVERING}} == roq::TradingStatus::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::Status{whitebit::protocol::json::Status::CLOSED}} == roq::TradingStatus::CLOSE);

template <>
template <>
std::optional<roq::TradingStatus> Map<whitebit::protocol::json::Status>::helper() const {
  return Helper{args_};
}

// whitebit::protocol::json::TimeInForce ==> roq::TimeInForce

template <>
template <>
constexpr Helper<whitebit::protocol::json::TimeInForce>::operator std::optional<roq::TimeInForce>() const {
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::TimeInForce::type_t;
    case UNDEFINED_INTERNAL:
      return roq::TimeInForce::UNDEFINED;
    case UNKNOWN_INTERNAL:
      return roq::TimeInForce::UNDEFINED;
    case GTC:
      return roq::TimeInForce::GTC;
    case FOK:
      return roq::TimeInForce::FOK;
    case IOC:
      return roq::TimeInForce::IOC;
    case POST_ONLY:
      return roq::TimeInForce::UNDEFINED;
  }
  return {};
}

static_assert(Helper{whitebit::protocol::json::TimeInForce{whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL}} == roq::TimeInForce::UNDEFINED);
static_assert(Helper{whitebit::protocol::json::TimeInForce{whitebit::protocol::json::TimeInForce::GTC}} == roq::TimeInForce::GTC);
static_assert(Helper{whitebit::protocol::json::TimeInForce{whitebit::protocol::json::TimeInForce::FOK}} == roq::TimeInForce::FOK);
static_assert(Helper{whitebit::protocol::json::TimeInForce{whitebit::protocol::json::TimeInForce::IOC}} == roq::TimeInForce::IOC);
static_assert(Helper{whitebit::protocol::json::TimeInForce{whitebit::protocol::json::TimeInForce::POST_ONLY}} == roq::TimeInForce::UNDEFINED);

template <>
template <>
std::optional<roq::TimeInForce> Map<whitebit::protocol::json::TimeInForce>::helper() const {
  return Helper{args_};
}

// whitebit::protocol::json::{ ContractType, OptionsType } ==> roq::SecurityType

template <>
template <>
constexpr Helper<whitebit::protocol::json::ContractType, whitebit::protocol::json::OptionsType>::operator std::optional<roq::SecurityType>() const {
  switch (std::get<1>(args_)) {
    using enum whitebit::protocol::json::OptionsType::type_t;
    case UNDEFINED_INTERNAL:
    case UNKNOWN_INTERNAL:
      break;
    case CALL:
    case PUT:
      return SecurityType::OPTION;
  }
  switch (std::get<0>(args_)) {
    using enum whitebit::protocol::json::ContractType::type_t;
    case UNDEFINED_INTERNAL:
    case UNKNOWN_INTERNAL:
      break;
    case INVERSE_PERPETUAL:
    case LINEAR_PERPETUAL:
      return SecurityType::SWAP;
    case LINEAR_FUTURES:
    case INVERSE_FUTURES:
      return SecurityType::FUTURES;
  }
  return SecurityType::SPOT;
}

static_assert(
    Helper{
        whitebit::protocol::json::ContractType{whitebit::protocol::json::ContractType::UNDEFINED_INTERNAL},
        whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::UNDEFINED_INTERNAL}} == roq::SecurityType::SPOT);
static_assert(
    Helper{
        whitebit::protocol::json::ContractType{whitebit::protocol::json::ContractType::UNDEFINED_INTERNAL},
        whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::CALL}} == roq::SecurityType::OPTION);
static_assert(
    Helper{
        whitebit::protocol::json::ContractType{whitebit::protocol::json::ContractType::UNDEFINED_INTERNAL},
        whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::PUT}} == roq::SecurityType::OPTION);
static_assert(
    Helper{
        whitebit::protocol::json::ContractType{whitebit::protocol::json::ContractType::INVERSE_PERPETUAL},
        whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::UNDEFINED_INTERNAL}} == roq::SecurityType::SWAP);
static_assert(
    Helper{
        whitebit::protocol::json::ContractType{whitebit::protocol::json::ContractType::LINEAR_PERPETUAL},
        whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::UNDEFINED_INTERNAL}} == roq::SecurityType::SWAP);
static_assert(
    Helper{
        whitebit::protocol::json::ContractType{whitebit::protocol::json::ContractType::LINEAR_FUTURES},
        whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::UNDEFINED_INTERNAL}} == roq::SecurityType::FUTURES);
static_assert(
    Helper{
        whitebit::protocol::json::ContractType{whitebit::protocol::json::ContractType::INVERSE_FUTURES},
        whitebit::protocol::json::OptionsType{whitebit::protocol::json::OptionsType::UNDEFINED_INTERNAL}} == roq::SecurityType::FUTURES);

template <>
template <>
std::optional<roq::SecurityType> Map<whitebit::protocol::json::ContractType, whitebit::protocol::json::OptionsType>::helper() const {
  return Helper{args_};
}

// roq ==>

// roq::OrderType ==> whitebit::protocol::json::OrderType

template <>
template <>
constexpr Helper<roq::OrderType>::operator std::optional<whitebit::protocol::json::OrderType>() const {
  switch (std::get<0>(args_)) {
    using enum roq::OrderType;
    case UNDEFINED:
      return whitebit::protocol::json::OrderType::UNDEFINED_INTERNAL;
    case MARKET:
      return whitebit::protocol::json::OrderType::MARKET;
    case LIMIT:
      return whitebit::protocol::json::OrderType::LIMIT;
  }
  return {};
}

static_assert(Helper{roq::OrderType::UNDEFINED} == whitebit::protocol::json::OrderType{whitebit::protocol::json::OrderType::UNDEFINED_INTERNAL});
static_assert(Helper{roq::OrderType::MARKET} == whitebit::protocol::json::OrderType{whitebit::protocol::json::OrderType::MARKET});
static_assert(Helper{roq::OrderType::LIMIT} == whitebit::protocol::json::OrderType{whitebit::protocol::json::OrderType::LIMIT});

template <>
template <>
std::optional<whitebit::protocol::json::OrderType> Map<roq::OrderType>::helper() const {
  return Helper{args_};
}

// roq::Side ==> whitebit::protocol::json::Side

template <>
template <>
constexpr Helper<roq::Side>::operator std::optional<whitebit::protocol::json::Side>() const {
  switch (std::get<0>(args_)) {
    using enum roq::Side;
    case UNDEFINED:
      return whitebit::protocol::json::Side::UNDEFINED_INTERNAL;
    case BUY:
      return whitebit::protocol::json::Side::BUY;
    case SELL:
      return whitebit::protocol::json::Side::SELL;
  }
  return {};
}

static_assert(Helper{roq::Side::UNDEFINED} == whitebit::protocol::json::Side{whitebit::protocol::json::Side::UNDEFINED_INTERNAL});
static_assert(Helper{roq::Side::BUY} == whitebit::protocol::json::Side{whitebit::protocol::json::Side::BUY});
static_assert(Helper{roq::Side::SELL} == whitebit::protocol::json::Side{whitebit::protocol::json::Side::SELL});

template <>
template <>
std::optional<whitebit::protocol::json::Side> Map<roq::Side>::helper() const {
  return Helper{args_};
}

// roq::TimeInForce ==> whitebit::protocol::json::TimeInForce

template <>
template <>
constexpr Helper<roq::TimeInForce>::operator std::optional<whitebit::protocol::json::TimeInForce>() const {
  switch (std::get<0>(args_)) {
    using enum roq::TimeInForce;
    case UNDEFINED:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case GFD:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case GTC:
      return whitebit::protocol::json::TimeInForce::GTC;
    case OPG:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case IOC:
      return whitebit::protocol::json::TimeInForce::IOC;
    case FOK:
      return whitebit::protocol::json::TimeInForce::FOK;
    case GTX:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case GTD:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case AT_THE_CLOSE:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case GOOD_THROUGH_CROSSING:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case AT_CROSSING:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case GOOD_FOR_TIME:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case GFA:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
    case GFM:
      return whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL;
  }
  return {};
}

static_assert(Helper{roq::TimeInForce::UNDEFINED} == whitebit::protocol::json::TimeInForce{whitebit::protocol::json::TimeInForce::UNDEFINED_INTERNAL});
static_assert(Helper{roq::TimeInForce::GTC} == whitebit::protocol::json::TimeInForce{whitebit::protocol::json::TimeInForce::GTC});
static_assert(Helper{roq::TimeInForce::IOC} == whitebit::protocol::json::TimeInForce{whitebit::protocol::json::TimeInForce::IOC});
static_assert(Helper{roq::TimeInForce::FOK} == whitebit::protocol::json::TimeInForce{whitebit::protocol::json::TimeInForce::FOK});

template <>
template <>
std::optional<whitebit::protocol::json::TimeInForce> Map<roq::TimeInForce>::helper() const {
  return Helper{args_};
}

}  // namespace roq
