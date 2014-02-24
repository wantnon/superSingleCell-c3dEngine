//
//  c3dMath.cpp
//  HelloCpp
//
//  Created by wantnon (yang chao) on 14-1-4.
//
//

#include "c3dMath.h"
float isEqual(float a,float b,float eps){
    if(fabsf(a-b)<=eps)return true;
    else return false;
}


float maxf(float a,float b){
    return a>b?a:b;
}
float minf(float a,float b){
    return a<b?a:b;
}

Cc3dMatrix4 unitMat(){
    Cc3dMatrix4 mat(1,0,0,0,//col 1
                    0,1,0,0,
                    0,0,1,0,
                    0,0,0,1);
    return mat;
}
Cc3dMatrix4 zeroMat(){
    Cc3dMatrix4 mat(0,0,0,0,//col 1
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0);
    return mat;
}
bool isEqual(const Cc3dMatrix4&mat1,const Cc3dMatrix4&mat2,float eps){
    for(int i=0;i<16;i++){
        float d=fabsf(mat1.getAt(i)-mat2.getAt(i));
        if(d>eps)return false;
    }
    return true;
}
bool isEqual(const Cc3dVector4&v1,const Cc3dVector4&v2,float eps){
    for(int i=0;i<4;i++){
        float d=fabsf(v1.getAt(i)-v2.getAt(i));
        if(d>eps)return false;
    }
    return true;
}
bool isUnitMat(const Cc3dMatrix4&mat){
    return isEqual(mat, unitMat());
}
Cc3dVector4 normalize(const Cc3dVector4&v){
    //assert(v.w()==0);
    float r2=v.x()*v.x()+v.y()*v.y()+v.z()*v.z();
    if(r2==0){
        return Cc3dVector4(0,0,0,0);
    }
    float r=sqrtf(r2);
    Cc3dVector4 rs(v.x()/r,v.y()/r,v.z()/r,0);
    return rs;
}

float dot(const Cc3dVector4&v1,const Cc3dVector4&v2){
    return v1.x()*v2.x()+v1.y()*v2.y()+v1.z()*v2.z();
}
Cc3dVector4 cross(const Cc3dVector4&v1,const Cc3dVector4&v2)
{
    assert(v1.w()==0);
    assert(v2.w()==0);
    Cc3dVector4 rs(v1.y()*v2.z()-v1.z()*v2.y(),
                   v1.z()*v2.x()-v1.x()*v2.z(),
                   v1.x()*v2.y()-v1.y()*v2.x(),
                   0);//cross product result must be a vector, so the fourth component set to zero
    return rs;
}
Cc3dVector4 componentProduct(const Cc3dVector4&v1,const Cc3dVector4&v2){
    Cc3dVector4 v(v1.x()*v2.x(),
                             v1.y()*v2.y(),
                             v1.z()*v2.z(),
                             v1.w()*v2.w());
    return v;
}
Cc3dMatrix4 transpose(const Cc3dMatrix4&mat){
    
    const float* m=mat.getArray();
    float rs[16]={
        m[0],m[4],m[8],m[12],//col 1
        m[1],m[5],m[9],m[13],//col 2
        m[2],m[6],m[10],m[14],//col 3
        m[3],m[7],m[11],m[15]//col 4
    };
    return Cc3dMatrix4(rs);
}
Cc3dMatrix4 inverse(const Cc3dMatrix4&mat){
    //this code is copy from: http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    float m[16];
    for(int i=0;i<16;i++)m[i]=mat.getAt(i);
    
    float invOut[16];
    
    float inv[16], det;
    int i;
    
    inv[0] = m[5]  * m[10] * m[15] -
    m[5]  * m[11] * m[14] -
    m[9]  * m[6]  * m[15] +
    m[9]  * m[7]  * m[14] +
    m[13] * m[6]  * m[11] -
    m[13] * m[7]  * m[10];
    
    inv[4] = -m[4]  * m[10] * m[15] +
    m[4]  * m[11] * m[14] +
    m[8]  * m[6]  * m[15] -
    m[8]  * m[7]  * m[14] -
    m[12] * m[6]  * m[11] +
    m[12] * m[7]  * m[10];
    
    inv[8] = m[4]  * m[9] * m[15] -
    m[4]  * m[11] * m[13] -
    m[8]  * m[5] * m[15] +
    m[8]  * m[7] * m[13] +
    m[12] * m[5] * m[11] -
    m[12] * m[7] * m[9];
    
    inv[12] = -m[4]  * m[9] * m[14] +
    m[4]  * m[10] * m[13] +
    m[8]  * m[5] * m[14] -
    m[8]  * m[6] * m[13] -
    m[12] * m[5] * m[10] +
    m[12] * m[6] * m[9];
    
    inv[1] = -m[1]  * m[10] * m[15] +
    m[1]  * m[11] * m[14] +
    m[9]  * m[2] * m[15] -
    m[9]  * m[3] * m[14] -
    m[13] * m[2] * m[11] +
    m[13] * m[3] * m[10];
    
    inv[5] = m[0]  * m[10] * m[15] -
    m[0]  * m[11] * m[14] -
    m[8]  * m[2] * m[15] +
    m[8]  * m[3] * m[14] +
    m[12] * m[2] * m[11] -
    m[12] * m[3] * m[10];
    
    inv[9] = -m[0]  * m[9] * m[15] +
    m[0]  * m[11] * m[13] +
    m[8]  * m[1] * m[15] -
    m[8]  * m[3] * m[13] -
    m[12] * m[1] * m[11] +
    m[12] * m[3] * m[9];
    
    inv[13] = m[0]  * m[9] * m[14] -
    m[0]  * m[10] * m[13] -
    m[8]  * m[1] * m[14] +
    m[8]  * m[2] * m[13] +
    m[12] * m[1] * m[10] -
    m[12] * m[2] * m[9];
    
    inv[2] = m[1]  * m[6] * m[15] -
    m[1]  * m[7] * m[14] -
    m[5]  * m[2] * m[15] +
    m[5]  * m[3] * m[14] +
    m[13] * m[2] * m[7] -
    m[13] * m[3] * m[6];
    
    inv[6] = -m[0]  * m[6] * m[15] +
    m[0]  * m[7] * m[14] +
    m[4]  * m[2] * m[15] -
    m[4]  * m[3] * m[14] -
    m[12] * m[2] * m[7] +
    m[12] * m[3] * m[6];
    
    inv[10] = m[0]  * m[5] * m[15] -
    m[0]  * m[7] * m[13] -
    m[4]  * m[1] * m[15] +
    m[4]  * m[3] * m[13] +
    m[12] * m[1] * m[7] -
    m[12] * m[3] * m[5];
    
    inv[14] = -m[0]  * m[5] * m[14] +
    m[0]  * m[6] * m[13] +
    m[4]  * m[1] * m[14] -
    m[4]  * m[2] * m[13] -
    m[12] * m[1] * m[6] +
    m[12] * m[2] * m[5];
    
    inv[3] = -m[1] * m[6] * m[11] +
    m[1] * m[7] * m[10] +
    m[5] * m[2] * m[11] -
    m[5] * m[3] * m[10] -
    m[9] * m[2] * m[7] +
    m[9] * m[3] * m[6];
    
    inv[7] = m[0] * m[6] * m[11] -
    m[0] * m[7] * m[10] -
    m[4] * m[2] * m[11] +
    m[4] * m[3] * m[10] +
    m[8] * m[2] * m[7] -
    m[8] * m[3] * m[6];
    
    inv[11] = -m[0] * m[5] * m[11] +
    m[0] * m[7] * m[9] +
    m[4] * m[1] * m[11] -
    m[4] * m[3] * m[9] -
    m[8] * m[1] * m[7] +
    m[8] * m[3] * m[5];
    
    inv[15] = m[0] * m[5] * m[10] -
    m[0] * m[6] * m[9] -
    m[4] * m[1] * m[10] +
    m[4] * m[2] * m[9] +
    m[8] * m[1] * m[6] -
    m[8] * m[2] * m[5];
    
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    
    if (det == 0)
        //return false;
        assert(false);
    
    det = 1.0 / det;
    
    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
    
   // return true;
    return Cc3dMatrix4(invOut);
}
Cc3dVector4 toV4(const Cc3dVector2&v2,float z,float w){
    return Cc3dVector4(v2.x(), v2.y(), z, w);
}
Cc3dVector2 toV2(const Cc3dVector4&v4){
    return Cc3dVector2(v4.x(), v4.y());
}
float getLength2(const Cc3dVector4&v){//square of length
    assert(v.w()==0);
    return v.x()*v.x()+v.y()*v.y()+v.z()*v.z();
}
float getLength(const Cc3dVector4&v){
    assert(v.w()==0);
    return sqrtf(v.x()*v.x()+v.y()*v.y()+v.z()*v.z());
}

Cc3dMatrix4 orthogonalization3x3(const Cc3dMatrix4&mat)
//对mat的3*3部分进行规范正交化
//*****除了计算误差外，软旋转的近似插值也会使矩阵变得非正交*****
{
	//（以<,>表内积）
	//
	//b1=a1                      u1=b1/|b1|
	//b2=a2-<a2,u1>u1            u2=b2/|b2|
	//b3=a3-<a3,u1>u1-<a3,u2>u2  u3=b3/|b3|
	//则b1,b2,b3为正交基，u1,u2,u3为标准正交基
	//
    float x=mat.getAt(12);
    float y=mat.getAt(13);
    float z=mat.getAt(14);
	//进行规范正交化
    Cc3dVector4 a1(mat.getAt(0),mat.getAt(1),mat.getAt(2),0);
    Cc3dVector4 a2(mat.getAt(4),mat.getAt(5),mat.getAt(6),0);
    Cc3dVector4 a3(mat.getAt(8),mat.getAt(9),mat.getAt(10),0);
    
	Cc3dVector4 u1,u2,u3;
	{
		Cc3dVector4 b1,b2,b3;
		//计算b1,u1
		{
            b1=a1;
            u1=b1;
            u1=normalize(u1);
		}
		//计算b2,u2
		{
			float k=dot(a2,u1);
			Cc3dVector4 ku1=u1*k;
            b2=a2-ku1;
            u2=b2;
            u2=normalize(u2);
		}
		//计算b3,u3
		{
			float k=dot(a3,u1);
			float _k=dot(a3,u2);
			Cc3dVector4 ku1,_ku2;
            ku1=u1*k;
            _ku2=u2*_k;
			Cc3dVector4 temp;//ku1+_ku2
            temp=ku1+_ku2;
            b3=a3-temp;
            u3=b3;
            u3=normalize(u3);
		}
	}
	//得到u1,u2,u3
	//将u1~3拷贝到rsmat
    Cc3dMatrix4 rsmat;
    rsmat.init(u1.x(),u1.y(),u1.z(),0,//col 1
               u2.x(),u2.y(),u2.z(),0,
               u3.x(),u3.y(),u3.z(),0,
               x,y,z,1);
    return rsmat;
	
}

