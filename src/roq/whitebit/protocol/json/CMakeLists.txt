set(TARGET_NAME ${PROJECT_NAME}-protocol-json)

set(SOURCES encoder.cpp map.cpp parser.cpp utils.cpp)

include(RoqAutogen)

set(AUTOGEN_SCHEMAS
    market_info_ack.json
    market_info_ack_item.json
    type.json
    pong.json
    method.json
    trades_update.json
    trades_update_params_item.json
    trades_update_params_item_item.json
    depth_update.json
    depth_update_params_item.json
    depth_update_params_item_data.json
    depth_update_params_item_data_level.json
    book_ticker_update_params_item.json
    book_ticker_update.json
    account_info_ack.json
    account_info_ack_result.json
    account_type.json
    amend_order_ack.json
    amend_order_ack_result.json
    auth.json
    cancel_all_orders_ack.json
    cancel_all_orders_ack_result.json
    cancel_all_orders_ack_result_list_item.json
    cancel_order_ack.json
    cancel_order_ack_result.json
    cancel_type.json
    category.json
    contract_type.json
    error.json
    event_type.json
    exec_type.json
    execution.json
    execution_data_item.json
    executions_ack.json
    executions_ack_result.json
    executions_ack_result_list_item.json
    leverage_filter.json
    lot_size_filter.json
    message.json
    operation.json
    options_type.json
    order_data_item.json
    order.json
    orders_ack.json
    orders_ack_result.json
    orders_ack_result_list_item.json
    order_status.json
    order_type.json
    place_order_ack.json
    place_order_ack_result.json
    position_data_item.json
    position.json
    positions_ack.json
    positions_ack_result.json
    positions_ack_result_list_item.json
    position_status.json
    price_filter.json
    reject_reason.json
    side.json
    status.json
    stop_order_type.json
    subscribe_data.json
    subscribe.json
    tick_direction.json
    time_in_force.json
    topic.json
    tp_sl_mode.json
    trigger_by.json
    wallet_balance_ack.json
    wallet_balance_ack_result.json
    wallet_balance_ack_result_list_item_coin_item.json
    wallet_balance_ack_result_list_item.json
    wallet_data_item_coin_item.json
    wallet_data_item.json
    wallet.json)

roq_autogen_hpp(
  OUTPUT
  AUTOGEN_HEADERS
  SOURCES
  ${AUTOGEN_SCHEMAS}
  TEMPLATE_DIR
  ${TEMPLATE_DIR}
  TEMPLATE_TYPE
  "json")

add_custom_target(protocol-json-autogen-headers ALL DEPENDS ${AUTOGEN_HEADERS})

roq_autogen_cpp(
  OUTPUT
  AUTOGEN_SOURCES
  SOURCES
  ${AUTOGEN_SCHEMAS}
  TEMPLATE_DIR
  ${TEMPLATE_DIR}
  TEMPLATE_TYPE
  "json")

roq_gitignore(OUTPUT .gitignore SOURCES ${TARGET_NAME} ${AUTOGEN_HEADERS} ${AUTOGEN_SOURCES})

add_library(${TARGET_NAME} OBJECT ${SOURCES} ${AUTOGEN_SOURCES})

add_dependencies(${TARGET_NAME} protocol-json-autogen-headers)

target_link_libraries(${TARGET_NAME} PRIVATE roq-core::roq-core roq-codec::roq-codec roq-utils::roq-utils roq-logging::roq-logging fmt::fmt)
