#ifndef REVMOB_REVMOB_H
#define REVMOB_REVMOB_H

namespace revmob {

    typedef enum TestingMode {
        kTestingModeUndefined,
        kTestingModeDisabled,
        kTestingModeWithAds,
        kTestingModeWithoutAds
    } TestingMode;

    class RevMob {
    public:

        static RevMob *SharedInstance();

        virtual void StartSession(const char *pAppID){};

        virtual void SetTestingMode (revmob::TestingMode pTestingMode = kTestingModeDisabled){};
        virtual TestingMode GetTestingMode(){ return kTestingModeUndefined; };

        virtual void PrintEnvironmentInformation(){};

        virtual void ShowFullscreen(){};
        virtual void ShowPopup(){};
        virtual void ShowBanner(){};
        virtual void HideBanner(){};
        virtual void OpenLink(){};

    private:
        RevMob(RevMob const&);
        RevMob& operator=(RevMob const&);

        static RevMob *sSharedInstance;

    protected:
        RevMob();
        virtual ~RevMob();


    };
    
    
} // namespace revmob

#endif /* REVMOB_REVMOBADS_H */
