//
//  c3dDeviceAndOSInfo.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-4-18.
//
//

#ifndef __HelloOpenGL__c3dDeviceAndOSInfo__
#define __HelloOpenGL__c3dDeviceAndOSInfo__

#include <iostream>
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32) 
	
#else
    #include <mach/mach.h>
	#include <mach/mach_host.h>
#endif

#include "c3dVector.h"
#include "c3dRect.h"
#include "c3dDeviceAndOSInfoPlatformDepend.h"
class Cc3dDeviceAndOSInfo
{
public:
    Cc3dDeviceAndOSInfo(){
    }
    virtual ~Cc3dDeviceAndOSInfo(){}
    static Cc3dDeviceAndOSInfo*sharedDeviceAndOSInfo();
    Cc3dVector2 getScreenSize();
    Cc3dVector2 getResolutionSize();
    void getMemStatistics(int&memUsed,int&memFree,int&memTotal);
    float getPixelDensity(){
        assert(getResolutionSize().x()/getScreenSize().x()==getResolutionSize().y()/getScreenSize().y());
        return getResolutionSize().x()/getScreenSize().x();
    }
    int getSupportedMaxTextureSize();
    Cc3dRect getScreenRect(){
        Cc3dVector2 screenSize=this->getScreenSize();
        Cc3dRect screenRect(0, 0, screenSize.x(), screenSize.y());
        return screenRect;
    }
};
#endif /* defined(__HelloOpenGL__c3dDeviceAndOSInfo__) */
