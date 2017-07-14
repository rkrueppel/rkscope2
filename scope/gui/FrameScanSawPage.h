#pragma once

#include "FrameScanBasePage.h"
#include "controls/ScopeSliderCtrl.h"
#include "controls/ScopeEditCtrl.h"
#include "controls/ScopeButtonCtrl.h"
#include "controls/ScopeCheckBoxCtrl.h"
#include "controllers/ScopeController.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Property page for sawtooth frame scan */
class CFrameScanSawPage
	: public CFrameScanBasePage {

protected:
	/** cutoff fraction for x scanner */
	CScopeEditCtrl<double> xcutoff_edit;

	/** cutoff fraction for y scanner */
	CScopeEditCtrl<double> ycutoff_edit;

	/** retrace fraction for x scanner */
	CScopeEditCtrl<double> xretrace_edit;

	/** retrace fraction for y scanner */
	CScopeEditCtrl<double> yretrace_edit;
	
public:
	/** create the edit objects (e.g. ScopeEditImpl) and tie them to the global variables (ScopeValues)
	* set connect back to true -> on change of ScopeValue (e.g. by ScopeController) the dialog control will
	* be updated accordingly */
	CFrameScanSawPage(const uint32_t& _area);

	~CFrameScanSawPage() { }

	// Only needed if we add handlers here (see e.g. CFrameScanHopperPage)
	BEGIN_MSG_MAP(CFrameScanSawPage)
		CHAIN_MSG_MAP(CFrameScanBasePage)
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	/** Attach the dialog elements */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}}