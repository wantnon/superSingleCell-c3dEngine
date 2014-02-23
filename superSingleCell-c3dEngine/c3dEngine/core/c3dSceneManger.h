//
//  c3dSceneManager.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-21.
//
//

#ifndef __HelloOpenGL__c3dSceneManager__
#define __HelloOpenGL__c3dSceneManager__

#include <iostream>
using namespace std;
#include "c3dNode.h"
//
class Cc3dSceneManager{
public:
    Cc3dSceneManager(){
        m_root=new Cc3dNode();
        m_root->autorelease();
        m_root->retain();
    }
    virtual~Cc3dSceneManager(){
        if(m_root)m_root->release();
    }
    static Cc3dSceneManager*sharedSceneManager();
    
    Cc3dNode*getRoot()const{return m_root;}
    
    void performDelayRemove(){
        performDelayRemove_inn(m_root);
    }
    
 
protected:
    void performDelayRemove_inn(Cc3dNode*node){
        if(node->getIsRemoveOnNextFrame()){
            node->setIsRemoveOnNextFrame(false);//must set isDirty to false first
            node->removeFromParent();
            return;
        }
        vector<Cc3dNode*> childList=node->getChildren();
        int nChild=(int)childList.size();
        for(int i=0;i<nChild;i++){
            assert(childList[i]);
            performDelayRemove_inn(childList[i]);
        }
    
    }
protected:
    Cc3dNode*m_root;
    
    
};
#endif /* defined(__HelloOpenGL__c3dSceneManager__) */
