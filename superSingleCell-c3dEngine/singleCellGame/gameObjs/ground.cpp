//
//  ground.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-4.
//
//

#include "ground.h"
#include "c3dFileUtils.h"
void Cground::loadConfig(const string&fileNameWithExt){
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
    //------------------提取文件信息
    {
        char buffer[1024]={0};
        //提取地形高度缩放因子
        fscanf(fp, "%s",buffer);
        fscanf(fp, "%f",&m_heightScale);
        
    }
    //关闭文件
    fclose(fp);
}
void Cground::loadConfig_texBlend(const string&fileNameWithExt){
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
    //------------------提取文件信息
    {
        char buffer[1024]={0};
        //提取纹理缩放系数
        fscanf(fp, "%s",buffer);
        fscanf(fp, "%f",&m_uvScale);
        
    }
    //关闭文件
    fclose(fp);
}