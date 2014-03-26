//
//  c3dSubMesh.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#ifndef __HelloOpenGL__c3dSubMesh__
#define __HelloOpenGL__c3dSubMesh__

#include <iostream>
#include <vector>
using namespace std;

#include "c3dIndexVBO.h"
#include "c3dTextureCache.h"
#include "c3dNode.h"
#include "c3dProgram.h"
#include "c3dProgramCache.h"
#include "c3dMaterial.h"
#include "c3dMeshData.h"
#include "c3dGLdebug.h"

class Cc3dSubMesh:public Cc3dNode
{
protected:
    Cc3dSubMeshData*m_subMeshData;
    Cc3dTexture *m_texture;
    Cc3dMaterial*m_material;
    bool m_isWriteDepthBuffer;
    bool m_isDoDepthTest;
    Ec3dCullFace m_cullFace;
    Cc3dIndexVBO*m_indexVBO;
public:
    Cc3dSubMesh(){
        m_cullFace=ec3dNotCull;
        m_subMeshData=NULL;
        m_texture=NULL;
        m_material=NULL;
        m_indexVBO=NULL;
        m_isWriteDepthBuffer=true;
        m_isDoDepthTest=true;
    }
    virtual~Cc3dSubMesh(){
        if(m_subMeshData)m_subMeshData->release();
        if(m_texture)m_texture->release();
        if(m_material)m_material->release();
        if(m_indexVBO)m_indexVBO->release();
    }
    bool init(){
        if(Cc3dNode::init()==false)return false;
        //default material
        Cc3dMaterial*material=new Cc3dMaterial();
        material->init();
        material->autorelease();
        setMaterial(material);
        //default meshData
        Cc3dSubMeshData*meshData=new Cc3dSubMeshData();
        meshData->init();
        meshData->autorelease();
        setMeshData(meshData);
        //default indexVBO
        Cc3dIndexVBO*indexVBO=new Cc3dIndexVBO();
        indexVBO->init();
        indexVBO->autorelease();
        setIndexVBO(indexVBO);
        return true;
    }
    void setIndexVBO(Cc3dIndexVBO*indexVBO){
        assert(indexVBO);
        if(m_indexVBO==NULL){
            m_indexVBO=indexVBO;
            m_indexVBO->retain();
        }else{
            m_indexVBO->release();
            m_indexVBO=indexVBO;
            m_indexVBO->retain();
        
        }
    }
    Cc3dIndexVBO*getIndexVBO()const{return m_indexVBO;}
    void setMeshData(Cc3dSubMeshData*meshData){
        assert(meshData);
        if(m_subMeshData==NULL){
            m_subMeshData=meshData;
            m_subMeshData->retain();
        }else{
            m_subMeshData->release();
            m_subMeshData=meshData;
            m_subMeshData->retain();
        }
    }
    void setCullFace(Ec3dCullFace cullFace){
        m_cullFace=cullFace;
    }
    Ec3dCullFace getCullFace()const {
        return m_cullFace;
    }
    Cc3dSubMeshData*getSubMeshData(){return m_subMeshData;}

    void setIsDoDepthTest(bool isDoDepthTest){m_isDoDepthTest=isDoDepthTest;}
    bool getIsDoDepthTest()const{return m_isDoDepthTest;}
    void setIsWriteDepthBuffer(bool value){
        m_isWriteDepthBuffer=value;
    }
    bool getIsWriteDepthBuffer()const{return m_isWriteDepthBuffer;}
    void setMaterial(Cc3dMaterial*material){
        assert(material);
        if(m_material==NULL){
            m_material=material;
            m_material->retain();
        }else{
            m_material->release();
            m_material=material;
            m_material->retain();
        
        }
    
    }
    Cc3dMaterial*getMaterial()const{return m_material;}
   
    
    void draw(){
        assert(m_camera);
        assert(m_light);
        assert(m_program);
        assert(m_passUnifoCallback);

        if(m_isDoDepthTest){
            glEnable(GL_DEPTH_TEST);
        }else{
            glDisable(GL_DEPTH_TEST);
        }
        if (m_isWriteDepthBuffer) {
            glDepthMask(GL_TRUE);
        }else{
            glDepthMask(GL_FALSE);
        }
        
        if(m_cullFace==ec3dNotCull){
            glDisable(GL_CULL_FACE);
        }else if(m_cullFace==ec3dCullBack){
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }else if(m_cullFace==ec3dCullFront){
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
        }else{
            assert(false);
        }
        m_program->useProgram();
        m_passUnifoCallback(this,m_program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture->getID());
        m_indexVBO->bindVertexBuffer();
        Cc3dIndexVBO::setPointers();
        m_indexVBO->bindIndexBuffer();
        m_indexVBO->drawIndexBuffer();
        m_indexVBO->unbindIndexBuffer();
        m_indexVBO->unbindVertexBuffer();

    }

    
    
    
    void setTexture(Cc3dTexture*texture){
        assert(texture);
        if(m_texture==NULL){
            m_texture=texture;
            m_texture->retain();
        }else{
            m_texture->release();
            m_texture=texture;
            m_texture->retain();
        }
        
    }
    Cc3dTexture*getTexture(){return m_texture;}


    void submit(GLenum usage);
    void submitVertex(GLenum usage);
    void submitIndex(GLenum usage);
    void clearMeshData();
    void addVertex(const float pos[3],const float texCoord[2],const float norm[3]);
    void addIDtri(int ID0,int ID1,int ID2);

 

};
#endif /* defined(__HelloOpenGL__c3dMesh__) */
