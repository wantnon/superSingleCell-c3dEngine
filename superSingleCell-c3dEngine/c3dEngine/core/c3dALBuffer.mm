//
//  c3dALBuffer.cpp
//  HelloOpenGL
//
//  Created by apple on 14-2-23.
//
//

#include "c3dALBuffer.h"
void Cc3dALBuffer::initBuffer(const string&filePath){
    m_filePath=filePath;
    string fileNameFull=Cc3dFileUtils::sharedFileUtils()->getFullPath(m_filePath);
    assert(m_buffer==0);
    alGenBuffers(1, &m_buffer);
    C3DCHECK_AL_ERROR_DEBUG();
    initBuffer(fileNameFull,m_data,m_buffer);
}

ALvoid  Cc3dALBuffer::alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq)
{
    static	alBufferDataStaticProcPtr	proc = NULL;
    
    if (proc == NULL) {
        proc = (alBufferDataStaticProcPtr) alcGetProcAddress(NULL, (const ALCchar*) "alBufferDataStatic");
    }
    
    if (proc)
        proc(bid, format, data, size, freq);
    
    return;
}

void* Cc3dALBuffer::MyGetOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*	outSampleRate)
{
    OSStatus						err = noErr;
    UInt64							fileDataSize = 0;
    AudioStreamBasicDescription		theFileFormat;
    UInt32							thePropertySize = sizeof(theFileFormat);
    AudioFileID						afid = 0;
    void*							theData = NULL;
    
    // Open a file with ExtAudioFileOpen()
    err = AudioFileOpenURL(inFileURL, kAudioFileReadPermission, 0, &afid);
    if(err) { printf("MyGetOpenALAudioData: AudioFileOpenURL FAILED, Error = %ld\n", err);
        exit(0);
    }
    
    // Get the audio data format
    err = AudioFileGetProperty(afid, kAudioFilePropertyDataFormat, &thePropertySize, &theFileFormat);
    if(err) { printf("MyGetOpenALAudioData: AudioFileGetProperty(kAudioFileProperty_DataFormat) FAILED, Error = %ld\n", err);
        exit(0);
    }
    
    if (theFileFormat.mChannelsPerFrame > 2)  {
        printf("MyGetOpenALAudioData - Unsupported Format, channel count is greater than stereo\n");
        exit(0);
    }
    
    if ((theFileFormat.mFormatID != kAudioFormatLinearPCM) || (!TestAudioFormatNativeEndian(theFileFormat))) {
        printf("MyGetOpenALAudioData - Unsupported Format, must be little-endian PCM\n");
        exit(0);
    }
    
    if ((theFileFormat.mBitsPerChannel != 8) && (theFileFormat.mBitsPerChannel != 16)) {
        printf("MyGetOpenALAudioData - Unsupported Format, must be 8 or 16 bit PCM\n");
        exit(0);
    }
    
    
    thePropertySize = sizeof(fileDataSize);
    err = AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &thePropertySize, &fileDataSize);
    if(err) { printf("MyGetOpenALAudioData: AudioFileGetProperty(kAudioFilePropertyAudioDataByteCount) FAILED, Error = %ld\n", err);
        exit(0);
    }
    
    // Read all the data into memory
    UInt32		dataSize = fileDataSize;
    theData = malloc(dataSize);
    if (theData)
    {
        AudioFileReadBytes(afid, false, 0, &dataSize, theData);
        if(err == noErr)
        {
            // success
            *outDataSize = (ALsizei)dataSize;
            *outDataFormat = (theFileFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
            *outSampleRate = (ALsizei)theFileFormat.mSampleRate;
        }
        else
        {
            // failure
            free (theData);
            theData = NULL; // make sure to return NULL
            printf("MyGetOpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", err);
            exit(0);
        }
    }
    
    // Dispose the ExtAudioFileRef, it is no longer needed
    if (afid) AudioFileClose(afid);
    return theData;
}


void Cc3dALBuffer::initBuffer(const string&fileNameFull,void*&data,ALuint&buffer)
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
        data = NULL;
    }
}

