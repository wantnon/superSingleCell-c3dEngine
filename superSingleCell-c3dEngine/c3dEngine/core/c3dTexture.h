//
//  c3dTexture.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#ifndef __HelloOpenGL__c3dTexture__
#define __HelloOpenGL__c3dTexture__

#include <iostream>
using namespace std;
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "c3dObject.h"
#include "c3dGLdebug.h"
class Cc3dTexture:public Cc3dObject
{
protected:
    GLuint m_texture;
    float m_width,m_height;
    string m_filePath;
public:
    Cc3dTexture(){
        m_texture=0;
        m_width=m_height=0;
    }
    ~Cc3dTexture(){
        if(m_texture!=0){
            glDeleteTextures(1, &m_texture);
        }
    }
    
    bool getIsInvalid()const {
        return m_texture==0;
    }
    
    GLuint getID()const {
        return m_texture;
    }
    float getWidth()const {
        return m_width;
    }
    float getHeight()const {
        return m_height;
    }
    string getFilePath()const{
        return m_filePath;
    }
    bool init( const string&filePath,int wrapS=GL_REPEAT,int wrapT=GL_REPEAT);
    bool initDepthTexture(int width,int height,GLint minFilter,GLint magFilter);
    bool initColorTexture(int width,int height,GLint minFilter,GLint magFilter, GLint internalformat=GL_RGBA);

};

#endif /* defined(__HelloOpenGL__c3dTexture__) */
