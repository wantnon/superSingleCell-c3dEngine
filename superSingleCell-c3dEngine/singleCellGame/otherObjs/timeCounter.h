//
//  timeCounter.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-22.
//
//

#ifndef __HelloOpenGL__timeCounter__
#define __HelloOpenGL__timeCounter__

#include <iostream>
const long _maxCount=0.5*std::numeric_limits<long>::max();;
class CtimeCounter
{
public:
    long count;
    CtimeCounter(){
        count=0;
    }
    long getCount()const{
        return count;
    }
    void doCount(){//进行计数，增加
        count++;
        if(count>_maxCount)count=0;//绕回
    }
    void doCount_dec(){//进行计数，减少，count最小减到0
        if(count==0)return;
        count--;
        if(count<0){
            count=0;
        }
    }
    void reSet(){//重置
        count=0;
    }
    void setValue(long _count){
        count=_count;
    }
};

#endif /* defined(__HelloOpenGL__timeCounter__) */
