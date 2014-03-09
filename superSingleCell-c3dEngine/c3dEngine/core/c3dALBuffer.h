//
//  c3dALBuffer.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-23.
//
//

#ifndef __HelloOpenGL__c3dALBuffer__
#define __HelloOpenGL__c3dALBuffer__

#include <iostream>
using namespace std;


#include "c3dALdebug.h"
#include "c3dObject.h"
#include "c3dFileUtils.h"

#include "c3dALPlatformDepend.h"

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
protected:
    ALuint m_buffer;
    void* m_data;//this member is copy from MusicCube sample on apple developer web site
    string m_filePath;
    
};


#endif /* defined(__HelloOpenGL__c3dALBuffer__) */
