#pragma once

#include "controllers/ScopeController.h"
#include "controls/ScopeButtonCtrl.h"
#include "resource.h"

namespace scope {
	namespace gui {

/** Property page for storage related options */
class CScanModesSettingsPage :
	public CPropertyPageImpl<CScanModesSettingsPage> {

protected:
	/** List of all areas, selection updates radio buttons */
	CListBox areas_list;

	/** @name Radio buttons for all different scan modes that Scope knows. These area connected to static ScopeButtons inside ScopeController.
	* @{ */
	CScopeButtonCtrl framesaw_radio;
	CScopeButtonCtrl framebidi_radio;
	CScopeButtonCtrl framehopper_radio;
	CScopeButtonCtrl frameresonancebidi_radio;
	CScopeButtonCtrl frameresonancehopper_radio;
	CScopeButtonCtrl linestraight_radio;
	/** @} */

protected:
	/** Checks radio buttons according to the scan mode of the selected area */
	void SetChecks(const uint32_t& _a);

public:
	enum { IDD = IDD_SCANMODES_PROPPAGE };

	BEGIN_MSG_MAP(CScanModesSettingsPage)
		MSG_WM_INITDIALOG(OnInitDialog);
		COMMAND_HANDLER(IDC_AREAS_LIST, LBN_SELCHANGE, OnAreaChange)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CScanModesSettingsPage();

	/** @name Called via Win32 messages
	* @{ */
	/** Handles dialog initialization */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** handles selection changes in area list box */
	LRESULT OnAreaChange(WORD, WORD, HWND, BOOL&);
	/** @} */
};

}}