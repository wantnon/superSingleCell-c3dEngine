//
//  c3dGestureAnalyzer.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//
#include "c3dGestureAnalyzer.h"
static Cc3dGestureAnalyzer*s_gestureAnalyzer=NULL;
Cc3dGestureAnalyzer* Cc3dGestureAnalyzer::sharedGestureAnalyzer(){
    if(s_gestureAnalyzer==NULL){
        s_gestureAnalyzer=new Cc3dGestureAnalyzer();
    }
    return s_gestureAnalyzer;
}
