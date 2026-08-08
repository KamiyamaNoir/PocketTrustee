#include "driver_multiplexer.hpp"

namespace {
    CoreMultiplexer::MultiplexerChannel _channel = CoreMultiplexer::MUX_NONE;
}

CoreMultiplexer::MultiplexerChannel CoreMultiplexer::Get() {
    return _channel;
}

void CoreMultiplexer::Set(MultiplexerChannel channel) {
    DEBUG_INFO("Multiplexer at %d", channel);

    _channel = channel;
    ll_multiplexer_set(channel);
}
