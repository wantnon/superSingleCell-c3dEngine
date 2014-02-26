//
//  c3dCamera.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-8.
//
//

#ifndef __HelloOpenGL__c3dCamera__
#define __HelloOpenGL__c3dCamera__

#include <iostream>
#include "c3dPlane.h"
#include "c3dObject.h"
#include "c3dGLMath.h"
#include "c3dRange.h"
class Cc3dFrustum
{
protected:
    Cc3dPlane planeList[6];//视锥表示为六个平面（第一个为近平面，最后一个为远平面，中间四个为侧面）
public:
    bool ballIsPotentiallyVisible(const Cc3dVector4&c,float R);
    void updateFrustum(const Cc3dMatrix4&projectionMatrix,const Cc3dMatrix4&viewMatrix/*,const  Cc3dRect&viewport*/);
    const Cc3dPlane&getPlaneByIndex(int index)const{
        assert(index>=0&&index<6);
        return planeList[index];
    }
  
};

class Cc3dCamera:public Cc3dObject
{

public:
    Cc3dCamera(){
        m_eyePos.init(0,0,0,1);
        m_center.init(0,0,-1,1);
        m_up.init(0,1,0,0);
        
        m_fovy=60;
        m_aspect=320.0/568.0;
        m_zNear=0.5;
        m_zFar=1500;
        
        const float w=320;
        const float h=568;
        m_range.init(-w/2, -w/2+w, -h/2, -h/2+h, -1024, 1024);
        m_projectionMode=ec3dPerspectiveMode;
    }
    ~Cc3dCamera(){};
    Cc3dFrustum getFrustum()const{return m_frustum;}
    void updateFrustum();
    void setEyePos(const Cc3dVector4&eyePos){m_eyePos=eyePos;};
    void setCenter(const Cc3dVector4&center){m_center=center;};
    void setUp(const Cc3dVector4&up){m_up=up;};
    void setFovy(float fovy){m_fovy=fovy;}
    void setAspect(float aspect){m_aspect=aspect;}
    void setzNear(float zNear){m_zNear=zNear;}
    void setzFar(float zFar){m_zFar=zFar;}
    float getFovy()const {return m_fovy;}
    float getAspect()const {return m_aspect;}
    float getzNear()const {return m_zNear;}
    float getzFar()const {return m_zFar;}
    void setRange(const Cc3dRange&range){m_range=range;}
    Cc3dRange getRange()const{return m_range;}
    Cc3dVector4 getEyePos()const{return m_eyePos;};
    Cc3dVector4 getCenter()const{return m_center;};
    Cc3dVector4 getUp()const{return m_up;};//note: it is not the corrected up, just the up we previously set
    Cc3dMatrix4 calculateViewMat()const;
    Cc3dMatrix4 calculateViewMatInverse();
    Cc3dMatrix4 calculateProjectionMat()const;
    Ec3dProjectionMode getProjectionMode(){return m_projectionMode;}
    void setProjectionMode(Ec3dProjectionMode projectionMode){m_projectionMode=projectionMode;}
    void printProjectionMode();
protected:
    //----projection
    //projection mode type
    Ec3dProjectionMode m_projectionMode;
    //perspective projection mode params
    float m_fovy;
    float m_aspect;
    float m_zNear;
    float m_zFar;
    //Ortho projection mode params
    Cc3dRange m_range;
    //----model view
    //m_eyePos, m_up, m_center are all world space value
    Cc3dVector4 m_eyePos;
    Cc3dVector4 m_up;
    Cc3dVector4 m_center;
protected:
    Cc3dFrustum m_frustum;//视截体
    
    
};
#endif /* defined(__HelloOpenGL__c3dCamera__) */
