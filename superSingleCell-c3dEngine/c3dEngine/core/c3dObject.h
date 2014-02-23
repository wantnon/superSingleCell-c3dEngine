//
//  c3dObject.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-13.
//
//

#ifndef __HelloOpenGL__c3dObject__
#define __HelloOpenGL__c3dObject__

#include <iostream>
#include <string>
using namespace std;
#include <assert.h>

class Cc3dObject{
public:
    Cc3dObject(){
        retainCount=1;
    }
    virtual~Cc3dObject(){}
    void retain(){
        retainCount++;
    }
    void autorelease();
    void release(){
        assert(retainCount>0);
        retainCount--;
        if(retainCount==0)delete this;
    
    }
    int getRetainCount()const {return retainCount;}
    bool init(){return true;}
  
protected:
    int retainCount;
};
#endif /* defined(__HelloOpenGL__c3dObject__) */
