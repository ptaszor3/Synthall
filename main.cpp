#include <iostream>
#include <fstream>
#include <stdint.h>
#include <cmath>
#include <limits>

#include "Instrument.hpp"
#include "./tones/synthesizers.hpp"
#include "./tones/basic.hpp"
#include "./envelopes/basic.hpp"
#include "./envelopes/arsd.hpp"
#include "./effects/VolumeControl.hpp"

constexpr double pi{3.1415926535897932384626};

template <typename T>
void little_put(std::ostream& file, T toPut) {

        for(int i = 0; i < sizeof(toPut); i++) {

                file.put(static_cast<uint8_t>(toPut & 0xFF));
                toPut >>= 8;
        }
}

template <typename uintx_t>
void write_WAVE(std::ostream& file, uintx_t *data, uint32_t numberOfSamples , uint32_t sampleRate, uint16_t numberOfChannels) {

        uint32_t Subchunk2Size = numberOfSamples * numberOfChannels * sizeof(uintx_t);
        uint32_t Chunksize = Subchunk2Size + 36;
        file << "RIFF";
        little_put(file, static_cast<uint32_t>(Chunksize));
        file << "WAVE";
        file << "fmt ";
        little_put(file, static_cast<uint32_t>(16));
        little_put(file, static_cast<uint16_t>(1));
        little_put(file, numberOfChannels);
        little_put(file, sampleRate);
        little_put(file, static_cast<uint32_t>(sampleRate * numberOfChannels * sizeof(uintx_t)));
        little_put(file, static_cast<uint16_t>(numberOfChannels * sizeof(uintx_t)));
        little_put(file, static_cast<uint16_t>(sizeof(uintx_t) * 8));
        file << "data";
        little_put(file, static_cast<uint32_t>(numberOfSamples * numberOfChannels * sizeof(uintx_t)));
        
        for(int i = 0; i < numberOfSamples; i++)
                for(int j = 0; j < numberOfChannels; j++)
                        little_put(file, data[i][j]);
}

int main() {

	envelopes::arsd::Quadratic envelope;
	tones::basic::Saw tone;
	effects::VolumeControl volume_control;
	Instrument basic_instrument(&tone, &envelope);

	basic_instrument.effects.push_back(&volume_control);
	volume_control.volume = 0.2;
	//tone->setHarmonic(0.5, 1);

	envelope.arsd = {0.1, 0.1, 0.5, 5};

        std::ofstream file("whatever.wav", std::ios::binary | std::ios::out);
        
        constexpr unsigned int size = 1000000;
        constexpr unsigned int channels = 1;
        uint16_t volatile  data[size][channels];
        
        for(int i = 0; i < size; i++)
                for(int j = 0; j < channels; j++) {
                        data[i][j] = basic_instrument.callback(i / 44100.0) * std::numeric_limits<uint16_t>::max() / 2;
			if(i == 200000)
				basic_instrument.play(Note(440, i / 44100.0, 0));
			if(i == 210000)
				basic_instrument.play(Note(554.36, i / 44100.0, 0));
			if(i == 220000)
				basic_instrument.play(Note(659.225, i / 44100.0, 0));
			
			if(i == 300000)
				basic_instrument.stop_notes(i / 44100.0);
		}

        write_WAVE(file, data, size, 44100, channels);
}

