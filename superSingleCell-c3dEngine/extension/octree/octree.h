//
//  octree.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-23.
//
//

#ifndef __HelloOpenGL__octree__
#define __HelloOpenGL__octree__

#include <iostream>
#include <vector>
#include "c3dMesh.h"
#include "geoMath.h"
#include "triangleWithNorm.h"
#include "c3dModel.h"
#include "c3dCommonFunc.h"
#include "c3dActor.h"
class CIDTriForOctree
{
protected:
    Cc3dIDTriangle m_IDtri;
    int m_meshID;//指明此三角形属于哪个mesh
    bool m_isAdded;//避免三角面重复
    Cc3dVector4 m_norm;
public:
   
    
    CIDTriForOctree(){
        m_meshID=-1;
        m_isAdded=false;
    }
    virtual ~CIDTriForOctree(){}
    Cc3dVector4 getNorm()const{return m_norm;}
    void setNorm(Cc3dVector4 norm){m_norm=norm;}
    void setMeshID(int meshID){
        m_meshID=meshID;
    }
    int getMeshID()const{return m_meshID;};
    void setIsAdded(bool value){
        m_isAdded=value;
    }
    bool getIsAdded()const{return m_isAdded;}
    void setIDtri(const Cc3dIDTriangle&IDtri){
        m_IDtri=IDtri;
    }
    Cc3dIDTriangle getIDtri()const{return m_IDtri;}
    
};

class CocNode
{
protected:
    Cc3dRange m_range;
	vector<CIDTriForOctree*> m_pIDtriExList;//必须用指针列表，因为不同叶子的pIDtriExList可能共用一些IDtriEx，并用IDtriEx.added来避免重复加入
	CocNode*m_children[8];
	bool m_isLeaf;
public:
    CocNode(){
        for(int i=0;i<8;i++){m_children[i]=NULL;}
        m_isLeaf=false;
    }
    virtual~CocNode(){}
    void clearpIDtriExListWithoutReleaseReferredElements();
    void clearpIDtriExListWithoutReleaseReferredElements_freeMem();
    void addpIDtriEx(CIDTriForOctree*pIDtriEx);
    vector<CIDTriForOctree*> getpIDTriExList()const{return m_pIDtriExList;}
    float getBoundingSphereRadius()const;
    Cc3dVector4 getCenter()const;
    int getIDtriExCount()const;
    CIDTriForOctree* getpIDtriExByIndex(int index)const;
    CocNode*getChildByIndex(int index)const;
    void setChildByIndex(int index,CocNode*node);
	bool getIsLeaf()const{return m_isLeaf;}
	void setIsLeaf(bool isLeaf){m_isLeaf=isLeaf;}
    void setRange(const Cc3dRange&range);
    Cc3dRange getRange()const{return m_range;}
};


class Coctree:public Cc3dActor
{
protected:
    CocNode*m_pRoot;//八叉树根指针
    vector<CIDTriForOctree*> m_pIDtriList;//所有叶子上的IDtriEx统一索引到这个表里
	vector<CocNode*> m_pVisibleLeafList;//可见叶子列表
    vector<CIDTriForOctree*> m_pVisibleIDtriList;//可见三角面列表(由可见叶子列表计算得来)
    Cc3dRange m_range;
	int m_leafCount;//叶子数
    int m_IDtriCount;//三角形数
public:
    Coctree(){
        initMembers();
	}
    virtual ~Coctree(){
        destory();
    }
    bool init(){
        Cc3dModel*model=new Cc3dModel();
        model->autorelease();
        model->init();
        addModel(model);
        return true;
    }
    Cc3dModel*getModel()const{
        assert(getModelCount()==1);
        return m_modelList[0];
    }
    void makeOctree();
    Cc3dRange getRangeOfIDtris()const;
    Cc3dRange getRangeOfIDtrisWithTags(const vector<int>&tagList)const;
    vector<CocNode*>  getpCollisionLeafList(const Cc3dVector4&c,float R);
	vector<CtriangleWithNorm> getCollisionTriangleList(const Cc3dVector4&c,float R,const vector<int>&skipTagList);
    int getLeafCount();
    int getIDtriCount();
    void updateVisibleIDTriList(const vector<int>&skipTagList);
    void submitVisibleIDTriList();
    
protected:
    void destory();
    vector<CIDTriForOctree*> getCollisionIDtriList(const Cc3dVector4&c,float R,const vector<int>&skipTagList);
	void updateVisibleNodeList();
    void getLeafCount_inn(CocNode*pNode,int&leafCount);
    void getpCollisionLeafList_inn(CocNode*pNode,const Cc3dVector4&c,float R, vector<CocNode*>&pCollisionLeafList);
    void updateVisibleNodeList_inn(CocNode*pNode);
    vector<CIDTriForOctree*> getIDtrisWithTags(const vector<int>&tagList)const;
    void makeOctree_inn(CocNode*&pNode,const vector<Cc3dMesh*>&pmeshList);
    void deletepIDtriExListForEachNONLeafNode(CocNode*pNode);
    void initMembers();
    void destoryAllNode();
    void destoryAllNode_inn(CocNode*pNode);
	bool intersect_IDTri_AABBBox(const float v0[3],const float v1[3],const float v2[3],
                                 float xmin,float xmax,float ymin,float ymax,float zmin,float zmax);
};
#endif /* defined(__HelloOpenGL__octree__) */
