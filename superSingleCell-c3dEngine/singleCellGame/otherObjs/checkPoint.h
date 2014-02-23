//
//  checkPoint.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-3-23.
//
//

#ifndef __HelloOpenGL__checkPoint__
#define __HelloOpenGL__checkPoint__

#include <iostream>
#include "bossCell.h"
#include "bubble.h"
#include "c3dAudioCache.h"
class CcheckPoint
{
protected:
    bool isEmpty;
    Cc3dTransform m_transform;
public:
    
    vector<Cc3dALSource*> playingALSourcePtrList;//正在播放的alsource指针列表
    CcheckPoint(){
        isEmpty=true;
    }
    virtual~CcheckPoint(){}
    void reSetToEmpty(){
        isEmpty=true;
    }
    bool getIsEmpty()const {
        return isEmpty;
    }
    void save(const CbossCell &_cell){
        isEmpty=false;
        m_transform=_cell.getTransform();
        playingALSourcePtrList=Cc3dAudioCache::sharedAudioCache()->get_playingSourcePtrList();
    }
    void recover(CbossCell &_cell)const {
        _cell.setTransform(m_transform);
        //重启playingALSourcePtrList中各source
        int nSource=(int)playingALSourcePtrList.size();
        for(int i=0;i<nSource;i++){
            playingALSourcePtrList[i]->play();
        }
        
    }
    
};
#endif /* defined(__HelloOpenGL__checkPoint__) */
