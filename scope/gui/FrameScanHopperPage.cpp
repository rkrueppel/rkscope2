#include "StdAfx.h"
#include "FrameScanHopperPage.h"
#include "PresetNameDlg.h"
#include "parameters/Framescan.h"
#include "helpers/ScopeButton.h"
#include "resource.h"

namespace scope {
	namespace gui {

CFrameScanHopperPage::CFrameScanHopperPage(const uint32_t& _area)
	: CFrameScanBasePage(_area, scope_controller.GuiParameters.areas[_area]->FrameHopper()) {
	
	// Overwrite base class enum { IDD = ... }
	// Use different dialog resources depending if this area is a slave area (only Pockels and ETL sliders) or a master area (full control complement)
	if ( scope_controller.GuiParameters.areas[area]->isslave() )
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_SLAVE_PROPPAGE);
	else
		m_psp.pszTemplate = MAKEINTRESOURCE(IDD_FRAMESCAN_HOPPER_PROPPAGE);
}

BOOL CFrameScanHopperPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
	// Call base class dialog initialization
	CFrameScanBasePage::OnInitDialog(wndFocus, lInitParam);

	// Add the plane list
	// Also slave areas have this!
	planes_list.Attach(GetDlgItem(IDC_PLANES_LIST));
	planes_list.InsertColumn(0, L"Plane", 0, 40);
	planes_list.InsertColumn(1, L"FastZ", 0, 40);
	planes_list.InsertColumn(2, L"Pockels", 0, 55);

	UpdatePlanesList();

	SetMsgHandled(true);
	return 0;
}

void CFrameScanHopperPage::UpdatePlanesList() {
	planes_list.DeleteAllItems();
	CString tmp;
	for ( uint32_t p = 0 ; p < scope_controller.GuiParameters.areas[area]->FrameHopper().planes.size() ; p++ ) {
		planes_list.InsertItem(p, L"Slice", 0);
		tmp.Format(L"%d", p);
		planes_list.SetItemText(p, 0, tmp);							// Slice no
		tmp.Format(L"%.1f", scope_controller.GuiParameters.areas[area]->FrameHopper().planes[p].position());
		planes_list.SetItemText(p, 1, tmp);							// fast z position
		tmp.Format(L"%.2f", scope_controller.GuiParameters.areas[area]->FrameHopper().planes[p].pockels());
		planes_list.SetItemText(p, 2, tmp);							// pockels
	}
}

LRESULT CFrameScanHopperPage::OnAddPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	parameters::PlaneProperties plane(scope_controller.GuiParameters.areas[area]->FrameHopper().fastz(), scope_controller.GuiParameters.areas[area]->FrameHopper().pockels());
	scope_controller.GuiParameters.areas[area]->FrameHopper().planes.push_back(plane);

	UpdatePlanesList();

	return 0;
}

LRESULT CFrameScanHopperPage::OnDeletePlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	int32_t sel = planes_list.GetSelectedIndex();
	if ( sel == -1 )
		return 0;

	// Delete the selected plane from the vector
	scope_controller.GuiParameters.areas[area]->FrameHopper().planes.erase(sel + std::begin(scope_controller.GuiParameters.areas[area]->FrameHopper().planes));
	UpdatePlanesList();

	return 0;
}


}

}