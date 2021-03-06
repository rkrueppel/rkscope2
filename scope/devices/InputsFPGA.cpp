#include "stdafx.h"
#include "InputsFPGA.h"
#include "fpga\FPGAPhotonCounter.h"
#include "fpga\FPGADigitalDemultiplexer.h"
#include "fpga\FPGAResonanceScanner_NI5771.h"
#include "fpga\FPGAAnalogIntegrator.h"
#include "fpga\FPGAResonanceScanner.h"
#include "fpga\FPGAAnalogDemultiplexer.h"
#include "fpga\FPGAAnalogDemultiplexerResonance.h"
#include "fpga\FPGANoiseOutput.h"
#include "helpers\helpers.h"
#include "helpers/ScopeDatatypes.h"
#include "parameters\Inputs.h"
#include "parameters\Scope.h"
#include "helpers\ScopeException.h"

namespace scope {

	config::InputFPGAType& theFPGA() {
		static config::InputFPGAType staticFPGA;
		return staticFPGA;
	}

	InputsFPGA::InputsFPGA(const uint32_t& _area, parameters::InputsFPGA* _inputparams, parameters::Scope& _params)
		: Inputs(_area)
		, laserpulsesperpixel(1000.0) {
		try {
			double pt = 0.0;
			theFPGA().Initialize(_inputparams);

			uint32_t samplesperchan = 0;
			// two more lines/preframelines are required, because in resonance scanner mode some pixels are thrown away at the beginning until the first line is triggered with the sync signal
			samplesperchan = _params.allareas[_area]->Currentframe().TotalPixels() + _inputparams->preframelines() * _params.allareas[_area]->Currentframe().XTotalPixels();
			if ( _params.requested_mode() == DaqModeHelper::nframes) {
				samplesperchan *= _params.allareas[_area]->daq.requested_frames() * _params.allareas[_area]->daq.averages();
			}

			DBOUT(L"Requested pixeltime area " << _area << L": " << _params.allareas[_area]->daq.pixeltime());
			pt = theFPGA().SetPixeltime(_area, _params.allareas[_area]->daq.pixeltime());
			DBOUT(L"Real pixeltime area " << _area << L": " << pt);
			laserpulsesperpixel = pt * 1E-6 * 80E6;
			DBOUT(L"Laser pulses per pixel " << laserpulsesperpixel);

			_params.allareas[_area]->daq.pixeltime = pt;
			DBOUT(L"Requested pixels " << samplesperchan);
			theFPGA().SetRequestedPixels(_area, samplesperchan);
			requested_samples = samplesperchan;

			if ( _params.requested_mode() == DaqModeHelper::continuous )
				theFPGA().SetContinuousAcquisition(true);		// overrides requested pixels on FPGA VI
			else
				theFPGA().SetContinuousAcquisition(false);

			// If T-Series is triggered the FPGA waits for external trigger (and all other tasks waiting for it)
			theFPGA().SetTriggering((_params.requested_mode() == DaqModeHelper::nframes) && _params.timeseries.triggered());
		
			// For diagnosis: Connect the FPGA generated sample clock to a DAQmx output terminal
			// if ( _area == 1 ) {
			//	DAQmxDisconnectTerms("/PXI-6259_0/PXI_Trig1", "/PXI-6259_0/PFI0");
			// }

			theFPGA().SetScannerdelay(_params.allareas[_area]->daq.ScannerDelaySamples(false));

		} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	}


	InputsFPGA::~InputsFPGA(void) {
		Stop();
	}

	void InputsFPGA::Start(void) {
		try {
			theFPGA().StartAcquisition();
		} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	}

	void InputsFPGA::Stop(void) {
		try {
			theFPGA().StopAcquisition();
		} catch (...) { ScopeExceptionHandler(__FUNCTION__); }
	}

	int32_t InputsFPGA::Read(DaqMultiChunk<2, 1, uint16_t>& _chunk, bool& _timedout, const double& _timeout) {
		int32_t read = 0;
		try {
			read = theFPGA().ReadPixels(masterarea, _chunk, _timeout, _timedout);
			if (_timedout) {
				DBOUT(L"InputsFPGA::Read area " << masterarea << L" timed out");
			}
			DBOUT(L"InputsFPGA::Read area " << masterarea << L" read " << read);
			theFPGA().CheckFPGADiagnosis();
		}
		catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		return read;
	}

	int32_t InputsFPGA::Read(DaqMultiChunk<2, 2, uint16_t>& _chunk, bool& _timedout, const double& _timeout) {
		int32_t read = 0;
		try {
			read = theFPGA().ReadPixels(masterarea, _chunk, _timeout, _timedout);
			if (_timedout) {
				DBOUT(L"InputsFPGA::Read area " << masterarea << L" timed out");
			}
			DBOUT(L"InputsFPGA::Read area " << masterarea << L" read " << read);
			theFPGA().CheckFPGADiagnosis();
		}
		catch (...) { ScopeExceptionHandler(__FUNCTION__); }
		return read;
	}

}

