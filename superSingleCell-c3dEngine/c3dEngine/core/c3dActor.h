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
    Cc3dModel* m_model;
    vector<Cc3dALSource*> m_alSourceList;
public:
    Cc3dActor(){
        m_model=NULL;
		assert(m_model==NULL);
		Cc3dNode::init();
		m_model = new Cc3dModel();
		m_model->autorelease();
		m_model->init();
		addChild(m_model);
    } 
    virtual~Cc3dActor(){ 
      
        int nSource=(int)m_alSourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=m_alSourceList[i];
            source->release();
            //cout<<"release source name:"<<source->getName()<<endl;
        }
    }
	Cc3dModel*getModel()const {return m_model;}
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
		assert(light);
		this->Cc3dNode::setLight(light);
        m_model->setLight(light);
    }
    
    void setProgram(Cc3dProgram *program){
        assert(program);
		this->Cc3dNode::setProgram(program);
		m_model->setProgram(program);
    }
    void setPassUnifoCallback(c3dPassUnifoCallbackPtr callback){
        assert(callback);
		this->Cc3dNode::setPassUnifoCallback(callback);
        m_model->setPassUnifoCallback(callback);
        
    }
    void setIsDoUpdate(bool value){
		this->Cc3dNode::setIsDoUpdate(value);
        m_model->setIsDoUpdate(value);
    }
    void setIsDoUpdateRecursively(bool value){
        this->Cc3dNode::setIsDoUpdateRecursively(value);
        m_model->setIsDoUpdateRecursively(value);
    }
    void setIsVisible(bool value){
        this->Cc3dNode::setIsVisible(value);
        m_model->setIsVisible(value);
    }
    void setIsVisibleRecursively(bool value){
        this->Cc3dNode::setIsVisibleRecursively(value);
		m_model->setIsVisibleRecursively(value);
    }
    void setCamera(Cc3dCamera*camera){
        
        this->Cc3dNode::setCamera(camera);
        
        m_model->setCamera(camera);
        
    }
    void addMesh(Cc3dMesh*mesh);
    void submitVertex(GLenum usage);
    void submitIndex(GLenum usage);

};



#endif /* defined(__HelloOpenGL__c3dActor__) */
