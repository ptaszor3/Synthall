#include "UnsynchronizedVibrato.hpp"

namespace effects {
	double UnsynchronizedVibrato::callback(Note note, Instrument *instrument, DoubleSeconds duration_from_start, int effects_position) { 
		return instrument->callback_single_sample_effect_prior_to(note, duration_from_start + DoubleSeconds(shape->callback(Note(frequency, note.begin_time, note.end_time, 1), duration_from_start - note.begin_time) * max_time_deviation), effects_position);
	}
	
	UnsynchronizedVibrato::UnsynchronizedVibrato(Signal *c_shape, double c_frequency, DoubleSeconds c_max_time_deviation) 
	:shape{c_shape}, frequency{c_frequency}, max_time_deviation{c_max_time_deviation}
	{}
}