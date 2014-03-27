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
#include "c3dMesh.h"
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
        Cc3dMesh*mesh=new Cc3dMesh();
        mesh->autorelease();
        mesh->init();
        addSubMesh(mesh);
        Cc3dSubMesh*submesh=new Cc3dSubMesh();
        submesh->init();
        submesh->autorelease();
        submesh->setTexture(texture);
        submesh->getIndexVBO()->genBuffers();
        
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
        submesh->getSubMeshData()->vlist.resize(4);
        memcpy(&submesh->getSubMeshData()->vlist[0], vertexArray, sizeof(vertexArray));
        submesh->addIDtri(0, 1, 2);
        submesh->addIDtri(0, 2, 3);
        
        this->getModel()->getMeshByIndex(0)->addSubMesh(submesh);
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
            assert(false);
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
