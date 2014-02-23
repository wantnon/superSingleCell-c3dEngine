//
//  backGroundMusicCube.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-5-7.
//
//

#ifndef __HelloOpenGL__backGroundMusicCube__
#define __HelloOpenGL__backGroundMusicCube__

#include <iostream>
#include "c3dAudioCache.h"
#include "c3dModel.h"
#include "c3dActor.h"
#include "c3dFileUtils.h"
class CbackGroundMusicCube:public Cc3dActor
{
public:
    Cc3dVector4 sourcePos;
    string audioFileName;
    CbackGroundMusicCube(){
    }
    virtual~CbackGroundMusicCube(){
       
    };
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
            char buffer[1024]={0};
            //audioFileName
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%s",buffer);
            audioFileName=buffer;
            //sourcePos
            float pos[4]={0,0,0,0};
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&pos[0]);
            fscanf(fp, "%f",&pos[1]);
            fscanf(fp, "%f",&pos[2]);
            sourcePos.init(pos);
        }
        //关闭文件
        fclose(fp);
    }

};
#endif /* defined(__HelloOpenGL__backGroundMusicCube__) */
