//
//  ground.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-4.
//
//

#ifndef __HelloOpenGL__ground__
#define __HelloOpenGL__ground__

#include <iostream>
using namespace std;
#include "terrain.h"
class Cground:public Cterrain
{
protected:
    bool m_isDoTexBlend;//是否通进纹理混合
public:
    void loadConfig(const string&fileNameWithExt);
    void loadConfig_texBlend(const string&fileNameWithExt);
    Cground(){
        m_isDoTexBlend=false;
    }
    virtual~Cground(){}
    void setIsDoTexBlend(bool value){
        m_isDoTexBlend=value;
    }
    bool getIsDoTexBlend()const {
        return m_isDoTexBlend;
        
    }
    

};
#endif /* defined(__HelloOpenGL__ground__) */
