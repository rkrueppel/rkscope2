#pragma once

#include "MovementSettingsSheet.h"
#include "resource.h"
#include "parameters/Area.h"
#include "parameters/Devices.h"
#include "TheScopeButtons.h"


namespace scope {
	namespace gui {

/** Property page for stage settings */
class CMovementPage :
	public CPropertyPageImpl<CMovementPage> {

protected:
	/** the setting sheets that contains the real stuff */
	CMovementSettingsSheet settings_sheet;

public:
	enum { IDD = IDD_MOVEMENT_PROPPAGE };
	
	CMovementPage(std::vector<std::unique_ptr<parameters::BaseArea>>& _allareas
		, std::vector<FPUButtons>& _fpubuttons
		, const double& _masterfovsizex
		, const double& _masterfovsizey
		, config::XYZStageParametersType& _stageparams
		, ZeroButtons& _zerobuttons);

	BEGIN_MSG_MAP_EX(CMovementPage)	
		MSG_WM_INITDIALOG(OnInitDialog);
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	/** @name Called via Win32 messages
	* @{ */
	/** Replace the placeholder with the CMovementSettingsSheet */
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	/** @} */
};

}
}
