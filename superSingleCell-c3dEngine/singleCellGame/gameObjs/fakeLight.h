//
//  fakeLight.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-1-4.
//
//

#ifndef __HelloOpenGL__fakeLight__
#define __HelloOpenGL__fakeLight__

#include <iostream>
#include "sprite.h"
#include "c3dFileUtils.h"
class CfakeLight:public Csprite
{
public:
    CfakeLight(){
    }
    virtual~CfakeLight(){};

};
class CfakeLightSet:public CspriteSet
{
public:
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
            //提取项目数
            int n;
            fscanf(fp, "%d",&n);
            //提取项目
            for(int i=0;i<n;i++){
                CfakeLight*pfakeLight=new CfakeLight();
                pfakeLight->autorelease();
                //----pos
                float pos[4]={0,0,0,1};
                fscanf(fp,"%s",buffer);
                if(buffer[0]=='#'){
                    cout<<"error:提前遇到#!"<<endl;
                    exit(0);
                }

                fscanf(fp, "%f",&pos[0]);
                fscanf(fp, "%f",&pos[1]);
                fscanf(fp, "%f",&pos[2]);
                pfakeLight->setPos(pos);
                //----r
                float r;
                fscanf(fp, "%f",&r);
                pfakeLight->init(r, r);
                pfakeLight->setRc(r);
                //将pfakeLight加入到pModelBaseList
                addActor(pfakeLight);
            }
            fscanf(fp, "%s",buffer);
            if(buffer[0]!='#'){
                cout<<"error:缺少#或实际条目数比n值大!"<<endl;
                exit(0);
            }

        }
        //关闭文件
        fclose(fp);
    }

    
    
};
#endif /* defined(__HelloOpenGL__fakeLight__) */
