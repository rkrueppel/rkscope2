#pragma once

#include "FPGAInterface.h"
#include "FPGAIO5771.h"
#include "NiFpga_AnalogIntegrator_NI5771.h"

// Forward declaration
namespace scope {
	namespace parameters {
		class InputsFPGAAnalogIntegrator;
	}
}

namespace scope {

	/** Wraps an FPGA that does analog pixel integration (with the NI 5771 FlexRIO adapter module http://sine.ni.com/nips/cds/view/p/lang/en/nid/210735)
	* @ingroup ScopeComponentsHardware */
	class FPGAAnalogIntegrator :
		public FPGAInterface,
		public FPGAIO5771,
		public SupportedAreas<1, 1> {

	protected:
		/** the parameter set */
		parameters::InputsFPGAAnalogIntegrator* parameters;

		/** both fifos for both channels */
		std::array<NiFpga_AnalogIntegrator_NI5771_TargetToHostFifoU32, 2> fifos;

	public:
		/** Load the FPGA bitfile, set the IO module's onboard clock and initialize the acquisition */
		FPGAAnalogIntegrator();

		/** Close FPGA session */
		~FPGAAnalogIntegrator();

		void Initialize(parameters::InputsFPGA* _parameters) override;
		double SetPixeltime(const uint32_t& _area, const double& _pixeltime) override;
		double SetLinetime(const uint32_t& _area, const double& _linetime) override;
		void SetTriggering(const bool& _waitfortrigger) override;
		void SetContinuousAcquisition(const bool& _cont) override;
		void SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) override;
		void StartAcquisition() override;
		void StopAcquisition()  override;

		/** @param[in] _area has to be 0 since only one area is supported */
		int32_t ReadPixels(const uint32_t& _area, DaqMultiChunk<2, 1, uint16_t>& _chunk, const double& _timeout, bool& _timedout) override;

		/** Set channel properties as baseline and bitshift */
		void SetChannelProps();

		/** Checks the status of the FIFOs on the FPGA */
		void CheckFPGADiagnosis();

		/** Clears the interloop and ToHost FIFOs */
		void ClearFIFOs();
	};

}