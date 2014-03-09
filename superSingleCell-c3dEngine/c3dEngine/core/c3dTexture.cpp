//
//  c3dTexture.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#include "c3dTexture.h"
#include "c3dTexturePlatformDepend.h"
bool Cc3dTexture::initDepthTexture(int width,int height,int wrapS,int wrapT,GLint minFilter,GLint magFilter)
//注意，如果用作render to texture的渲染目标，则wrapS和wrapT均应取GL_CLAMP_TO_EDGE
{
    assert(m_texture==0);
    m_width=width;
    m_height=height;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    //GL_NEAREST,GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    //
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrapS);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrapT);
    //注意：深度纹理特殊
    //            1，用GL_DEPTH_COMPONENT，而不用GL_DEPTH_COMPONENT16
    //            2，用GL_UNSIGNED_INT，而不用GL_UNSIGNED_BYTE
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,m_width, m_height, 0,
                 GL_DEPTH_COMPONENT,GL_UNSIGNED_INT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    C3DCHECK_GL_ERROR_DEBUG();
    return true;
}

bool Cc3dTexture::initColorTexture(int width,int height,int wrapS,int wrapT,GLint minFilter,GLint magFilter)
//注意，如果用作render to texture的渲染目标，则wrapS和wrapT均应取GL_CLAMP_TO_EDGE
{
    assert(m_texture==0);
    m_width=width;
    m_height=height;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    //GL_NEAREST或GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    C3DCHECK_GL_ERROR_DEBUG();
    return true;
}

bool Cc3dTexture::init(const string&filePath,int wrapS,int wrapT,GLint minFilter,GLint magFilter)
{
    GLuint texture;
    float width,height;
    texture=createGLTexture_iOS(filePath,wrapS,wrapT,minFilter,magFilter,width,height);
    //
    m_texture=texture;
    m_width=width;
    m_height=height;
    m_filePath=filePath;
    
    return true;

}
