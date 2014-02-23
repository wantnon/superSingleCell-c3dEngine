//
//  c3dALBuffer.h
//  HelloOpenGL
//
//  Created by apple on 14-2-23.
//
//

#ifndef __HelloOpenGL__c3dALBuffer__
#define __HelloOpenGL__c3dALBuffer__

#include <iostream>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#include "c3dALdebug.h"
#include "c3dObject.h"
#include "c3dFileUtils.h"
typedef ALvoid	AL_APIENTRY	(*alBufferDataStaticProcPtr) (const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq);
class Cc3dALBuffer:public Cc3dObject
{
public:
    Cc3dALBuffer(){
        m_data=NULL;
        m_buffer=0;
    }
    virtual~Cc3dALBuffer(){
        if(m_buffer!=0){
            alDeleteBuffers(1, &m_buffer);
        }
        if(m_data!=NULL){
            free(m_data);
        }
        C3DCHECK_AL_ERROR_DEBUG();
    }
    bool getIsValidBuffer()const{
        bool isValid=(m_buffer!=0&&alIsBuffer(m_buffer));//seems alIsBuffer(m_buffer)==AL_TRUE include m_buffer!=0, test this later
        return isValid;
    }
    
    void initBuffer(const string&filePath);
    
    ALuint getBuffer()const {
        return m_buffer;
    }
    
    string getFilePath()const{
        return m_filePath;
    }
protected:
    void initBuffer(const string&fileNameFull,void*&_data,ALuint&_buffer);
    ALvoid  alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq);
    void* MyGetOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*	outSampleRate);
protected:
    ALuint m_buffer;
    void* m_data;
    string m_filePath;
    
};


#endif /* defined(__HelloOpenGL__c3dALBuffer__) */
