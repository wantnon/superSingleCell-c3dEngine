//
//  seabed.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-4.
//
//

#ifndef __HelloOpenGL__seabed__
#define __HelloOpenGL__seabed__

#include <iostream>
using namespace std;
#include "c3dModel.h"
#include "c3dActor.h"
#include "c3dFileUtils.h"
class Cseabed:public Cc3dActor
{
protected:
    float m_height;
public:
    Cseabed(){
        m_height=-c3d_INF;
    }
    virtual~Cseabed(){};
    bool init(float r,Cc3dTexture*texture){
        Cc3dModel*model=new Cc3dModel();
        model->autorelease();
        model->init();
        addModel(model);
        Cc3dMesh*mesh=new Cc3dMesh();
        mesh->init();
        mesh->autorelease();
        mesh->setTexture(texture);
        mesh->getIndexVBO()->genBuffers();
        
        const float h=m_height;
        const float d=r;
        //v2(-d,h,-d)  v1(d,h,-d)
        //     ---------
        //     |                    |
        //     |                    |
        //     ---------
        //v3(-d,h,d)   v0(d,h,d)
        float vertexArray[4][3+2+3+2]={
            {d,h,d,1,0,0,1,0,0,0},
            {d,h,-d,1,1,0,1,0,0,0},
            {-d,h,-d,0,1,0,1,0,0,0},
            {-d,h,d,0,0,0,1,0,0,0}
        };
        mesh->getMeshData()->vlist.resize(4);
        memcpy(&mesh->getMeshData()->vlist[0], vertexArray, sizeof(vertexArray));
        mesh->addIDtri(0, 1, 2);
        mesh->addIDtri(0, 2, 3);
        
        this->getModelByIndex(0)->addMesh(mesh);
        return true;
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
        //------------------提取文件信息
        {
            char buffer[1024]={0};
            //提取seabed高度
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&m_height);
            
        }
        //关闭文件
        fclose(fp);
    }

    
};
#endif /* defined(__HelloOpenGL__seabed__) */
