//
//  c3dTexture.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#include "c3dTexture.h"
bool Cc3dTexture::initDepthTexture(int width,int height,GLint minFilter,GLint magFilter){
    assert(m_texture==0);
    m_width=width;
    m_height=height;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    //GL_NEAREST,GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    //      // Remove artifact on the edges of the shadowmap
    //      //禁止纹理重复，似乎不光是禁止了重复，去掉下面两句效果完全混乱
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    //注意：深度纹理特殊
    //            1，用GL_DEPTH_COMPONENT，而不用GL_DEPTH_COMPONENT16
    //            2，用GL_UNSIGNED_INT，而不用GL_UNSIGNED_BYTE
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,m_width, m_height, 0,
                 GL_DEPTH_COMPONENT,GL_UNSIGNED_INT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    C3DCHECK_GL_ERROR_DEBUG();
    return true;
}

bool Cc3dTexture::initColorTexture(int width,int height,GLint minFilter,GLint magFilter, GLint internalformat){
    assert(m_texture==0);
    m_width=width;
    m_height=height;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    //GL_NEAREST或GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    //禁止纹理重复
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, m_width, m_height, 0,
                 internalformat, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    C3DCHECK_GL_ERROR_DEBUG();
    return true;
}

bool Cc3dTexture::init(const string&filePath,int wrapS,int wrapT)
//只支持2的幂的贴图
{
    assert(m_texture==0);
    m_filePath=filePath;
    //生成纹理
    // 1
    NSString *fileName_NS=[[[NSString alloc] initWithUTF8String:filePath.c_str()] autorelease];
    CGImageRef spriteImage = [UIImage imageNamed:fileName_NS].CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %@", fileName_NS);
        exit(1);
    }
    
    // 2
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    this->m_width=width;
    this->m_height=height;
    
    
    
    GLubyte * spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    // 3
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    
    CGContextRelease(spriteContext);
    
    // 4
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //GL_NEAREST,GL_LINEAR
    //no need to use mipmap, that is slow
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    
    /*  //生成mipmap，参考apple例程PVRTextureLoader
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR);//GL_NEAREST，GL_LINEAR，GL_LINEAR_MIPMAP_LINEAR，GL_LINEAR_MIPMAP_NEAREST
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);//GL_LINEAR_MIPMAP_NEAREST);// GL_NEAREST，GL_LINEAR，GL_LINEAR_MIPMAP_LINEAR，GL_LINEAR_MIPMAP_NEAREST
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
     glGenerateMipmap(GL_TEXTURE_2D);*/
    free(spriteData);
    //填充texture
    this->m_texture=texture;
    C3DCHECK_GL_ERROR_DEBUG();
    return true;

}
