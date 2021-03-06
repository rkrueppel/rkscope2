#include "stdafx.h"
#include "Inputs.h"

namespace scope {

	Inputs::Inputs(const uint32_t& _area)
		: masterarea(_area) {
		requested_samples = 10000;
	}

	Inputs::~Inputs(void) {

	}

	uint32_t Inputs::RequestedSamples() const {
		return requested_samples;
	}

}
