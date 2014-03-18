//
//  loadInfo.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#ifndef __HelloOpenGL__loadInfo__
#define __HelloOpenGL__loadInfo__

#include <iostream>
using namespace std;
#include "c3dLight.h"
#include "c3dMesh.h"
#include "c3dModel.h"
void loadInfo_light(Cc3dLight*light,const string&fileNameWithExt);
void loadInfo_mesh(Cc3dMesh*mesh,const string&meshFilePathShort,const string&texClipPathShort,float scale=1,Cc3dVector4 offsetVec=Cc3dVector4());
void loadInfo_model(Cc3dModel*model,const string&modelClipPathShort,float scale=1,Cc3dVector4 offsetVec=Cc3dVector4());
#endif /* defined(__HelloOpenGL__loadInfo__) */
