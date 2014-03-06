//
//  c3dDeviceAndOSInfoPlatformDepend.h
//  HelloOpenGL
//
//  Created by ck02-084 on 14-3-6.
//
//

#ifndef __HelloOpenGL__c3dDeviceAndOSInfoPlatformDepend__
#define __HelloOpenGL__c3dDeviceAndOSInfoPlatformDepend__

#include <iostream>
using namespace std;
#import <mach/mach.h>
#import <mach/mach_host.h>
#include "c3dVector.h"
Cc3dVector2 getScreenSize_iOS();
Cc3dVector2 getResolutionSize_iOS();
void getMemStatistics_iOS(int&memUsed,int&memFree,int&memTotal);
#endif /* defined(__HelloOpenGL__c3dDeviceAndOSInfoPlatformDepend__) */
