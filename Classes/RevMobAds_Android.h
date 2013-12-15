#ifndef REVMOB_REVMOBADS_ANDROID_H
#define REVMOB_REVMOBADS_ANDROID_H

#include "RevMob.h"

namespace revmob {

class RevMobAds_Android: public RevMob {
public:
	void StartSession(const char *pAppID);

	void SetTestingMode (revmob::TestingMode pTestingMode = kTestingModeDisabled);
	TestingMode GetTestingMode();

	void PrintEnvironmentInformation();

	void ShowFullscreen();
	void ShowPopup();
	void ShowBanner();
	void HideBanner();
	void OpenLink();

};

} /* namespace revmob */
#endif /* REVMOB_REVMOBADS_ANDROID_H */
