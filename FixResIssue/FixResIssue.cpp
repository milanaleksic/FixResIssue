#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <string>

using namespace std;

#define TEMP_WIDTH 1280
#define TEMP_HEIGHT 800

#define SLEEP_INTERVAL 4000


DEVMODEW GetCurrentResolutionInformation() {
	DEVMODEW devMode;
	cout << "Reading current resolution information..." << endl;
	devMode.dmSize = sizeof(DEVMODEW);

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);

	cout << "Bits per pixel: " << devMode.dmBitsPerPel << endl;
	cout << "Current resolution: " << devMode.dmPelsWidth << "x" << devMode.dmPelsHeight<< endl << endl;
	return devMode;
}

LONG TestIfResolutionChangeToTempIsPossible(DEVMODEW& devMode) {
	devMode.dmPelsWidth = TEMP_WIDTH;
	devMode.dmPelsHeight = TEMP_HEIGHT;
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

	cout << "Changing resolution to: " << TEMP_WIDTH << "x" << TEMP_HEIGHT << endl << endl;
	LONG res = ChangeDisplaySettings(&devMode, CDS_TEST);
	string testRes;
	
	switch (res) {
		case DISP_CHANGE_SUCCESSFUL :
			testRes = "The settings change was successful";
			break;
		case DISP_CHANGE_BADDUALVIEW :
			testRes = "The settings change was unsuccessful because system is DualView capable";
			break;
		case DISP_CHANGE_BADFLAGS :
			testRes = "An invalid set of flags was passed in";
			break;
		case DISP_CHANGE_BADMODE :
			testRes = "The graphics mode is not supported";
			break;
		case DISP_CHANGE_BADPARAM :
			testRes = "An invalid parameter was passed in. This can include an invalid flag or combination of flags";
			break;
		case DISP_CHANGE_FAILED :
			testRes = "The display driver failed the specified graphics mode";
			break;
		case DISP_CHANGE_NOTUPDATED :
			testRes = "Unable to write settings to the registry";
			break;
		case DISP_CHANGE_RESTART :
			testRes = "The computer must be restarted in order for the graphics mode to work";
			break;
		default :
			testRes = "Unknown response";
			break;
	}
	cout << "Testing new resolution returned: " << testRes << " (" << res << ")" << endl << endl;
	return res;
}

void SetResolutionToTemporaryOne(DEVMODEW& devMode) {
	DWORD dFlags = CDS_TEST;
	devMode.dmPelsWidth = TEMP_WIDTH;
	devMode.dmPelsHeight = TEMP_HEIGHT;
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	ChangeDisplaySettings(&devMode, 0);
}

void ReturnToPreviousResolution(DEVMODEW& devMode, LONG origWidth, LONG origHeight) {
	cout << "Waiting " << SLEEP_INTERVAL << "ms before returning to default resolution" << endl << endl;
	SleepEx(SLEEP_INTERVAL, FALSE);

	devMode.dmPelsWidth = origWidth;
	devMode.dmPelsHeight = origHeight;

	ChangeDisplaySettings(&devMode, 0);

	SleepEx(SLEEP_INTERVAL, FALSE);
}

int _tmain(int argc, _TCHAR* argv[])
{
	DEVMODEW devMode = GetCurrentResolutionInformation();
	DWORD origWidth = devMode.dmPelsWidth;
	DWORD origHeight = devMode.dmPelsHeight;
	
	LONG res = TestIfResolutionChangeToTempIsPossible(devMode);
	
	if (res != DISP_CHANGE_SUCCESSFUL) {
		cerr << "Giving up from changing resolution!";
		SleepEx(SLEEP_INTERVAL, FALSE);
		exit(1);
	}

	SetResolutionToTemporaryOne(devMode);
	ReturnToPreviousResolution(devMode, origWidth, origHeight);
	
	SleepEx(SLEEP_INTERVAL, FALSE);
	return 0;
}

