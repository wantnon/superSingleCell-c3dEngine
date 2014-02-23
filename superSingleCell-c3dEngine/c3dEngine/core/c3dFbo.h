//
//  fbo.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-6.
//
//

#ifndef __HelloOpenGL__fbo__
#define __HelloOpenGL__fbo__

#include <iostream>
using namespace std;
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "c3dTexture.h"
class Cc3dFbo:public Cc3dObject
{
protected:
    Cc3dTexture*m_depthTex;
    Cc3dTexture*m_colorTex;
    GLuint m_fbo;
public:
    
    Cc3dFbo(){
        m_depthTex=NULL;
        m_colorTex=NULL;
        m_fbo=0;
    }
    ~Cc3dFbo(){
        if(m_fbo!=0)glDeleteFramebuffers(1, &m_fbo);
        if(m_depthTex)m_depthTex->release();
        if(m_colorTex)m_colorTex->release();
    }
    bool init(){
        glGenFramebuffers(1, &m_fbo);
        return true;
    }
    GLuint getID()const{return m_fbo;}
    void attachDepthTex(Cc3dTexture*depthTexture);
    void attachColorTex(Cc3dTexture*colorTexture);
      
    
};
#endif /* defined(__HelloOpenGL__fbo__) */
