//
//  c3dTexturePlatformDepend.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#ifndef __HelloOpenGL__c3dTexturePlatformDepend__
#define __HelloOpenGL__c3dTexturePlatformDepend__

#include <iostream>
using namespace std;
#include "c3dGLdebug.h"
GLuint createGLTexture_iOS(const string&filePath,int wrapS,int wrapT,GLint minFilter,GLint magFilter,
                           float&textureWidth,float&textureHeight);
#endif /* defined(__HelloOpenGL__c3dTexturePlatformDepend__) */
