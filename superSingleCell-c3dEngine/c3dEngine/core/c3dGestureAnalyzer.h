//
//  c3dGestureAnalyzer.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//

#ifndef __HelloOpenGL__c3dGestureAnalyzer__
#define __HelloOpenGL__c3dGestureAnalyzer__
#include <iostream>
using namespace std;
#include "c3dTimeCounter.h"
#include "c3dVector.h"
#include "c3dTouchSequence.h"
#include "c3dDeviceAndOSInfo.h"

class Cc3dGestureAnalyzer
{

public:
    Cc3dGestureAnalyzer(){
        
    }
    virtual~Cc3dGestureAnalyzer(){
        
    }
    static Cc3dGestureAnalyzer* sharedGestureAnalyzer();
    Cc3dVector2 getPoint()const{//数学坐标系坐标(原点在左下角)
        Cc3dTouch touch=Cc3dTouchSequence::sharedTouchSequence()->getLatestTouches()[0];
        Cc3dVector2 point=Cc3dVector2(touch.getPoint().x(),
                           Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y()-touch.getPoint().y());
        return point;
    }

    Cc3dVector2 getEarlierPoint()const{
        Cc3dTouch touch=Cc3dTouchSequence::sharedTouchSequence()->getLatestTouches()[0];
        long time=touch.getTime();
        long earlierTime=Cc3dTouchSequence::sharedTouchSequence()->getEarlierTime(time);
        vector<Cc3dTouch> earlierTouches=Cc3dTouchSequence::sharedTouchSequence()->getTouchesAtTime(earlierTime);
        assert(earlierTouches.empty()==false);
        Cc3dVector2 point=Cc3dVector2(earlierTouches[0].getPoint().x(),
                                      Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y()-earlierTouches[0].getPoint().y());
        return point;
    }
    Cc3dVector2 getLatestTouchBeganPoint()const {
        Cc3dVector2 point=Cc3dVector2(Cc3dTouchSequence::sharedTouchSequence()->getLatestTouchesWithType(e_c3dTouchBegan)[0].getPoint().x(),
                                      Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y()-Cc3dTouchSequence::sharedTouchSequence()->getLatestTouchesWithType(e_c3dTouchBegan)[0].getPoint().y());
        return point;
    }
    long getLatestTouchBeganTime()const{
        return Cc3dTouchSequence::sharedTouchSequence()->getLatestTouchTypeTime(e_c3dTouchBegan);
    }


    bool getIsTapOnce(){
        //如果当前是抬起，且最近的一次按下距当前的时间较短，则判断为TapOnce
        if(Cc3dTouchSequence::sharedTouchSequence()->getTouchesAtTimeWithType(Cc3dTimeCounter::sharedTimeCounter()->getCount(), e_c3dTouchEnd).empty()==false){
            long latestTouchBeganTime=Cc3dTouchSequence::sharedTouchSequence()->getLatestTouchTypeTime(e_c3dTouchBegan);
            if(Cc3dTimeCounter::sharedTimeCounter()->getCount()-latestTouchBeganTime<10){
                return true;
            }
        }
        return false;
    }
    Cc3dVector2 getTouchMoveSpeed(){
        if(getIsDown()){
            Cc3dVector2 currentTouchPoint=this->getPoint();
            Cc3dVector2 earlierTouchPoint=Cc3dTouchSequence::sharedTouchSequence()->getTouchesAtEarlierTime(Cc3dTimeCounter::sharedTimeCounter()->getCount())[0].getPoint();
            return (currentTouchPoint-earlierTouchPoint)*(1.0/(0.01+Cc3dTimeCounter::sharedTimeCounter()->getCount()-Cc3dTouchSequence::sharedTouchSequence()->getEarlierTime(Cc3dTimeCounter::sharedTimeCounter()->getCount())));
        }else{
            return Cc3dVector2(0, 0);
        }
    }
    bool getIsDown()const {
        //通过比较最近的touchBegan和touchEnd的时间先后来判断是否为按下状态
        Cc3dTouch touchBegan=Cc3dTouchSequence::sharedTouchSequence()->getLatestTouchesWithType(e_c3dTouchBegan)[0];
        Cc3dTouch touchEnd=Cc3dTouchSequence::sharedTouchSequence()->getLatestTouchesWithType(e_c3dTouchEnd)[0];
        if(touchBegan.getTime()<touchEnd.getTime()){
            return false;
        }else{
            return true;
        }
    }

};
#endif /* defined(__HelloOpenGL__c3dGestureAnalyzer__) */
