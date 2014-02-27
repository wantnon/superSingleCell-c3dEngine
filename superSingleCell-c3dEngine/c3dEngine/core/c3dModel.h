//
//  c3dModel.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-3.
//
//

#ifndef HelloOpenGL_c3dModel_h
#define HelloOpenGL_c3dModel_h
#include "c3dLight.h"
#include "c3dCamera.h"
#include "c3dTransform.h"
#include "c3dIndexVBO.h"
#include "c3dMesh.h"
#include "c3dObject.h"
#include "c3dNode.h"
#include "c3dProgram.h"
#include "c3dLight.h"


class Cc3dModel:public Cc3dNode
{

protected:
    vector<Cc3dMesh*> m_meshList;//网格列表
    
public:
    Cc3dModel(){

    }
    virtual ~Cc3dModel(){
        
    }
    int getMeshCount()const{return (int)m_meshList.size();}
    Cc3dMesh*getMeshByIndex(int index)const{
        assert(index>=0&&index<(int)m_meshList.size());
        return m_meshList[index];
    }
    vector<Cc3dMesh*> getMeshList()const{return m_meshList;}
    void removeMeshByIndex(int index){
        assert(index>=0&&index<(int)m_meshList.size());
        m_meshList[index]->removeFromParent();
        m_meshList.erase(m_meshList.begin()+index);
    }
    Cc3dRange getRange()const ;
    void scale_modifyVertexDirectly(float kx,float ky,float kz);


    
    void genVBOBuffers();
  
    void setCullFace(Ec3dCullFace cullFace){
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setCullFace(cullFace);
        }
    }
    void setIsDoDepthTest(bool isDoDepthTest){
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setIsDoDepthTest(isDoDepthTest);
        }
    }
    void setIsWriteDepthBuffer(bool value){
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setIsWriteDepthBuffer(value);
        }
        
    }
    void setCamera(Cc3dCamera*camera){
        
        this->Cc3dNode::setCamera(camera);
        
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setCamera(camera);
        }
    
    }
    void setProgram(Cc3dProgram *program){
        assert(program);
        this->Cc3dNode::setProgram(program);
        
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setProgram(program);
        }
    }
    void setCallback(passUnifoCallbackPtr callback){
        assert(callback);
        this->Cc3dNode::setCallback(callback);
        
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setCallback(callback);
        }
    
    }
    void setLight(Cc3dLight*light){
        
        this->Cc3dNode::setLight(light);
        
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setLight(light);
        }
    }
    void setIsDoUpdate(bool value){
        this->Cc3dNode::setIsDoUpdate(value);
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setIsDoUpdate(value);
        }
    }
    void setIsDoUpdateRecursively(bool value){
        this->Cc3dNode::setIsDoUpdateRecursively(value);
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setIsDoUpdateRecursively(value);
        }
    }
    void setIsVisible(bool value){
        this->Cc3dNode::setIsVisible(value);
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setIsVisible(value);
        }
    }
    void setIsVisibleRecursively(bool value){
        this->Cc3dNode::setIsVisibleRecursively(value);
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh*pmesh=m_meshList[i];
            if(pmesh==NULL)continue;
            pmesh->setIsVisibleRecursively(value);
        }
    }
    void setColor(float r,float g,float b,float a);
    void setDiffuseAlpha(float alpha);
    void setDiffuseRGB(float r,float g,float b);
    void setAmbient(float r,float g,float b);
    void setSpecular(float r,float g,float b);
    void setShininess(float shininess);
    void setIsHighlightUntransp(bool isHighlightUntransp);
    void submit(GLenum usage);
    void submitVertex(GLenum usage);
    void submitIndex(GLenum usage);
    void addMesh(Cc3dMesh*mesh);
    void releaseIDtriList();
    void releasevList();

};




#endif
