#include "stdafx.h"
#include "StackSettingsPage.h"

namespace scope {
	namespace gui {

		CStackSettingsPage::CStackSettingsPage(
			parameters::Stack& _stackparams
			, RunButtons& _runbuttons
			, StackButtons& _stackbuttons
			, ScopeCounters& _counters
			, config::XYZStageParametersType& _stageparams
			, std::vector<std::unique_ptr<parameters::BaseArea>>& _allareas
		)
			: initialized(false)
			, stackparams(_stackparams)
			, stageparams(_stageparams)
			, allareas(_allareas)
			, start_stack_button(_runbuttons.startstack)
			, startat_edit(_stackparams.startat[0].position, true, true)
			, stopat_edit(_stackparams.stopat[0].position, true, true)
			, spacing_edit(_stackparams.spacing, true, true)
			, starthere_button(_stackbuttons.starthere)
			, stophere_button(_stackbuttons.stophere)
			, stack_progress(_counters.planecounter)
			, zdevicetype(ZDeviceHelper::ZStage)
		{
		}

		BOOL CStackSettingsPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
			start_stack_button.AttachToDlgItem(GetDlgItem(IDC_STARTSTACK_BUTTON));
			startat_edit.AttachToDlgItem(GetDlgItem(IDC_STARTAT_EDIT));
			stopat_edit.AttachToDlgItem(GetDlgItem(IDC_STOPAT_EDIT));
			spacing_edit.AttachToDlgItem(GetDlgItem(IDC_SPACING_EDIT));
			starthere_button.AttachToDlgItem(GetDlgItem(IDC_STARTHERE_BUTTON));
			stophere_button.AttachToDlgItem(GetDlgItem(IDC_STOPHERE_BUTTON));
			stack_progress.AttachToDlgItem(GetDlgItem(IDC_STACK_PROGRESS));
			zstage_button.Attach(GetDlgItem(IDC_ZSTAGE_RADIO));
			fastz_button.Attach(GetDlgItem(IDC_ETL_RADIO));
			recalc_button.Attach(GetDlgItem(IDC_RECALC_BUTTON));
			lambda_static.Attach(GetDlgItem(IDC_LAMBDA_STATIC));
			
			zstage_button.SetCheck(BM_SETCHECK);
			zdevicetype = ZDeviceHelper::ZStage;
			stackparams.zdevicetype = zdevicetype;

			planes_list.Attach(GetDlgItem(IDC_SLICES_LIST));
			planes_list.InsertColumn(0, L"Slice", 0, 35);
			planes_list.InsertColumn(1, L"Z Stage", 0, 50);
			CString label(L"");
			for ( uint32_t a = 0 ; a < allareas.size() ; a++ ) {
				label.Format(L"Pockels %d", a+1);
				planes_list.InsertColumn(2+2*a, label, 0, 60);
				label.Format(L"FastZ %d", a+1);
				planes_list.InsertColumn(3+2*a, label, 0, 50);
			}

			SetMsgHandled(false);
			initialized = true;
			return 0;
		}

		LRESULT CStackSettingsPage::OnZControlRadio(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
			// Could have used DDX, but this seems easier...
			// Get the checked one and set local and GuiParameters
			if ( zstage_button.GetCheck() == BST_CHECKED )
				zdevicetype = ZDeviceHelper::ZStage;
			if ( fastz_button.GetCheck() == BST_CHECKED )
				zdevicetype = ZDeviceHelper::FastZ;
			stackparams.zdevicetype = zdevicetype;
			UpdatePlanesList();
			return 0;
		}

		LRESULT CStackSettingsPage::OnRecalcButton(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
			UpdatePlanesList();
			return 0;
		}

		void CStackSettingsPage::UpdatePlanesList() {
			planes_list.DeleteAllItems();
			std::wostringstream stream;

			for ( uint32_t p = 0 ; p < stackparams.planes.size() ; p++ ) {
				planes_list.InsertItem(p, L"Plane", NULL);
				
				// Plane no
				stream << p;
				planes_list.SetItemText(p, 0, stream.str().c_str());
				stream.str(L"");

				// Stage z Position. Always the same for all areas, either variable or fixed to current position (if etl zstacking)
				if ( zdevicetype == ZDeviceHelper::ZStage )
					stream << std::setprecision(0) << stackparams.planes[p][0].position();
				else
					stream << std::setprecision(0) << stageparams.zpos();
				planes_list.SetItemText(p, 1, stream.str().c_str());
				stream.str(L"");

				// Fast z position and pockels for each area
				for ( uint32_t a = 0 ; a < allareas.size() ; a++ ) {
					stream << std::setprecision(2) << stackparams.planes[p][a].pockels();
					planes_list.SetItemText(p, 2+2*a, stream.str().c_str());
					stream.str(L"");
					// Etl z position. Either variable or fixed to current positions (if stage z stacking)
					if ( zdevicetype == ZDeviceHelper::FastZ )
						stream << std::setprecision(0) << stackparams.planes[p][a].position();
					else
						stream << std::setprecision(0) <<  allareas[a]->Currentframe().fastz();
					planes_list.SetItemText(p, 3+2*a, stream.str().c_str());
					stream.str(L"");
				}

			}
			double l = (stackparams.Lambda(0)==0)?0:abs(1/stackparams.Lambda(0));
			stream << L"Lambda " << std::setprecision(1) << l << L" �m";
			lambda_static.SetWindowText(stream.str().c_str());
		}

		void CStackSettingsPage::SetReadOnlyWhileScanning(const bool& _ro) {
			if ( initialized ) {
				// ReadOnlyWhileScanning is true while scanning -> enabled = false
				BOOL state = !_ro;
				recalc_button.EnableWindow(state);
				fastz_button.EnableWindow(state);
				zstage_button.EnableWindow(state);
				// If we disable, the user cannot scroll through the list (and see what planes left to scan etc.) while scanning
				//planes_list.EnableWindow(state);
			}
		}

	}
}