//
//  shadowMapInfo.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-2-5.
//
//

#ifndef __HelloOpenGL__shadowMapInfo__
#define __HelloOpenGL__shadowMapInfo__

#include <iostream>
#include <string>
using namespace std;
#include "c3dFileUtils.h"
class CshadowMapInfo
{
public:
    float lightCamAngle;//光源视角相机张角
    CshadowMapInfo(){
        lightCamAngle=0;
    }
    void loadConfig(const string&fileNameWithExt){
        //------------------获得绝对路径
        string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(fileNameWithExt);
        //打开文件
        FILE  *fp=NULL;
        fp=fopen(pathName.c_str(),"r");
        if(fp==NULL)
        {
            cout<<"Open "<<pathName<<" failed!"<<endl;
            exit(0);
        }
        //------------------提取文件信息，并建立列模型列表
        {
            //提取lightCamAngle
            fscanf(fp, "%f",&lightCamAngle);
        }
        //关闭文件
        fclose(fp);
    }

};
#endif /* defined(__HelloOpenGL__shadowMapInfo__) */
