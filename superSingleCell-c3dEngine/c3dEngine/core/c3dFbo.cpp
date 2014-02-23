//
//  fbo.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-6.
//
//

#include "c3dFbo.h"
void Cc3dFbo::attachDepthTex(Cc3dTexture*depthTexture){
    assert(depthTexture);
    assert(m_depthTex==NULL);
    m_depthTex=depthTexture;
    m_depthTex->retain();
    GLuint depthTex=m_depthTex->getID();
    GLenum FBOstatus;
    GLint oldFbo=0;//当前fbo留底，以便恢复
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    //不支持glDrawBuffer(GL_NONE)和glReadBuffer(GL_NONE)
    // attach the texture to FBO depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthTex, 0);
    // check FBO status
    FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(FBOstatus != GL_FRAMEBUFFER_COMPLETE){
        cout<<"error: GL_FRAMEBUFFER_COMPLETE failed!";
        switch(FBOstatus){
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                cout<<0<<endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                cout<<1<<endl;
                break ;
            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                cout<<2<<endl;
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                cout<<3<<endl;
                break;
                
        }
        assert(false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
}
void Cc3dFbo::attachColorTex(Cc3dTexture*colorTexture){
    assert(colorTexture);
    assert(m_colorTex==NULL);
    m_colorTex=colorTexture;
    m_colorTex->retain();
    GLuint colorTex=m_colorTex->getID();
    GLenum FBOstatus;
    GLint oldFbo=0;//当前fbo留底，以便恢复
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    //不支持glDrawBuffer(GL_NONE)和glReadBuffer(GL_NONE)
    // attach the texture to FBO depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, colorTex, 0);//由于提供了colorTex，所以不用提前bind(colorTex)
    //opengl(es)的设计原则就是如果参数没指明一个东西
    //需要提前bind，如果参数中指明了一个东西，就不用提前bind
    // check FBO status
    FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(FBOstatus != GL_FRAMEBUFFER_COMPLETE){
        cout<<"error: GL_FRAMEBUFFER_COMPLETE failed!";
        assert(false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
}