#pragma once

#include "NoScanBasePage.h"
#include "StorageSettingsPage.h"
#include "StimulationSettingsPage.h"
#include "MovementPage.h"
#include "DAQmxPage.h"
#include "FPGAPhotonCounterPage.h"
#include "FPGADigitalDemultiplexerPage.h"
#include "FPGAAnalogIntegratorPage.h"
#include "FPGAResonanceScannerPage.h"
#include "FPGAResonanceScanner_NI5771Page.h"
#include "FPGAAnalogDemultiplexerPage.h"
#include "FPGAAnalogDemultiplexerResonancePage.h"
#include "parameters/Area.h"
#include "parameters/Devices.h"
#include "parameters/Storage.h"
#include "parameters/Stimulation.h"


namespace scope {
	namespace gui {

		/** Settings sheet that has settings pages for different parameter sets */
		class CScanSettingsSheet
			: public CPropertySheetImpl<CScanSettingsSheet> {

		protected:
			std::vector<std::unique_ptr<parameters::BaseArea>>& allareas;
			
			std::vector<FPUButtons>& fpubuttons;
			
			/** one imaging settings page for each area. */
			std::vector<std::unique_ptr<CNoScanBasePage>> scanpages;
			

			/** the storage settings page */
			CStorageSettingsPage storagesettingspage;

			/** the stimulation settings page */
			CStimulationSettingsPage stimulationsettingspage;
			
			/** the settings page for all stages */
			CMovementPage movementpage;

			/** a settings page for DAQmx/FPGA information */
			config::InputGuiPageType inputsinfospage;

		public:
			
			CScanSettingsSheet(
				std::vector<std::unique_ptr<parameters::BaseArea>>& _allareas
				, std::vector<FPUButtons>& _fpubuttons
				, const double& _masterfovsizex
				, const double& _masterfovsizey
				, parameters::Storage& _storageparams
				, parameters::Stimulation& _stimulationparams
				, config::XYZStageParametersType& _stageparams
				, ZeroButtons& _zerobuttons
			);

			BEGIN_MSG_MAP(CScanSettingsSheet)	  
				NOTIFY_CODE_HANDLER(TCN_SELCHANGE, OnSelChange)
				CHAIN_MSG_MAP(CPropertySheetImpl<CScanSettingsSheet>)
				REFLECT_NOTIFICATIONS()
			END_MSG_MAP()

			/** @name Called via Win32 messages
			* Override Create and use Callback to switch to child window style during creation
			* since Property sheets are popups as standard (Windows not WTL I think)
			* @{ */

			/** Adds all the settingspages */
			HWND Create(const HWND hWndParent, const int nStartPage, const CRect & rc);

			/** We need this for whatever (see some WTL dialog tutorial)... */
			static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam);

			LRESULT OnSelChange(WPARAM wParam, LPNMHDR pnmHdr, BOOL & bHandled);
			/** @} */

			/** @name Called from TheScope
			* @{ */
			/** Change the imaging settings page according to the choosen scan mode */
			void ChangeScanmode(const uint32_t& _area, const ScannerVectorType& _type);
			
			/** disables controls other than connected buttons and values when acquisition is running */
			void SetReadOnlyWhileScanning(const bool& _ro);
			/** @} */
		};

	}
}