include(RoqAutogen)

set(TARGET_NAME ${PROJECT_NAME}-test)

set(SOURCES
    json_market_info_ack.cpp
    json_pong.cpp
    json_trades_update.cpp
    json_depth_update.cpp
    json_book_ticker_update.cpp
    json_account_info_ack.cpp
    json_amend_order_ack.cpp
    json_auth.cpp
    json_cancel_all_orders_ack.cpp
    json_cancel_order_ack.cpp
    json_encoder.cpp
    json_execution_2.cpp
    json_executions_ack.cpp
    json_order.cpp
    json_orders_ack.cpp
    json_place_order_ack.cpp
    json_position.cpp
    json_positions_ack.cpp
    json_subscribe.cpp
    json_wallet_balance_ack.cpp
    json_wallet.cpp
    main.cpp)

roq_gitignore(OUTPUT .gitignore SOURCES ${TARGET_NAME})

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_NAME}-protocol-json roq-server::roq-server Catch2::Catch2)

if(ROQ_BUILD_TYPE STREQUAL "Release")
  set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
