//
//  c3dTimeCounter.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-19.
//
//

#ifndef __HelloOpenGL__c3dTimeCounter__
#define __HelloOpenGL__c3dTimeCounter__

#include <iostream>
#include <limits>
using namespace std;

#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32) 
    const long maxCount=0.5*(numeric_limits<long>::max)();//see:http://blog.chinaunix.net/uid-17102734-id-2830143.html
#else
    const long maxCount=0.5*(numeric_limits<long>::max());
#endif

class Cc3dTimeCounter
{
protected:
        long m_count;
public:
    static Cc3dTimeCounter*sharedTimeCounter();
    Cc3dTimeCounter(){
        m_count=0;
    }
    virtual~Cc3dTimeCounter(){};
    long getCount()const{
        return m_count;
    }
    void doCount(){
        m_count++;
        if(m_count>maxCount)m_count=0;//wrap
    }
};
#endif /* defined(__HelloOpenGL__timeCounter__) */
