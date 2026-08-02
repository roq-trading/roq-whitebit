/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/whitebit/application.hpp"

#include "roq/whitebit/flags/settings.hpp"

#include "roq/whitebit/gateway/config.hpp"
#include "roq/whitebit/gateway/controller.hpp"

using namespace std::literals;

namespace roq {
namespace whitebit {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  flags::Settings settings{args};
  gateway::Config config{settings};
  auto context = server::create_io_context(settings);
  server::Trading<gateway::Controller>{settings, config, *context}.dispatch();
  return EXIT_SUCCESS;
}

}  // namespace whitebit
}  // namespace roq
