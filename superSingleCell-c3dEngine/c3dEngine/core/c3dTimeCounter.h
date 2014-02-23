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
const long maxCount=0.5*std::numeric_limits<long>::max();
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
    void doCount(){//进行计数，增加
        m_count++;
        if(m_count>maxCount)m_count=0;//绕回
    }
};
#endif /* defined(__HelloOpenGL__timeCounter__) */
