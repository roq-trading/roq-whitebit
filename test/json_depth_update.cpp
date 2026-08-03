/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "parser_tester.hpp"

using namespace roq;
using namespace roq::whitebit;

using namespace std::literals;
using namespace std::chrono_literals;

using value_type = protocol::json::DepthUpdate;

// note! truncated
TEST_CASE("snapshot", "[json_depth_update]") {
  auto message = R"({)"
                 R"("method": "depth_update", )"
                 R"("params": [)"
                 R"(true, {)"
                 R"("timestamp": 1785730130.6040511, )"
                 R"("update_id": 22216665449, )"
                 R"("asks": [)"
                 R"(["62957.3", "0.017"], )"
                 R"(["62961.4", "0.029"], )"
                 R"(["63038.7", "0.285"])"
                 R"(], )"
                 R"("bids": [)"
                 R"(["62957.2", "0.017"], )"
                 R"(["62951.7", "0.004"], )"
                 R"(["62803.3", "1.09"])"
                 R"(], )"
                 R"("event_time": 1785730135.194067)"
                 R"(}, )"
                 R"("BTC_PERP")"
                 R"(], )"
                 R"("id": null)"
                 R"(})"sv;
  auto helper = [](value_type const &obj) {
    // CHECK(obj.topic == "orderbook.1.BTCUSDT"sv);
    // CHECK(depth == 1);
  };
  ParserTester<value_type>::dispatch(helper, message, 8192, 1);
}

TEST_CASE("incremental", "[json_depth_update]") {
  auto message = R"({)"
                 R"("method": "depth_update", )"
                 R"("params": [)"
                 R"(false, {)"
                 R"("timestamp": 1785730136.175524, )"
                 R"("update_id": 22216668616, )"
                 R"("past_update_id": 22216668570, )"
                 R"("asks": [)"
                 R"(["62925.8", "0.13"], )"
                 R"(["62932.1", "0.328"], )"
                 R"(["62934.2", "0.31"], )"
                 R"(["62940.4", "0.541"], )"
                 R"(["62942.5", "0.499"], )"
                 R"(["62948.8", "0.836"], )"
                 R"(["62950.9", "0.593"], )"
                 R"(["62957.2", "0.839"], )"
                 R"(["62997.2", "0"])"
                 R"(], )"
                 R"("bids": [)"
                 R"(["62820.2", "0.453"], )"
                 R"(["62818.1", "0.499"], )"
                 R"(["62816", "0.446"], )"
                 R"(["62813.9", "0.496"], )"
                 R"(["62788.7", "0.906"], )"
                 R"(["62784.6", "1.032"], )"
                 R"(["62780.4", "1.548"], )"
                 R"(["62776.2", "1.351"])"
                 R"(], )"
                 R"("event_time": 1785730136.2352009)"
                 R"(}, )"
                 R"("BTC_PERP")"
                 R"(], )"
                 R"("id": null)"
                 R"(})"sv;
  auto helper = [](value_type const &obj) {
    // CHECK(obj.topic == "orderbook.1.BTCUSDT"sv);
    // CHECK(depth == 1);
  };
  ParserTester<value_type>::dispatch(helper, message, 8192, 1);
}
