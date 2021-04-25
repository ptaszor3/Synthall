#pragma once

#include "../Envelope.hpp"

namespace envelopes {
        namespace basic {
                class NoEnvelope :public Envelope {
                public:
                        Sample callback(Note note, DoubleSeconds duration_from_start);
                };
        }
}
