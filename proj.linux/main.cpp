#include "../Classes/AppDelegate.h"
#include "cocos2d.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

USING_NS_CC;

int main(int argc, char **argv)
{
    // create the application instance
    AppDelegate app;

    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    //eglView->setFrameSize(240, 320); // galaxy young
    eglView->setFrameSize(480, 800); // nexus
    //eglView->setFrameSize(320, 480); // iphone
    //eglView->setFrameSize(640, 1136); // iphone 5
    //eglView->setFrameSize(640, 960); // iphone 4
    //eglView->setFrameSize(800, 1280); // nexus 7
    //eglView->setFrameSize(1200, 1920); // nexus 7 II
    //eglView->setFrameSize(1600, 2560); // nexus 10
    //eglView->setFrameSize(768, 1024); // ipad ipad mini
    //eglView->setFrameSize(1536, 2048); // ipad 3

    return CCApplication::sharedApplication()->run();
}
