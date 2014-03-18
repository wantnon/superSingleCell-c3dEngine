//
//  c3dDeviceAndOSInfoPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-6.
//
//

#include "c3dDeviceAndOSInfoPlatformDepend.h"
#include "c3dAssert.h"
Cc3dVector2 getScreenSize_plat(){

	return Cc3dVector2(640*0.5,1136*0.5);
}
Cc3dVector2 getResolutionSize_plat(){

	//由于iOS上凡640x1136分辨率者只有高清，所以这里只能假装是高清以便能够使用资源
	return Cc3dVector2(640,1136);
}
void getMemStatistics_plat(int&memUsed,int&memFree,int&memTotal){//获得内存统计数据
 
	//no implemented yet

	C3DASSERT(false);
}