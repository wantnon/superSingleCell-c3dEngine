//
//  c3dActor.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__c3dActor__
#define __HelloOpenGL__c3dActor__

#include <iostream>
#include "c3dModel.h"
#include "c3dAudioCache.h"
class Cc3dActor:public Cc3dNode
{
protected:
    vector<Cc3dModel*> m_modelList;
    vector<Cc3dALSource*> m_alSourceList;
public:
    Cc3dActor(){
        
    }
    virtual~Cc3dActor(){
      
        int nSource=(int)m_alSourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=m_alSourceList[i];
            source->release();
            cout<<"release source name:"<<source->getName()<<endl;
        }
    }
    int getModelCount()const{return (int)m_modelList.size();}
    Cc3dModel*getModelByIndex(int index){
        assert(index>=0&&index<(int)m_modelList.size());
        return m_modelList[index];
    }
    void addALSource(Cc3dALSource*source){
        assert(source);
        assert(getALSourceByName(source->getName())==NULL);
        m_alSourceList.push_back(source);
        source->retain();
    }
    void stopAllALSource(){
        int nSource=(int)m_alSourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=m_alSourceList[i];
            source->stop();
        }
    }
    Cc3dALSource* getALSourceByName(const string&name){
        int nSource=(int)m_alSourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=m_alSourceList[i];
            if(source->getName()==name)return source;
        }
        return NULL;
    }
    void setLight(Cc3dLight*light){
        this->Cc3dNode::setLight(light);
        int nModel=(int)m_modelList.size();
        for(int i=0;i<nModel;i++){
            if(m_modelList[i]==NULL)continue;
            m_modelList[i]->setLight(light);
        }
    }
    
    void setProgram(Cc3dProgram *program){
        assert(program);
        this->Cc3dNode::setProgram(program);
        
        int n=(int)m_modelList.size();
        for(int i=0;i<n;i++){
            Cc3dModel*p=m_modelList[i];
            if(p==NULL)continue;
            p->setProgram(program);
        }
    }
    void setCallback(void (*callback)(Cc3dNode*, Cc3dProgram*)){
        assert(callback);
        this->Cc3dNode::setCallback(callback);
        
        int n=(int)m_modelList.size();
        for(int i=0;i<n;i++){
            Cc3dModel*p=m_modelList[i];
            if(p==NULL)continue;
            p->setCallback(callback);
        }
        
    }
    void setIsDoUpdate(bool value){
        this->Cc3dNode::setIsDoUpdate(value);
        int nModel=(int)m_modelList.size();
        for(int i=0;i<nModel;i++){
            if(m_modelList[i]==NULL)continue;
            m_modelList[i]->setIsDoUpdate(value);
        }
    }
    void setIsDoUpdateRecursively(bool value){
        this->Cc3dNode::setIsDoUpdateRecursively(value);
        int nModel=(int)m_modelList.size();
        for(int i=0;i<nModel;i++){
            if(m_modelList[i]==NULL)continue;
            m_modelList[i]->setIsDoUpdateRecursively(value);
        }
    }
    void setIsVisible(bool value){
        this->Cc3dNode::setIsVisible(value);
        int nModel=(int)m_modelList.size();
        for(int i=0;i<nModel;i++){
            if(m_modelList[i]==NULL)continue;
            m_modelList[i]->setIsVisible(value);
        }
    }
    void setIsVisibleRecursively(bool value){
        this->Cc3dNode::setIsVisibleRecursively(value);
        int nModel=(int)m_modelList.size();
        for(int i=0;i<nModel;i++){
            if(m_modelList[i]==NULL)continue;
            m_modelList[i]->setIsVisibleRecursively(value);
        }
    }
    void setCamera(Cc3dCamera*camera){
        
        this->Cc3dNode::setCamera(camera);
        
        int n=(int)m_modelList.size();
        for(int i=0;i<n;i++){
            Cc3dModel*p=m_modelList[i];
            if(p==NULL)continue;
            p->setCamera(camera);
        }
        
    }
    vector<Cc3dModel*> getModelList()const{return m_modelList;}
    void addModel(Cc3dModel*pModel);
    void submitVertex(GLenum usage);
    void submitIndex(GLenum usage);

};



#endif /* defined(__HelloOpenGL__c3dActor__) */
