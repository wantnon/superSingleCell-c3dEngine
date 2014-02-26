//
//  c3dCamera.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-8.
//
//
#include <math.h>
#include "c3dCamera.h"
#include "c3dGeoMath.h"
//--------frustum
bool Cc3dFrustum::ballIsPotentiallyVisible(const Cc3dVector4&c,float R){//判断球体(c,R)是否与视锥相交

    for(int i=0;i<6;i++){
        float PND=directedDistanceFromPointToPlane(planeList[i], c);
        if(PND<-R){//只有一个平面，球体位于其背面且不与之相交，就是不可见
            return false;
        }
    }
    return true;
    
}
void Cc3dFrustum::updateFrustum(const Cc3dMatrix4&projectionMatrix,const Cc3dMatrix4&viewMatrix)
//对透视投影模式和正交投影模式均适用
{
    //由于显然视截体与viewport无关，而我们这里计算视截体的方法又要用到viewport，
    //那么意味着计算过程中viewport会被抵消掉，所以viewport的值可随便取。为了简单，我们取viewport=Cc3dRect(0,1,0,1)即可。
    Cc3dRect viewport(0,1,0,1);
    //
    float left=viewport.getMinX();
    float down=viewport.getMinY();
    float right=left+viewport.getWidth();
    float up=down+viewport.getHeight();
    //近平面
    //   p3(left,up,0)------p2(right,up,0)
    //      |                   |
    //      |                   |
    //   p0(left,down,0)----p1(right,down,0)
    //远平面
    //   q3(left,up,1)------q2(right,up,1)
    //      |                   |
    //      |                   |
    //   q0(left,down,1)----q1(right,down,1)
    Cc3dVector4 p[4];//近平面四个顶点的视口空间坐标
    p[0].init(left, down, 0, 1);
    p[1].init(right,down,0,1);
    p[2].init(right,up,0,1);
    p[3].init(left,up,0,1);
    Cc3dVector4 q[4];//远平面四个顶点的视口空间坐标
    q[0].init(left,down,1,1);
    q[1].init(right,down,1,1);
    q[2].init(right,up,1,1);
    q[3].init(left,up,1,1);
    //求invProjectionView
    Cc3dMatrix4 projectionInverse=inverse(projectionMatrix);
    Cc3dMatrix4 viewInverse=inverse(viewMatrix);
    Cc3dMatrix4 projectionViewInverse=inverse(Cc3dMatrix4(projectionMatrix)*Cc3dMatrix4(viewMatrix));//viewInverse*projectionInverse;
    //将p转化到世界空间
    for(int i=0;i<=3;i++){
        //将p[i]转化到世界空间
        p[i]=convertFromViewportSpaceToWorldSpace(p[i],projectionViewInverse,viewport);
    }//p已转化为世界坐标
    //将q转化到世界空间
    for(int i=0;i<=3;i++){
        //将q[i]转化到世界空间
        q[i]=convertFromViewportSpaceToWorldSpace(q[i],projectionViewInverse,viewport);
    }//q已转化为世界坐标
    //求六个平面（法向指向视锥内部）
    //六个面的法向量
    Cc3dVector4 norm[6];
    norm[0]=calculateNormOfTri(p[2], p[1], p[0]);//近平面法向
    norm[1]=calculateNormOfTri(p[1], q[1], q[0]);
    norm[2]=calculateNormOfTri(p[1], q[2], q[1]);
    norm[3]=calculateNormOfTri(p[2], q[3], q[2]);
    norm[4]=calculateNormOfTri(p[0], q[0], q[3]);
    norm[5]=Cc3dVector4(0,0,0,0)-norm[0];//远平面法向（与近平面相反）
    //求六个面
    planeList[0].init(p[0],norm[0]);//近平面
    planeList[1].init(p[1],norm[1]);
    planeList[2].init(p[1],norm[2]);
    planeList[3].init(p[2],norm[3]);
    planeList[4].init(p[0],norm[4]);
    planeList[5].init(q[0],norm[5]);//远平面
}
//--------camera
void Cc3dCamera::updateFrustum(){
    getFrustum().updateFrustum(calculateProjectionMat(), calculateViewMat());
}
Cc3dMatrix4 Cc3dCamera::calculateViewMat()const {
    Cc3dMatrix4 ret;
    ret=calculateViewMatrix(m_eyePos, m_center, m_up);
    return ret;
}
Cc3dMatrix4 Cc3dCamera::calculateViewMatInverse(){
    Cc3dMatrix4 ret;
    ret=calculateViewMatrixInverse(m_eyePos, m_center, m_up);
    return ret;
}
Cc3dMatrix4 Cc3dCamera::calculateProjectionMat()const {
    Cc3dMatrix4 projectionMat;
    switch (m_projectionMode) {
        case ec3dPerspectiveMode:
            projectionMat=::calculatePerspectiveProjectionMatrix(m_fovy, m_aspect, m_zNear, m_zFar);
            break;
        case ec3dOrthographicMode:
            projectionMat=::calculateOrthoProjectionMatrix(m_range.getMinX(), m_range.getMaxX(), m_range.getMinY(), m_range.getMaxY(), m_range.getMinZ(), m_range.getMaxZ());
            break;
        default:
            assert(false);
            break;
    }
    return projectionMat;
}
