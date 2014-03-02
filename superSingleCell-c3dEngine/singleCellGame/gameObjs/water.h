//
//  water.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-19.
//
//

#ifndef __HelloOpenGL__water__
#define __HelloOpenGL__water__

#include <iostream>
using namespace std;
#import <OpenAL/al.h>
#include "c3dModel.h"
#include "c3dTimeCounter.h"
#include "molecule.h"
#include "c3dAudioCache.h"
#include "c3dActor.h"
#include "c3dFileUtils.h"
//水面可以平移，但不支持放缩
//只可以水平平移，不允许竖直方向上平移，因为水面高度由Horizon决定
class Cwater:public Cc3dActor
{
protected:
    Csprite* m_reflectSquare;//反射矩形
    Csprite* m_arabesquiticSquare;//花纹矩形
    Cmolecule* m_centerSquare;//中心距形
public:
    Cc3dALSource *pALSource_water_gulu;
	float Horizon;//高度
    float flow_s,flow_t;//实现流动贴图
//    float maxAlpha;//reflectSquare的最大
    
    

    bool isExist;//是否存在
    float sRepeat_reflect,tRepeat_reflect;//反射贴图s,t的重复次数，可以是分数
    float hurtValue;//伤害值
	Cwater(){
        pALSource_water_gulu=NULL;
        flow_s=flow_t=0;
        hurtValue=0;//默认无伤害
        sRepeat_reflect=tRepeat_reflect=1;
        isExist=true;
    //    maxAlpha=1.0;
        m_reflectSquare=NULL;
        m_centerSquare=NULL;
        m_arabesquiticSquare=NULL;
		Horizon=-c3d_INF;
        
	}
	virtual ~Cwater(){

    };
    bool init(){
        Cc3dModel*model=new Cc3dModel();
        model->init();
        model->autorelease();
        addModel(model);
        return true;
    }
    Cc3dModel*getModel(){
        assert(getModelCount()==1);
        return m_modelList[0];
    }
    void setReflectSquare(Csprite*reflectSquare){
        assert(reflectSquare);
        if(m_reflectSquare==NULL){
            m_reflectSquare=reflectSquare;
            m_reflectSquare->retain();
        }else{
            m_reflectSquare->release();
            m_reflectSquare=reflectSquare;
            m_reflectSquare->retain();
        }
        addChild(m_reflectSquare);
        
    }
    Csprite*getReflectSquare()const{return m_reflectSquare;}
    void setArabesquiticSquare(Csprite*arabesquiticSquare){
        assert(arabesquiticSquare);
        if(m_arabesquiticSquare==NULL){
            m_arabesquiticSquare=arabesquiticSquare;
            m_arabesquiticSquare->retain();
        }else{
            m_arabesquiticSquare->release();
            m_arabesquiticSquare=arabesquiticSquare;
            m_arabesquiticSquare->retain();
        }
        addChild(m_arabesquiticSquare);
    }
    Csprite*getArabesquiticSquare()const{return m_arabesquiticSquare;}
    void setCenterSquare(Cmolecule*centerSquare){
        assert(centerSquare);
        if(m_centerSquare==NULL){
            m_centerSquare=centerSquare;
            m_centerSquare->retain();
        }else{
            m_centerSquare->release();
            m_centerSquare=centerSquare;
            m_centerSquare->retain();
        }
        addChild(m_centerSquare);
        
    }
    Cmolecule*getCenterSquare()const{return m_centerSquare;}
    void set_isExist(bool _isExist){
        isExist=_isExist;
        
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
            //提取sRepeat_reflect和tRepeat_reflect
            fscanf(fp, "%f",&sRepeat_reflect);
            fscanf(fp, "%f",&tRepeat_reflect);
            //提取水对主角的伤害
            fscanf(fp,"%f",&hurtValue);
            //提取Horizon
            fscanf(fp, "%f",&Horizon);
            
        }
        //关闭文件
        fclose(fp);
    }

    float getHorizon()const {
        return Horizon;
    }
    void wave(const Cc3dVector4&pos){
        m_centerSquare->useNextFrameTex_notRewind();
        //获得已播放的百分率
        int nFrame=m_centerSquare->getFrameTexList().size();
        int iCurFrame=m_centerSquare->i_curFrameTex;
        const float percentage=0.3;
        //声音
        //如果此时恰为percentage位置的帧且声音已播完，则播放，否则不播放
        if(iCurFrame==(int)(nFrame*percentage)){
            pALSource_water_gulu->updatePosition(pos);
            pALSource_water_gulu->playIfNotPlaying();
        }

        
    }
    void rewind(){
        
        m_centerSquare->useTex(0);
    
    }
    void push(){
        rewind();
              
    }
    void uvFlow(float dflow_s,float dflow_t){
        flow_s+=dflow_s;
        if(flow_s>1.0)flow_s=0;//由于shader中纹理坐标变量的位数较少，所以不能让flow_s和flow_t太大，否则越界后贴图发生跳动
        flow_t+=dflow_t;
        if(flow_t>1.0)flow_t=0;
    
    }
       
    void submitStream_centerSquare(){
        m_centerSquare->getModel()->getMeshByIndex(0)->getIndexVBO()->submitVertex(m_centerSquare->getModel()->getMeshByIndex(0)->getMeshData()->vlist,GL_STREAM_DRAW);
        m_centerSquare->getModel()->getMeshByIndex(0)->getIndexVBO()->submitIndex(m_centerSquare->getModel()->getMeshByIndex(0)->getMeshData()->IDtriList,GL_STREAM_DRAW);
        
    }

    void update_centerSquare(const Cc3dVector4&pos){
        const float h=Horizon;
        Cc3dMesh&mesh=*m_centerSquare->getModel()->getMeshByIndex(0);
        //   3 --- 2
        //     |       |
        //   0 ---1
        float Dx=m_centerSquare->getRx();//x轴边心距
        float Dz=m_centerSquare->getRy();//z轴边心距
        float vertexArray[4][3+2+3+2]={
            {pos.x()-Dx,h,pos.z()+Dz,0,1,0,1,0,0,0},
            {pos.x()+Dx,h,pos.z()+Dz,1,1,0,1,0,0,0},
            {pos.x()+Dx,h,pos.z()-Dz,1,0,0,1,0,0,0},
            {pos.x()-Dx,h,pos.z()-Dz,0,0,0,1,0,0,0}
        };
        mesh.clearMeshData();
        mesh.getMeshData()->vlist.resize(4);
        memcpy(&mesh.getMeshData()->vlist[0], vertexArray, sizeof(vertexArray));
        mesh.addIDtri(0, 1, 2);
        mesh.addIDtri(0, 2, 3);
    }
    void submitStream_arabesquiticSquare(){
        m_arabesquiticSquare->getModel()->getMeshByIndex(0)->getIndexVBO()->submitVertex(m_arabesquiticSquare->getModel()->getMeshByIndex(0)->getMeshData()->vlist,GL_STREAM_DRAW);
        m_arabesquiticSquare->getModel()->getMeshByIndex(0)->getIndexVBO()->submitIndex(m_arabesquiticSquare->getModel()->getMeshByIndex(0)->getMeshData()->IDtriList,GL_STREAM_DRAW);
        
    }
    
    void update_arabesquiticSquare(const Cc3dVector4&pos,
                              float umin=0,float umax=1,float vmin=0,float vmax=1){
        const float h=Horizon;
        Cc3dMesh&mesh=*m_arabesquiticSquare->getModel()->getMeshByIndex(0);
        //   3 --- 2
        //     |       |
        //   0 ---1
        float Dx=m_arabesquiticSquare->getRx();//x轴边心距
        float Dz=m_arabesquiticSquare->getRy();//z轴边心距
        float vertexArray[4][3+2+3+2]={
            {-Dx,h,Dz,umin,vmax,0,1,0,0,0},
            {+Dx,h,Dz,umax,vmax,0,1,0,0,0},
            {+Dx,h,-Dz,umax,vmin,0,1,0,0,0},
            {-Dx,h,-Dz,umin,vmin,0,1,0,0,0}
        };
        mesh.clearMeshData();
        mesh.getMeshData()->vlist.resize(4);
        memcpy(&mesh.getMeshData()->vlist[0], vertexArray, sizeof(vertexArray));
        mesh.addIDtri(0, 1, 2);
        mesh.addIDtri(0, 2, 3);
    }
    void submitStream_reflectSquare(){
        m_reflectSquare->getModel()->getMeshByIndex(0)->getIndexVBO()->submitVertex(m_reflectSquare->getModel()->getMeshByIndex(0)->getMeshData()->vlist,GL_STREAM_DRAW);
        m_reflectSquare->getModel()->getMeshByIndex(0)->getIndexVBO()->submitIndex(m_reflectSquare->getModel()->getMeshByIndex(0)->getMeshData()->IDtriList,GL_STREAM_DRAW);
        
    }
    
    void update_reflectSquare(const Cc3dVector4&cellpos){
        const float h=Horizon;
        Cc3dMesh&mesh=*m_reflectSquare->getModel()->getMeshByIndex(0);
        Cc3dVector4 waterpos=this->getTransform().getPos();
        Cc3dVector4 waterToCell=cellpos-waterpos;
        float offsetx=-waterToCell.x();
        float offsetz=-waterToCell.z();
        float Dx=m_reflectSquare->getRx();//x轴边心距
        float Dz=m_reflectSquare->getRy();//z轴边心距
        float offsetxDivDoubleDx=offsetx/(Dx*2);//如果无重复映射，则空间位移1对应的贴图位移为offsetxDivDoubleDx
        float offsetzDivDoubleDz=offsetz/(Dz*2);
        float offset_u=flow_s*sRepeat_reflect;
        float offset_v=flow_t*tRepeat_reflect;
        float offset_u2=offsetxDivDoubleDx;//alpha贴图与主角同步移动
        float offset_v2=offsetzDivDoubleDz;
        //   3 --- 2
        //     |       |
        //   0 ---1
        float vertexArray[4][3+2+3+2]={
            {-Dx,h,+Dz,0+offset_u,tRepeat_reflect+offset_v,0,1,0,0+offset_u2,1+offset_v2},
            {+Dx,h,+Dz,sRepeat_reflect+offset_u,tRepeat_reflect+offset_v,0,1,0,1+offset_u2,1+offset_v2},
            {+Dx,h,-Dz,sRepeat_reflect+offset_u,0+offset_v,0,1,0,1+offset_u2,0+offset_v2},
            {-Dx,h,-Dz,0+offset_u,0+offset_v,0,1,0,0+offset_u2,0+offset_v2}
        };
        mesh.clearMeshData();
        mesh.getMeshData()->vlist.resize(4);
        memcpy(&mesh.getMeshData()->vlist[0], vertexArray, sizeof(vertexArray));
        mesh.addIDtri(0, 1, 2);
        mesh.addIDtri(0, 2, 3);
    }
    


};
#endif /* defined(__HelloOpenGL__water__) */
