//
//  myFunc.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-19.
//
//

#ifndef __HelloOpenGL__myFunc__
#define __HelloOpenGL__myFunc__

#include <iostream>
#include <string>
using namespace std;
#include "c3dMeshData.h"
#include "c3dRange.h"
#include "c3dDeviceAndOSInfo.h"
#include "geoMath.h"
#include "c3dNode.h"
#include "c3dProgram.h"
#include "c3dCamera.h"
string getResolutionStr();
//cube的纹理映射模式
#define cubUVMode_FITFACE 0 //适应各面
#define cubUVMode_UNIFORM 1 //均匀映射
Cc3dMeshData*genSampleCubeWithCode(float cx,float cy,float cz,float dx,float dy,float dz,
                                   float k_s=1,//s的放缩系数
                                   float k_t=1,//t的放缩系数
                                   int cubUVMode=cubUVMode_FITFACE);//用代码生成立方体
Cc3dMeshData genQuad(float p[4][3]);
Cc3dRange rangeAdd(const Cc3dRange&range1,const Cc3dRange&range2);
void reCalNorms_precise_normByFace(Cc3dMeshData*meshData);
void reCalNorms_precise_normByVertex(Cc3dMeshData*meshData);
Cc3dVector4 softMov(const Cc3dVector4&originPos,const Cc3dVector4&tgPos,const float percentage);
float softMov(const float origin,const float tg,const float percentage);
#endif /* defined(__HelloOpenGL__myFunc__) */
