#include "StdAfx.h"
#include "XYControl.h"
#include "parameters/Devices.h"
#include "controllers/ScopeLogger.h"

namespace scope {

XYControl::XYControl()
	: initialized(false)
	, pollinterval(1000) {
}

XYControl::~XYControl() {
	StopPolling();
}

void XYControl::Initialize(parameters::XYControl& _params) {
	pos[0] = &_params.xpos;
	pos[1] = &_params.ypos;
	pollinterval = round2ui32(_params.pollinterval());
	initialized = true;
	// Polling more frequently most definitely will lead to problems...
	if ( pollinterval > 100 ) {
		// we need to call from the this pointer to call the most derived version of StartPolling
		this->StartPolling();
	}
}

void XYControl::StartPolling() {
	// If something went wrong during initialization we do not want to poll (and probably cause more errors)
	if ( initialized ) {
		fut = std::async([this]() {
			while (!stop.IsSet()) {
				UpdatePositionValues();
				std::this_thread::sleep_for(std::chrono::milliseconds(pollinterval));
			} 
			return true;
		} );
	}
}

void XYControl::StopPolling() {
	stop.Set();
	if ( fut.valid() )
		fut.wait();
}

void XYControl::UpdatePositionValues() {
	// Do nothing
}

}