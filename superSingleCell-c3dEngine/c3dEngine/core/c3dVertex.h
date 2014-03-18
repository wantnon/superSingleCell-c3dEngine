//
//  vertex.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#ifndef __HelloOpenGL__vertex__
#define __HelloOpenGL__vertex__

#include <iostream>
using namespace std;
#include "c3dDefine.h"
#include "c3dVector.h"
class Cc3dVertex USED_FOR_SUBMIT_TO_VBO
{
protected:
    Cc3dVector3 m_pos;
    Cc3dVector2 m_texCoord;
    Cc3dVector3 m_norm;
    Cc3dVector2 m_texCoord2;
public:
    Cc3dVertex(){};
    ~Cc3dVertex(){};//used for submit to vbo, so destructor must not be virtual
    Cc3dVertex(float x,float y,float z,
            float s,float t,
            float nx,float ny,float nz){
        init(x, y, z, s, t, nx, ny, nz);
    }
    Cc3dVertex(const float pos[3],const float texCoord[2],const float norm[3]){
        m_pos.init(pos);
        m_texCoord.init(texCoord);
        m_norm.init(norm);
    }
    Cc3dVertex(const float pos[3],const float texCoord[2]){
        m_pos.init(pos);
        m_texCoord.init(texCoord);
        
    }

    void init(float x,float y,float z,
              float s,float t,
              float nx,float ny,float nz){
        m_pos.init(x, y, z);
        m_texCoord.init(s,t);
        m_norm.init(nx, ny, nz);
        
    }
    Cc3dVector3 getPos()const {
        return m_pos;
    }
    Cc3dVector3 getNorm()const {
        return m_norm;
    }
    Cc3dVector2 getTexCoord()const {
        return m_texCoord;
    }
    void setPos(const Cc3dVector3&pos){
        m_pos=pos;

    }
    void setPos(float x,float y,float z){
        m_pos.init(x,y,z);
    }

    void setTexCoord(const Cc3dVector2&texCoord){
        m_texCoord=texCoord;
    }
    void setTexCoord(float s,float t){
        m_texCoord.init(s,t);
    }
    void setNorm(const Cc3dVector3&norm){
        m_norm=norm;
    }
    void setNorm(float x,float y,float z){
        m_norm.init(x,y,z);
    }
    void setTexCoord2(const Cc3dVector2&texCoord2){
        m_texCoord2=texCoord2;
    }
    void setTexCoord2(float s,float t){
        m_texCoord2.init(s,t);
    }
    
};


#endif /* defined(__HelloOpenGL__vertex__) */
