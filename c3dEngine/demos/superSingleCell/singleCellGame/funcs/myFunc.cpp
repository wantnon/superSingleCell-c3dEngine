//
//  myFunc.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-19.
//
//

#include "myFunc.h"

Cc3dSubMeshData genQuad(float p[4][3]){
    Cc3dSubMeshData meshData;
    meshData.addIDtri(0, 1, 2);
    meshData.addIDtri(0, 2, 3);
    float u[4][2]={
        {0,1},
        {1,1},
        {1,0},
        {0,0}
    };
    float norm[4][3]={
        {0,0,0},
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };
    meshData.addVertex(p[0], u[0], norm[0]);
    meshData.addVertex(p[1], u[1], norm[1]);
    meshData.addVertex(p[2], u[2], norm[2]);
    meshData.addVertex(p[3], u[3], norm[3]);
    return meshData;
    
}
Cc3dRange rangeAdd(const Cc3dRange&range1,const Cc3dRange&range2){
    Cc3dRange range=range1;
    range.add(range2);
    return range;
    
}
Cc3dSubMeshData*genSampleCubeWithCode(float cx,float cy,float cz,float dx,float dy,float dz,
                                   float k_s,
                                   float k_t,
                                   int cubUVMode)
//用代码生成示例立方体--abc
//cx,cy,cz为中心--abc
//dx,dy,dz分别为x,y,z方向上的边心距--abc
{
    Cc3dSubMeshData*meshData=new Cc3dSubMeshData();
    meshData->init();
    meshData->autorelease();
    meshData->vlist.reserve(8);
    meshData->IDtriList.reserve(12);
    //----生成顶点数据--abc
    Cc3dVertex tv;
    //前面--abc
    tv.setPos(Cc3dVector3(cx-dx, cy-dy,cz+dz));tv.setTexCoord(Cc3dVector2(0, 0));tv.setNorm(Cc3dVector3(0, 0, 1));//0
    meshData->vlist.push_back(tv);
    tv.setPos(Cc3dVector3(cx+dx, cy-dy,cz+dz));tv.setTexCoord(Cc3dVector2(1, 0));tv.setNorm(Cc3dVector3(0, 0, 1));//1
    meshData->vlist.push_back(tv);
    tv.setPos(Cc3dVector3(cx+dx, cy+dy, cz+dz));tv.setTexCoord(Cc3dVector2(1, 1));tv.setNorm(Cc3dVector3(0, 0, 1));//2
    meshData->vlist.push_back(tv);
    tv.setPos(Cc3dVector3(cx-dx, cy+dy, cz+dz));tv.setTexCoord(Cc3dVector2(0, 1));tv.setNorm(Cc3dVector3(0, 0, 1));//3
    meshData->vlist.push_back(tv);
    //后面--abc
    tv.setPos(Cc3dVector3(cx-dx, cy-dy,cz -dz));tv.setTexCoord(Cc3dVector2(0, 0));tv.setNorm(Cc3dVector3(0, 0, -1));//4
    meshData->vlist.push_back(tv);
    tv.setPos(Cc3dVector3(cx+dx, cy-dy, cz-dz));tv.setTexCoord(Cc3dVector2(1, 0));tv.setNorm(Cc3dVector3(0, 0, -1));//5
    meshData->vlist.push_back(tv);
    tv.setPos(Cc3dVector3(cx+dx, cy+dy, cz-dz));tv.setTexCoord(Cc3dVector2(1, 1));tv.setNorm(Cc3dVector3(0, 0, -1));//6
    meshData->vlist.push_back(tv);
    tv.setPos(cx-dx, cy+dy, cz-dz);tv.setTexCoord(0, 1);tv.setNorm(0, 0, -1);//7
    meshData->vlist.push_back(tv);
    //上面--abc
    tv.setPos(cx-dx, cy+dy, cz+dz);tv.setTexCoord(0, 0);tv.setNorm(0, 1, 0);//8
    meshData->vlist.push_back(tv);
    tv.setPos(cx+dx, cy+dy, cz+dz);tv.setTexCoord(1, 0);tv.setNorm(0, 1, 0);//9
    meshData->vlist.push_back(tv);
    tv.setPos(cx+dx, cy+dy, cz-dz);tv.setTexCoord(1, 1);tv.setNorm(0, 1, 0);//10
    meshData->vlist.push_back(tv);
    tv.setPos(cx-dx, cy+dy, cz-dz);tv.setTexCoord(0, 1);tv.setNorm(0, 1, 0);//11
    meshData->vlist.push_back(tv);
    //下面--abc
    tv.setPos(cx-dx, cy-dy, cz+dz);tv.setTexCoord(0, 0);tv.setNorm(0, -1, 0);//12
    meshData->vlist.push_back(tv);
    tv.setPos(cx+dx, cy-dy, cz+dz);tv.setTexCoord(1, 0);tv.setNorm(0, -1, 0);//13
    meshData->vlist.push_back(tv);
    tv.setPos(cx+dx, cy-dy, cz-dz);tv.setTexCoord(1, 1);tv.setNorm(0, -1, 0);//14
    meshData->vlist.push_back(tv);
    tv.setPos(cx-dx, cy-dy, cz-dz);tv.setTexCoord(0, 1);tv.setNorm(0, -1, 0);//15
    meshData->vlist.push_back(tv);
    //左面--abc
    tv.setPos(cx-dx, cy-dy,cz -dz);tv.setTexCoord(0, 0);tv.setNorm(-1, 0, 0);//16
    meshData->vlist.push_back(tv);
    tv.setPos(cx-dx, cy-dy, cz+dz);tv.setTexCoord(1, 0);tv.setNorm(-1, 0, 0);//17
    meshData->vlist.push_back(tv);
    tv.setPos(cx-dx, cy+dy, cz+dz);tv.setTexCoord(1, 1);tv.setNorm(-1, 0, 0);//18
    meshData->vlist.push_back(tv);
    tv.setPos(cx-dx, cy+dy, cz-dz);tv.setTexCoord(0, 1);tv.setNorm(-1, 0, 0);//19
    meshData->vlist.push_back(tv);
    //右面--abc
    tv.setPos(cx+dx, cy-dy, cz-dz);tv.setTexCoord(0, 0);tv.setNorm(1, 0, 0);//20
    meshData->vlist.push_back(tv);
    tv.setPos(cx+dx, cy-dy,cz+ dz);tv.setTexCoord(1, 0);tv.setNorm(1, 0, 0);//21
    meshData->vlist.push_back(tv);
    tv.setPos(cx+dx, cy+dy, cz+dz);tv.setTexCoord(1, 1);tv.setNorm(1, 0, 0);//22
    meshData->vlist.push_back(tv);
    tv.setPos(cx+dx, cy+dy, cz-dz);tv.setTexCoord(0, 1);tv.setNorm(1, 0, 0);//23
    meshData->vlist.push_back(tv);
    //----生成索引三角形--abc
    Cc3dIDTriangle tIDtri;
    //前面--abc
    tIDtri.setvID(0, 1, 2);
    meshData->IDtriList.push_back(tIDtri);
    tIDtri.setvID(0, 2, 3);
    meshData->IDtriList.push_back(tIDtri);
    //后面--abc
    tIDtri.setvID(4, 6, 5);
    meshData->IDtriList.push_back(tIDtri);
    tIDtri.setvID(4, 7, 6);
    meshData->IDtriList.push_back(tIDtri);
    //上面--abc
    tIDtri.setvID(8, 9, 10);
    meshData->IDtriList.push_back(tIDtri);
    tIDtri.setvID(8, 10, 11);
    meshData->IDtriList.push_back(tIDtri);
    //下面--abc
    tIDtri.setvID(12, 14, 13);
    meshData->IDtriList.push_back(tIDtri);
    tIDtri.setvID(12, 15, 14);
    meshData->IDtriList.push_back(tIDtri);
    //左面--abc
    tIDtri.setvID(16, 17, 18);
    meshData->IDtriList.push_back(tIDtri);
    tIDtri.setvID(16, 18, 19);
    meshData->IDtriList.push_back(tIDtri);
    //右面--abc
    tIDtri.setvID(20, 22, 21);
    meshData->IDtriList.push_back(tIDtri);
    tIDtri.setvID(20, 23, 22);
    meshData->IDtriList.push_back(tIDtri);
    //----重新计算各三角形各顶点的uv
    int nv=(int)meshData->vlist.size();
    vector<bool> markList(nv);//标记各顶点是否已经重计算过uv，避免多次计算（多次计算可能导致出错),初始全0
    int nIDtri=(int)meshData->IDtriList.size();
    for(int i=0;i<nIDtri;i++){
        Cc3dIDTriangle&IDtri=meshData->IDtriList[i];
        //重新计算IDtri各顶点的uv
        switch(cubUVMode)
        {
            case cubUVMode_FITFACE:
            {
                //将IDtri中未mark的顶点的uv分别乘以k_s,k_t
                for(int j=0;j<3;j++){
                    int vID=IDtri.vID[j];//顶点号--abc
                    if(markList[vID]==false){//未计算过--abc
                        //重新计算--abc
                        Cc3dVertex&v=meshData->vlist[vID];
                        Cc3dVector2 texCoord=v.getTexCoord();
                        texCoord.setx(texCoord.x()*k_s);
                        texCoord.sety(texCoord.y()*k_t);
                        v.setTexCoord(texCoord);
                        markList[vID]=true;//标记为已计算--abc
                    }
                }
            }break;
            case cubUVMode_UNIFORM:
            {
                //将IDtri各顶点的uv，重新按顶点坐标来计算，并以k_s,k_t放缩--abc
                //判断IDtri的类型--abc
                const int type_LR=0;//左或右--abc
                const int type_UD=1;//上或下--abc
                const int type_FB=2;//前或后--abc
                int type=0;
                Cc3dVector3 norm=meshData->vlist[IDtri.vID[0]].getNorm();
                if(fabsf(fabsf(norm.x())-1)<0.01){//左面或右面--abc
                    type=type_LR;
                }else if(fabsf(fabsf(norm.y())-1)<0.01){//上面或下面--abc
                    type=type_UD;
                }else{//前面或后面--abc
                    type=type_FB;
                }//得到type
                for(int j=0;j<3;j++){
                    int vID=IDtri.vID[j];//顶点号--abc
                    if(markList[vID]==false){//未计算过--abc
                        //重新计算--abc
                        Cc3dVertex&v=meshData->vlist[vID];
                        switch (type) {
                            case type_LR:{
                                //用yz
                                Cc3dVector2 texCoord(v.getPos().y()*k_s,v.getPos().z()*k_t);
                                v.setTexCoord(texCoord);
                            }break;
                            case type_UD:{
                                //用xz
                                Cc3dVector2 texCoord(v.getPos().x()*k_s,v.getPos().z()*k_t);
                                v.setTexCoord(texCoord);
                            }break;
                            case type_FB:{
                                //用xy
                                Cc3dVector2 texCoord(v.getPos().x()*k_s,v.getPos().y()*k_t);
                                v.setTexCoord(texCoord);
                            }break;
                        }
                        //标记为已计算--abc
                        markList[vID]=true;
                    }
                }
            }break;
            default:
                cout<<"error:未知的cubUV映射类型!"<<endl;
                assert(false);
                break;
        }
        
        
    }
    return meshData;
    
}

void reCalNorms_precise_normByVertex(Cc3dSubMeshData*meshData){
    vector<Cc3dVector4> normList((int)meshData->vlist.size());//顶点法向先存到这里--abc
    //将normList中所有元素清0
    int nNorm=(int)normList.size();
    for(int i=0;i<nNorm;i++){
        Cc3dVector4&norm=normList[i];
        norm.init(0,0,0,0);
    }
    //计算顶点法向量--abc
    //注意，此时的顶点已经分裂了，只能通过位置是否相同来判断是否为同一顶点--abc
    int nv=(int)meshData->vlist.size();
    for(int i=0;i<nv;i++){
        const int vID=i;
        Cc3dVertex&v=meshData->vlist[vID];
        Cc3dVector3 pos=v.getPos();
        //求哪些三角形包含与v位置相同的顶点，就把这个三角形的法向量积加到此顶点上--abc
        int nIDtri=(int)meshData->IDtriList.size();
        for(int j=0;j<nIDtri;j++){
            Cc3dIDTriangle&IDtri=meshData->IDtriList[j];
            //看IDtri是否包含与v重合的顶点，如果有，就将IDtri的法向量累加到v上--abc
            bool haveSamePosVert=false;
            for(int k=0;k<3;k++){
                int _vID=IDtri.vID[k];
                Cc3dVertex&_v=meshData->vlist[_vID];
                Cc3dVector3 _pos=_v.getPos();
                //看_v是否与v重合--abc
                const float eps=0.000001;//容差--abc
                if(fabs(pos.x()-_pos.x())<eps
                   &&fabs(pos.y()-_pos.y())<eps
                   &&fabs(pos.z()-_pos.z())<eps){
                    haveSamePosVert=true;
                    break;
                }
            }//得到haveSamePosVert
            if(haveSamePosVert){//IDtri有顶点与v重合--abc
                //求IDtri的法向量--abc
                int ID0=IDtri.vID[0];
                int ID1=IDtri.vID[1];
                int ID2=IDtri.vID[2];
                Cc3dVertex&v0=meshData->vlist[ID0];
                Cc3dVertex&v1=meshData->vlist[ID1];
                Cc3dVertex&v2=meshData->vlist[ID2];
                Cc3dVector4 pos0=v0.getPos().toV4(1);
                Cc3dVector4 pos1=v1.getPos().toV4(1);
                Cc3dVector4 pos2=v2.getPos().toV4(1);
                Cc3dVector4 norm=calculateNormOfTri(pos0, pos1, pos2);
                //将norm加到v上--abc
                normList[vID]=normList[vID]+norm;
            }
        }
    }
    //对normList中所有元素进行规格化--abc
    for(int i=0;i<nNorm;i++){
        Cc3dVector4&norm=normList[i];
        norm=normalize(norm);
        
    }
    //用normList填充到顶点--abc
    for(int i=0;i<(int)meshData->vlist.size();i++){
        meshData->vlist[i].setNorm(normList[i].toV3());
    }
}
void reCalNorms_precise_normByFace(Cc3dSubMeshData*meshData){
    int nIDtri=(int)meshData->IDtriList.size();
    for(int i=0;i<nIDtri;i++){
        Cc3dIDTriangle&IDtri=meshData->IDtriList[i];
        int ID0=IDtri.vID[0];
        int ID1=IDtri.vID[1];
        int ID2=IDtri.vID[2];
        Cc3dVertex&v0=meshData->vlist[ID0];
        Cc3dVertex&v1=meshData->vlist[ID1];
        Cc3dVertex&v2=meshData->vlist[ID2];
        Cc3dVector4 pos0=v0.getPos().toV4(1);
        Cc3dVector4 pos1=v1.getPos().toV4(1);
        Cc3dVector4 pos2=v2.getPos().toV4(1);
        Cc3dVector4 norm=calculateNormOfTri(pos0, pos1, pos2);
        v0.setNorm(norm.toV3());
        v1.setNorm(norm.toV3());
        v2.setNorm(norm.toV3());
    }
    
}


Cc3dVector4 softMov(const Cc3dVector4&originPos,//初始位置--abc
                    const Cc3dVector4&tgPos,//目标位置--abc
                    const float percentage//移动百分比--abc
)//软移动，返回结果位置--abc
{
	//将originPos移到tgPos，软跟随效果，不一次到位，只移动percentage的比例--abc
	Cc3dVector4 vec=tgPos-originPos;//从originPos到tgPos的向量--abc
	Cc3dVector4 dvec=vec*percentage;//实际移动向量--abc
	//移动--abc
    Cc3dVector4 rsPos=originPos+dvec;
    return rsPos;
}

float softMov(const float origin,//初始值--abc
              const float tg,//目标值--abc
              const float percentage)//移动百分比--abc

//软移动，返回结果值--abc
{
	//将origin移到tg，软跟随效果，不一次到位，只移动percentage的比例--abc
	float diff=tg-origin;//从originPos到tgPos的差--abc
    float actualMov=diff*percentage;//实际移动量--abc
	//移动--abc
    float rs=origin+actualMov;
    return rs;
}

