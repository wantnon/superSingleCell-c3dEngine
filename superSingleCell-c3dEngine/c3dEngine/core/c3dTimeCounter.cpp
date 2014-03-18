//
//  c3dTimeCounter.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-19.
//
//

#include "c3dTimeCounter.h"
static Cc3dTimeCounter*s_timeCounter=NULL;
Cc3dTimeCounter*Cc3dTimeCounter::sharedTimeCounter(){
    if(s_timeCounter==NULL){
        s_timeCounter=new Cc3dTimeCounter();
    }
    return s_timeCounter;
}