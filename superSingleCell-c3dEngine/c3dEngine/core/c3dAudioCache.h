//
//  c3dAudioCache.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-2-8.
//
//

#ifndef __HelloOpenGL__c3dAudioCache__
#define __HelloOpenGL__c3dAudioCache__

#include <iostream>
#include <vector>
using namespace std;

#include "c3dALSource.h"


#include "c3dCommonFunc.h"




class Cc3dAudioCache
{
protected:
    vector<Cc3dALSource*> pSourceList;
    vector<Cc3dALBuffer*> pBufferList;
    bool isRemoveUnusedBuffersAndSourcesOnNextFrame;
public:
    static Cc3dAudioCache*sharedAudioCache();
    Cc3dAudioCache(){
        isRemoveUnusedBuffersAndSourcesOnNextFrame=false;
    }
    virtual ~Cc3dAudioCache(){
        //----注意，一定要先释放source再释放buffer，否则出现al_invalid_operation错误
        //销毁source
        int nSource=(int)pSourceList.size();
        for(int i=0;i<nSource;i++){
            pSourceList[i]->release();
        }
        //销毁buffer
        int nBuffer=(int)pBufferList.size();
        for(int i=0;i<nBuffer;i++){
            pBufferList[i]->release();
        }
        pSourceList.clear();
        pBufferList.clear();
    }
    void removeUnusedBuffersAndSources(){
    //    stopAllPlayingSource();
        int sourceRemovedCount=0;
        //must release sources first and then release buffers
        int nSource=(int)pSourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=pSourceList[i];
            if(source->getRetainCount()==1){
                source->release();
                pSourceList.erase(pSourceList.begin()+i);
                i--;
                nSource--;
                sourceRemovedCount++;
            }
        }
    //    cout<<"source remove count:"<<sourceRemovedCount<<endl;
        int nBuffer=(int)pBufferList.size();
        for(int i=0;i<nBuffer;i++){
            Cc3dALBuffer*buffer=pBufferList[i];
            if(buffer->getRetainCount()==1){
                buffer->release();
                pBufferList.erase(pBufferList.begin()+i);
                i--;
                nBuffer--;
            }
        }
    }
    void removeUnusedBuffersAndSourcesOnNextFrame(){
        isRemoveUnusedBuffersAndSourcesOnNextFrame=true;
    }
    void performDelayRemoveUnusedBuffersAndSources(){
        if(isRemoveUnusedBuffersAndSourcesOnNextFrame){
            removeUnusedBuffersAndSources();
            isRemoveUnusedBuffersAndSourcesOnNextFrame=false;
        }
    }
    Cc3dALBuffer* getBufferByKey(const string&key)
    //看_filePathShort对应的buffer是否已经存在，如果存在返回指针，否则返回NULL
    {
        int nBuffer=(int)pBufferList.size();
        for(int i=0;i<nBuffer;i++){
            assert(pBufferList[i]);
            if(pBufferList[i]->getFilePath()==key){
                return pBufferList[i];
            }
        }
        return NULL;
    }
    int getBufferIndex(const Cc3dALBuffer*buffer)const{//return -1 if failed
        assert(buffer);
        int n=(int)pBufferList.size();
        for(int i=0;i<n;i++){
            if(pBufferList[i]==buffer){
                return i;
            }
        }
        return -1;
        
    }
    int getSourceCount()const{return (int)pSourceList.size();}
    int getBufferCount()const{return (int)pBufferList.size();}
    int getSourceIndex(const Cc3dALSource*source)const{//return -1 if failed
        assert(source);
        int n=(int)pSourceList.size();
        for(int i=0;i<n;i++){
            if(pSourceList[i]==source){
                return i;
            }
        }
        return -1;
        
    }
    vector<Cc3dALSource*>getSourcesByBuffer(const Cc3dALBuffer*buffer)
    {
        assert(buffer);
        assert(getBufferIndex(buffer)!=-1);
        vector<Cc3dALSource*> sourceList;
        int nSource=(int)pSourceList.size();
        for(int i=0;i<nSource;i++){
            if(pSourceList[i]->getBuffer()==buffer){
                sourceList.push_back(pSourceList[i]);
            }
        }
        return sourceList;
    }
    Cc3dALSource*getSourceByName(const string&name){
        int nSource=(int)pSourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=pSourceList[i];
            if(source->getName()==name){
                return source;
            }
        }
        return NULL;

    
    }
    void removeBuffer(Cc3dALBuffer*buffer)
    {
        int index=getBufferIndex(buffer);
        assert(index!=-1);
        buffer->release();
        pBufferList.erase(pBufferList.begin()+index);

    }
    void removeSource(Cc3dALSource*source)
    {
        int index=getSourceIndex(source);
        assert(index!=-1);
        source->release();
        pSourceList.erase(pSourceList.begin()+index);
        
        
    }
    
    Cc3dALBuffer *CreateBuffer(const string&_filePathShort);
    Cc3dALSource *CreateSource(Cc3dALBuffer*pBuffer,const string&name,bool isReuse=true);
    Cc3dALSource *CreateBufferAndSource(const string&_filePathShort,const string&name,bool isReuse=true){
        Cc3dALBuffer*pBuffer=CreateBuffer(_filePathShort);
        return CreateSource(pBuffer,name,isReuse);
    }
    void stopAllPlayingSource(){
        int nSource=(int)pSourceList.size();
        for(int i=0;i<nSource;i++){
            if(pSourceList[i]->getIsPlaying()){
                pSourceList[i]->stop();
            }
        }
    }
    void pauseAllPlayingSource(){
        int nSource=(int)pSourceList.size();
        for(int i=0;i<nSource;i++){
            if(pSourceList[i]->getIsPlaying()){
                pSourceList[i]->pause();
            }
        }
    }
    vector<Cc3dALSource*> get_playingSourcePtrList()const {
        vector<Cc3dALSource*> playingSourcePtrList;
        int nSource=(int)pSourceList.size();
        for(int i=0;i<nSource;i++){
            if(pSourceList[i]->getIsPlaying()){
                playingSourcePtrList.push_back(pSourceList[i]);
            }
        }
        return playingSourcePtrList;
    }
    void continuePausedSource(){//继续播放所有暂停的source
        int nSource=(int)pSourceList.size();
        for(int i=0;i<nSource;i++){
            if(pSourceList[i]->getIsPaused()){
                pSourceList[i]->play();
            }
        }
    }
    void print(){
        //print all buffers and related sources
        cout<<"-----------------------------------"<<endl;
        
        int nBuffer=(int)pBufferList.size();
        for(int i=0;i<nBuffer;i++){
            Cc3dALBuffer*buffer=pBufferList[i];
            cout<<"buffer:"<<buffer->getFilePath()<<endl;
            cout<<"related sources: ";
            int nSource=(int)pSourceList.size();
            for(int j=0;j<nSource;j++){
                Cc3dALSource*source=pSourceList[j];
                if(source->getBuffer()==buffer){
                    cout<<source->getName()<<" ";
                }
            }cout<<endl;
        }
        cout<<"total buffer:"<<(int)pBufferList.size()<<endl;
        cout<<"total source:"<<(int)pSourceList.size()<<endl;
        cout<<"-----------------------------------"<<endl;
        
    }
    
    
    
};




#endif /* defined(__HelloOpenGL__c3dAudioCache__) */
