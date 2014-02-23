//
//  c3dDocuments.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#ifndef __HelloOpenGL__c3dDocuments__
#define __HelloOpenGL__c3dDocuments__

#include <iostream>
#include <string>
using namespace std;
class Cc3dDocuments{
public:
    static Cc3dDocuments*sharedDocuments();
    string getDocumentsPath();
    
};
#endif /* defined(__HelloOpenGL__c3dDocuments__) */
