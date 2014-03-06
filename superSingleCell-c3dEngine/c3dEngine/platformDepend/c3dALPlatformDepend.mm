//
//  c3dALPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by ck02-084 on 14-3-6.
//
//

#include "c3dALPlatformDepend.h"
void initBuffer_iOS(const string&fileNameFull,void*&data,ALuint&buffer)
{
    ALenum  error = AL_NO_ERROR;
    ALenum  format=0;
    ALsizei size=0;
    ALsizei freq=0;
    
    // get some audio data from a wave file
    NSString *fileNameFull_NS=[[[NSString alloc] initWithUTF8String:fileNameFull.c_str()] autorelease];
    CFURLRef fileURL = (CFURLRef)[[NSURL fileURLWithPath:fileNameFull_NS] retain];
    if (fileURL)
    {
        data = MyGetOpenALAudioData(fileURL, &size, &format, &freq);
        NSLog(@"audioInfo:%d,%d,%d",size,format,freq);
        CFRelease(fileURL);
        
        if((error = alGetError()) != AL_NO_ERROR) {
            printf("error loading sound: %x\n", error);
            exit(1);
        }
        
        // use the static buffer data API
        alBufferDataStaticProc(buffer, format, data, size, freq);
        
        if((error = alGetError()) != AL_NO_ERROR) {
            printf("error attaching audio to buffer: %x\n", error);
        }
    }
    else
    {
        printf("Could not find file!\n");
        data=NULL;
        assert(false);
    }
}
