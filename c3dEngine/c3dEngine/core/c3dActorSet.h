//
//  c3dActorSet.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-21.
//
//

#ifndef __HelloOpenGL__c3dActorSet__
#define __HelloOpenGL__c3dActorSet__

#include <iostream>
using namespace std;
#include "c3dNode.h"
class Cc3dActorSet{
protected:
    vector<Cc3dNode*> actorList;
public:
    void addActor(Cc3dNode*node){
        assert(node);
        actorList.push_back(node);
    }
    void clear(){
        actorList.clear();
    }
    int getActorCount()const{return (int)actorList.size();}
    Cc3dNode* getActorByIndex(int index){
        assert(index>=0&&index<=(int)actorList.size()-1);
        return actorList[index];
    }
    const vector<Cc3dNode*>&getActorList(){return actorList;}
    void setActorList(const vector<Cc3dNode*>&list){actorList=list;}
    
};


#endif /* defined(__HelloOpenGL__c3dActorSet__) */
