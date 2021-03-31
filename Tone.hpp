#pragma once

#include "Note.hpp"

class Tone {

public:
	virtual double callback(Note note, double actual_time) = 0;
	virtual ~Tone() {}
};