#import "c3dView.h"
//
#include <iostream>
using namespace std;
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
//
#include "c3dSceneManger.h"
#include "c3dAutoreleasePool.h"
#include "c3dDeviceAndOSInfo.h"
#include "c3dAppState.h"
#include "c3dTextureCache.h"
#include "c3dAudioCache.h"
#include "c3dTimeCounter.h"
//
#include "initGame.h"



@implementation Cc3dView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}
- (void)setupLayer {
    _eaglLayer = (CAEAGLLayer*) self.layer;
    _eaglLayer.opaque = YES;
}

- (void)setupContext {   
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        assert(false);
    }
    
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        assert(false);
    }
}

- (void)setupDisplayLink {
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)setupRenderBuffer {
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
    /*Note: When the Core Animation layer’s bounds or properties change, 
     your application should reallocate the renderbuffer’s storage.
     If you do not reallocate the renderbuffers,
     the renderbuffer size won’t match the size of the view;
     in this case, Core Animation may scale the image’s contents to fit in the view. 
     To prevent this, the Xcode template reallocates the framebuffer and renderbuffer whenever the view layout changes.
     http://developer.apple.com/library/ios/#documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/WorkingwithEAGLContexts/WorkingwithEAGLContexts.html
     */
}

- (void)setupDepthBuffer {
    glGenRenderbuffers(1, &_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16 /*24_OES*/, self.frame.size.width, self.frame.size.height);
}

- (void)setupFrameBuffer {
    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
    //Test the framebuffer for completeness. This test only needs to be performed when the framebuffer's configuration changes.
    GLenum status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE){
        cout<<"failed to make complete framebuffer object!"<<endl;
        exit(0);
    }

}


- (void)initOpenAL {
	ALCcontext		*newContext = NULL;
	ALCdevice		*newDevice = NULL;
	
	// Create a new OpenAL Device
	// Pass NULL to specify the system’s default output device
	newDevice = alcOpenDevice(NULL);
	if (newDevice != NULL)
	{
		// Create a new OpenAL Context
		// The new context will render to the OpenAL Device just created
		newContext = alcCreateContext(newDevice, 0);
		if (newContext != NULL)
		{
			// Make the new context the Current OpenAL Context
			alcMakeContextCurrent(newContext);
					
		}else{
            cout<<"error:newContext==NULL!"<<endl;
            assert(false);
        }
	}else{
        cout<<"error:newDevice==NULL!"<<endl;
        assert(false);
    }
	// clear any errors
	alGetError();

}
- (void)cleanUpOpenAL {
    TeardownOpenAL();

}
- (void)render:(CADisplayLink*)displayLink {
   
    //switch back to screen fbo
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);//tile-base architecture要求bind之后立即clear更快些
    glViewport(Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getMinX(),
               Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getMinY(),
               Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getWidth() ,
               Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getHeight());//屏幕窗口
    
    if(Cc3dAppState::sharedAppState()->getIsInBackGround())return;

    
    //remove nodes marked as isRemoveOnNextFrame
    Cc3dSceneManager::sharedSceneManager()->performDelayRemove();
    //remove unused resource in caches (must after performDelayRemove)
    Cc3dTextureCache::sharedTextureCache()->performDelayRemoveUnusedTextures();
    Cc3dAudioCache::sharedAudioCache()->performDelayRemoveUnusedBuffersAndSources();
    //visitUpdate
    Cc3dSceneManager::sharedSceneManager()->getRoot()->visitUpdate();
    //visitDraw
    Cc3dSceneManager::sharedSceneManager()->getRoot()->visitDraw();
    //autorelease
    Cc3dAutoreleasePool::sharedAutoreleasePool()->releaseAll();
    //refresh
    //[self discardDepthBuffer];
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context presentRenderbuffer:GL_RENDERBUFFER];
    
    C3DCHECK_GL_ERROR_DEBUG() ;
    
    C3DCHECK_AL_ERROR_DEBUG();
    Cc3dTimeCounter::sharedTimeCounter()->doCount();//要放在最后，以便touchSequence时间与本帧时间一致
}


- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    assert(self);
    //----框架
    [self setupLayer];
    [self setupContext];
    [self setupDepthBuffer];
    [self setupRenderBuffer];
    [self setupFrameBuffer];
    [self setupDisplayLink];//其中定义了render回调
    [self initOpenAL];
    //----随机数
    srand(time(0));
    //----openal状态
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);//设置距离模型
    //----opengl状态
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);//指定清理color buffers时所用的颜色，默认值为(0,0,0,0)
                                              //见:http://msdn.microsoft.com/en-us/library/windows/desktop/dd318377(v=vs.85).aspx
    glEnableVertexAttribArray(ATTRIB_LOC_position_local);
    glEnableVertexAttribArray(ATTRIB_LOC_texCoord);
    glEnableVertexAttribArray(ATTRIB_LOC_normal_local);
    glEnableVertexAttribArray(ATTRIB_LOC_texCoord2);
    //----查询版本信息
    const GLubyte* s= glGetString(GL_SHADING_LANGUAGE_VERSION);
    cout<<s<<endl;
     //----获取设备信息
    cout<<"screenSize: "<<Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x()<<" "<<Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y()<<endl;
    cout<<"resolution: "<<Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x()<<" "<<Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y()<<endl;
    //----创建默认shaderProgam
    c3dDefaultShadersSetup();
   
    C3DCHECK_GL_ERROR_DEBUG();
    C3DCHECK_AL_ERROR_DEBUG();
    
    initGame();
    
    C3DCHECK_GL_ERROR_DEBUG();
    C3DCHECK_AL_ERROR_DEBUG();
    return self;
}
-(BOOL) CheckForExtension:(NSString *)searchName
{
    // For performance, the array can be created once and cached.
    NSString *extensionsString = [NSString stringWithCString:(const char*)glGetString(GL_EXTENSIONS) encoding: NSASCIIStringEncoding];
    NSArray *extensionsNames = [extensionsString componentsSeparatedByString:@" "];
    return [extensionsNames containsObject: searchName];
}
- (void)dealloc
{
    
    [self cleanUpOpenAL];
    [_context release];
    _context = nil;
    [super dealloc];
}
@end