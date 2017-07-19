#include "StdAfx.h"
#include "FrameScanSawPage.h"
#include "PresetNameDlg.h"
#include "parameters/Framescan.h"
#include "helpers/ScopeButton.h"
#include "resource.h"

namespace scope {
	namespace gui {

CFrameScanSawPage::CFrameScanSawPage(const uint32_t& _area)
	: CFrameScanBasePage(_area, scope_controller.GuiParameters.areas[_area]->FrameSaw())
	, xcutoff_edit(&scope_controller.GuiParameters.areas[_area]->FrameSaw().xcutoff, true, true)
	, ycutoff_edit(&scope_controller.GuiParameters.areas[_area]->FrameSaw().ycutoff, true, true)
	, xretrace_edit(&scope_controller.GuiParameters.areas[_area]->FrameSaw().xretrace, true, true)
	, yretrace_edit(&scope_controller.GuiParameters.areas[_area]->FrameSaw().yretrace, true, true) {

	// Overwrite base class enum { IDD = ... }
	// Use different dialog resources depending if this area is a slave area (only Pockels and ETL sliders) or a master area (full control complement)
	if ( scope_controller.GuiParameters.areas[area]->isslave() )
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_SLAVE_PROPPAGE);
	else
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_SAW_PROPPAGE);
}

BOOL CFrameScanSawPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
	// Call base class dialog initialization
	CFrameScanBasePage::OnInitDialog(wndFocus, lInitParam);

	// Add the additional dialog elements
	xcutoff_edit.AttachToDlgItem(GetDlgItem(IDC_XCUTOFF_EDIT));
	ycutoff_edit.AttachToDlgItem(GetDlgItem(IDC_YCUTOFF_EDIT));
	xretrace_edit.AttachToDlgItem(GetDlgItem(IDC_XRETRACE_EDIT));
	yretrace_edit.AttachToDlgItem(GetDlgItem(IDC_YRETRACE_EDIT));

	SetMsgHandled(true);
	return 0;
}


}

}