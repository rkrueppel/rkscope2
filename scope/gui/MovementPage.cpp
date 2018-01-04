#include "stdafx.h"
#include "MovementPage.h"
#include "MovementSettingsSheet.h"

namespace scope {
	namespace gui {
		
		CMovementPage::CMovementPage(
			std::vector<parameters::MasterArea>& _masterareas
			, std::vector<parameters::SlaveArea>& _slaveareas
			, std::array<FPUButtons, config::nmasters>& _masterfpubuttons
			, std::array<FPUButtons, config::nslaves>& _slavefpubuttons
			, const double& _masterfovsizex
			, const double& _masterfovsizey
			, config::XYZStageParametersType& _stageparams
			, ZeroButtons& _zerobuttons
		)
			: settings_sheet(_masterareas, _slaveareas, _masterfpubuttons, _slavefpubuttons, _masterfovsizex, _masterfovsizey, _stageparams, _zerobuttons)
		{
			
		}


		BOOL CMovementPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {	
			// Replace placeholder with scan settings property sheet
			HWND hWndPlaceHolder = GetDlgItem(IDC_MOVEMENTSETTINGS_HOLDER);
			::ShowWindow(hWndPlaceHolder, FALSE);	
			CRect rc;	
			::GetWindowRect(hWndPlaceHolder, &rc);		
			ScreenToClient(&rc);
			settings_sheet.Create(m_hWnd, 0, rc);

			SetMsgHandled(false);
			return 0;
		}

	}
}