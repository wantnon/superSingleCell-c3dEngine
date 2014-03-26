//
//  archScene.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-26.
//
//

#ifndef __HelloOpenGL__archScene__
#define __HelloOpenGL__archScene__

#include <iostream>
#include "c3dMesh.h"
#include "octree.h"
#include "geoMath.h"
#include "collisionWithModelAtTriLevel.h"
#include "loadInfo.h"
#include "c3dFileUtils.h"
bool matchPrefix(const string&str,const string&strPrefix);
#define tag_collisionSubMesh 1
class CarchScene:public Coctree
{
public:
    int nSubMesh;//submesh数量
    float modelScale;//模型缩放比例

    CarchScene(){
        nSubMesh=0;
    }
    virtual ~CarchScene() {
    
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
        //------------------提取文件信息，并建立模型列表
        {
            char buffer[1024]={0};
            //提取放缩系数
            float scale;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&scale);
            assert(scale==1);
            this->modelScale=scale;
        }
        //关闭文件
        fclose(fp);
    }
    void init(const string&configFilePathShort,const string&modelClipPathShort){
        Coctree::init();
        //加载配置文件
        {
            loadConfig(configFilePathShort);
        }
        //加载模型
        {
            loadInfo_mesh(this->getMesh(),modelClipPathShort,this->modelScale);
        }
        //将定位submesh舍去
        {
            int nSubMesh=(int)this->getMesh()->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                Cc3dSubMesh*subMesh=this->getMesh()->getSubMeshByIndex(i);
                if(matchPrefix(getFileNameFromFilePath(subMesh->getTexture()->getFilePath()),"boss")||getFileNameFromFilePath(subMesh->getTexture()->getFilePath())=="myCell.png"){
                    assert(this->getMesh()->isHaveChild(subMesh));
                    this->getMesh()->removeMeshByIndex(i);
                    nSubMesh--;
                    i--;
                }
            }
     /*       //对pmeshList进行紧缩
            int nullCount=0;
            for(int i=0;i<nSubMesh;i++){
                if(this->getMesh()->getSubMeshByIndex(i)==NULL){
                    nullCount++;
                }else{
                    this->getMesh()->getSubMeshByIndex(i-nullCount)=this->getMesh()->getSubMeshByIndex(i);
                }
            }
            this->getMesh()->pmeshList.resize(nSubMesh-nullCount);
      */
        }
        //对collision submesh标记tag
        {
            int nSubMesh=(int)this->getMesh()->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                Cc3dSubMesh*subMesh=this->getMesh()->getSubMeshByIndex(i);
                if(getFileNameFromFilePath(subMesh->getTexture()->getFilePath())=="cb.png"){
                    subMesh->setTag(tag_collisionSubMesh);
                }
            }

            
        }
        //为indexVBO创建缓冲
        {
            int nSubMesh=(int)this->getMesh()->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                this->getMesh()->getSubMeshByIndex(i)->getIndexVBO()->genBuffers();
            }
        }
        //制作octree
        this->makeOctree();
        //提交各submesh的顶点数据
        {
            int nSubMesh=(int)this->getMesh()->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                this->getMesh()->getSubMeshByIndex(i)->getIndexVBO()->submitVertex(this->getMesh()->getSubMeshByIndex(i)->getSubMeshData()->vlist, GL_STATIC_DRAW);
            }
        }


    }

    
    
    void submitIDtriAroundSphere(const Cc3dVector4&c,float R)
    //显示球体(c,R)附近透明三角面
    {
		//-------------------------获得主角周围的三角形
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
        vector<CIDTriForOctree*> pIDtriExList=getCollisionIDtriList(c, R, skipTagList);
        //将pIDtriExList填充到各submesh
        {
            //----清空各submesh的IDtriList
            int nSubMesh=(int)this->getMesh()->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                this->getMesh()->getSubMeshByIndex(i)->getSubMeshData()->IDtriList.clear();
            }
            //----将pIDtriExList中的三角形填充到相应的submesh的IDtriList中
            int nIDtriEx=(int)pIDtriExList.size();
            for(int i=0;i<nIDtriEx;i++){
                CIDTriForOctree*pIDtriEx=pIDtriExList[i];
                int meshID=pIDtriEx->getSubMeshID();
                this->getMesh()->getSubMeshByIndex(meshID)->getSubMeshData()->IDtriList.push_back(pIDtriEx->getIDtri());
            }
            //----提交各submesh的索引表
            for(int i=0;i<nSubMesh;i++){
                this->getMesh()->getSubMeshByIndex(i)->getIndexVBO()->submitIndex(this->getMesh()->getSubMeshByIndex(i)->getSubMeshData()->IDtriList , GL_STREAM_DRAW);
            }
        }

	}

    
    
    
    //-------------------------碰撞 ----------------------------------------
    
	

    bool collisionTest_lineSeg(const Cc3dVector4&p1,const Cc3dVector4&p2,Cc3dVector4&p,Cc3dVector4&collisionFaceNorm)
    //判断线段p1p2是否与this相交，同时得到碰撞点坐标
    //前端返回是否相交
    //p带会交点坐标
	{
        assert(p1.w()==1);
        assert(p2.w()==1);
		//作以p1,p2中点为球心，p1p2距离的一半为半径的球体
		Cc3dVector4 c=(p1+p2)*0.5;
		Cc3dVector4 p1p2=p2-p1;
		float R=getLength(p1p2)/2;
		//求球体(c,R)的碰撞三角面集
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
		vector<CtriangleWithNorm> triWithNormList=getCollisionTriangleList(c,R,skipTagList);
		return collisionTest_lineSeg_common(triWithNormList, p1, p2, c, R, p,collisionFaceNorm);
	}
    float collisionTestWithWall_multiPoint(const Cc3dVector4&c,const float Rc,const float RH,const Cc3dVector4&up,Cc3dVector4&e_back,
                                           const vector<int>&skipTagList,const int intended_nSP)
    //以_c为球心，Rc为长半径的扁球体与*this进行多点碰撞检测（_c为世界坐标）
    //up为球体的向上方向
    //若碰撞，前端返回打入深度；若不碰撞，前端返回-1
    //若发生碰撞，则e_back带回反弹方向（单位向量，世界坐标），否则e_back无效
	{
		//求球体(c,R)的碰撞三角面集
		vector<CtriangleWithNorm> triWithNormList=getCollisionTriangleList(c,Rc,skipTagList);
        return collisionTestWithWall_multiPoint_common(triWithNormList, c, Rc,RH, up, e_back,intended_nSP);
		
	}
    float collisionTestWithWall_singlePoint(const Cc3dVector4&c,const float Rc,const float RH,const Cc3dVector4&up,Cc3dVector4&e_back,
                                           const vector<int>&skipTagList,
                                            const int intended_nSP)
    //以_c为球心，Rc为长半径的扁球体与*this进行多点碰撞检测（_c为世界坐标）
    //up为球体的向上方向
    //若碰撞，前端返回打入深度；若不碰撞，前端返回-1
    //若发生碰撞，则e_back带回反弹方向（单位向量，世界坐标），否则e_back无效
	{
		//求球体(c,R)的碰撞三角面集
		vector<CtriangleWithNorm> triWithNormList=getCollisionTriangleList(c,Rc,skipTagList);
        return collisionTestWithWall_singlePoint_common(triWithNormList, c, Rc,RH, up, e_back,intended_nSP);
		
	}

    float getH_floor(const Cc3dVector4&c,const float Rc,Cc3dVector4&houseFloorNorm)
    //求点c处house地板高度，并带回c处地板法向
    //如果返回-inf，则houseFloorNorm为(0,1,0)
	{
		//求球体(c,R)的碰撞三角面集
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
		vector<CtriangleWithNorm> triWithNormList=getCollisionTriangleList(c,Rc,skipTagList);
        float H;
        H=getH_floor_common(triWithNormList, c, Rc, houseFloorNorm);
		return H;
	}
   
    float getH_ceil(const Cc3dVector4&c,const float Rc,Cc3dVector4&houseCeilNorm)
    //求点c处天花板高度，并带回c处天花板法向
    //如果返回inf，则houseCeilNorm为(0,-1,0)
	{
		//求球体(c,R)的碰撞三角面集
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
		vector<CtriangleWithNorm> triWithNormList=getCollisionTriangleList(c,Rc,skipTagList);
        float H;
        H=getH_ceil_common(triWithNormList,c, Rc, houseCeilNorm);
        return H;
    }
    

};
#endif /* defined(__HelloOpenGL__archScene__) */
