#pragma once

#include "controls/ScopeLEDCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controllers/ScopeController.h"
#include "devices/fpga/FPGAAnalogDemultiplexer.h"
#include "resource.h"

// Forward declaration
namespace scope {
	namespace parameters {
		class InputsFPGAAnalogDemultiplexer;
	}
}

namespace scope {
	namespace gui {

/** Class for the analog demultiplexer FPGA infos property page */
class CFPGAAnalogDemultiplexerPage :
	public CPropertyPageImpl<CFPGAAnalogDemultiplexerPage> {

protected:
	
public:
	enum { IDD = IDD_FPGAANALOGDEMULTIPLEXER_PROPPAGE };

	/** @name LEDs for FPGA status
	* @{ */
	CScopeLEDCtrl tohostoverflowA1_led;
	CScopeLEDCtrl tohostoverflowA2_led;
	CScopeLEDCtrl interloopoverflow_led;
	CScopeLEDCtrl interlooptimeout_led;
	CScopeLEDCtrl acquiring_led;
	CScopeLEDCtrl aioverrange_led;
	/** @} */

	/** @name Baseline and bitshift for both channels. Baseline is around 127 and bitshift for conversion from U32 to U16, start at around 3?!
	* @{ */
	CScopeEditCtrl<uint8_t> bitshiftch1a1_edit;
	CScopeEditCtrl<uint8_t> bitshiftch1a2_edit;
	CScopeEditCtrl<uint8_t> baselinech1_edit;
	CScopeEditCtrl<uint8_t> baselinech2_edit;
	CScopeEditCtrl<uint8_t> cutoffch1_edit;
	CScopeEditCtrl<uint8_t> cutoffch2_edit;
	CScopeEditCtrl<uint8_t> bitshiftch2a1_edit;
	CScopeEditCtrl<uint8_t> bitshiftch2a2_edit;
	/** @} */

	/** histogram range set option for Area1 and Area2*/
	CScopeEditCtrl<uint32_t> hist_range1_edit;
	CScopeEditCtrl<uint32_t> hist_range2_edit;

	/** Connect to the FIFOStatus ScopeNumbers */
	CFPGAAnalogDemultiplexerPage(parameters::InputsFPGAAnalogDemultiplexer& _parameters, ScopeValue<uint32_t>& _histrange1, ScopeValue<uint32_t>& _histrange2);

	BEGIN_MSG_MAP(CFPGAAnalogDemultiplexerPage)
		MSG_WM_INITDIALOG(OnInitDialog);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}}