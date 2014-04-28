//
//  c3dTransform.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#ifndef __HelloOpenGL__c3dTransform__
#define __HelloOpenGL__c3dTransform__

#include <iostream>
#include <math.h>
#include "c3dMatrix.h"
#include "c3dVector.h"
#include "c3dMath.h"
#include "c3dGLMath.h"
class Cc3dTransform
{
protected:
    Cc3dMatrix4 m_RTmat;//旋转平移矩阵--abc
    float m_scaleX,m_scaleY,m_scaleZ;
public:
    Cc3dTransform(){
        m_RTmat=unitMat();
        m_scaleX=m_scaleY=m_scaleZ=1;
    }
    virtual~Cc3dTransform(){}
    Cc3dMatrix4 getRTmat()const{return m_RTmat;}
    void setRmat(const Cc3dMatrix4&Rmat);
    void setRTmat(const Cc3dMatrix4&mat);
    Cc3dMatrix4*getRTmatPointer(){return &m_RTmat;}
    float getScaleX()const{return m_scaleX;}
    float getScaleY()const{return m_scaleY;}
    float getScaleZ()const{return m_scaleZ;}
    void setScale(float scaleX,float scaleY,float scaleZ);
    void scale(float scaleX,float scaleY,float scaleZ);
    Cc3dMatrix4 getRTSmat()const;
    void rotate(float nx,float ny,float nz,float cosa,float sina);
    void rotateRelativeToFather(float nx,float ny,float nz,float cosa,float sina);
    void move(float dx,float dy,float dz);
    void moveX(float dx);
    void moveY(float dy);
    void moveZ(float dz);
    void moveRelativeToFather(float dx,float dy,float dz);
    Cc3dVector4 getPos()const;
    float getPosY()const;
    void setPos(float x,float y,float z);
    void setPos(const Cc3dVector4&pos);
    void setRot(float nx,float ny,float nz,float cosa,float sina);
    void setPosY(float y);
    Cc3dVector4 convertToWorldSpace(const Cc3dVector4&pos_local)const;

};

#endif /* defined(__HelloOpenGL__c3dTransform__) */
