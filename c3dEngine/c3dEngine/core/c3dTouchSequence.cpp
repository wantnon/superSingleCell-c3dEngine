//
//  c3dTouchSequence.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-21.
//
//

#include "c3dTouchSequence.h"
static Cc3dTouchSequence*s_touchSequence=NULL;
Cc3dTouchSequence*Cc3dTouchSequence::sharedTouchSequence(){
    if(s_touchSequence==NULL){
        s_touchSequence=new Cc3dTouchSequence();
    }
    return s_touchSequence;
}