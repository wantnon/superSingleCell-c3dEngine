//
//  c3dGL.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//

#ifndef __HelloOpenGL__c3dGL__
#define __HelloOpenGL__c3dGL__
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32) 
	#include "GL/glew.h"
	#include "GL/glut.h"
	#include "GL/gl.h"
	#include "GL/glext.h"
	#include "GL/glu.h"
	#include "GL/glut.h"
#else
    #include <OpenGLES/ES2/gl.h>
#endif

#endif /* defined(__HelloOpenGL__c3dGL__) */
