//
//  c3dFileUtils.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#include "c3dFileUtils.h"
#include "c3dPathFunc.h"
#include "c3dPathPlatformDepend.h"
#include "c3dAssert.h"
static Cc3dFileUtils*s_fileUtils=NULL;
Cc3dFileUtils*Cc3dFileUtils::sharedFileUtils(){
    if(s_fileUtils==NULL){
        s_fileUtils=new Cc3dFileUtils();
    }
    return s_fileUtils;
}
string Cc3dFileUtils::getFullPath(const string&filePath){
    return getFullPath_iOS(filePath);
}


bool Cc3dFileUtils::getIsFileOrClipExistUnderPath(const string&path,const string&fileOrClipName)
{
    if(path.empty()){
        assert(fileOrClipName.empty()==false);
        assert(fileOrClipName[(int)fileOrClipName.size()-1]!='/');
        string fileOrClipFullPath=getFullPath(path);
        bool isExist=isFileExistsAtPath_iOS(fileOrClipFullPath);
        return isExist;
    }else{
        assert(path.empty()==false);
        assert(path[(int)path.size()-1]!='/');
        assert(fileOrClipName.empty()==false);
        assert(fileOrClipName[(int)fileOrClipName.size()-1]!='/');
        string fileOrClipFullPath=getFullPath(path)+"/"+fileOrClipName;
        bool isExist=isFileExistsAtPath_iOS(fileOrClipFullPath);
        return isExist;
    }
}
bool Cc3dFileUtils::getIsFileOrClipExist(const string&fileOrClipPath)
{
    assert(fileOrClipPath.empty()==false);
    assert(fileOrClipPath[(int)fileOrClipPath.size()-1]!='/');
    string path,fileOrClipName;
    vector<string>rs=splitStrInTwoByLastBar(fileOrClipPath);
    path=rs[0];
    fileOrClipName=rs[1];
    return getIsFileOrClipExistUnderPath(path, fileOrClipName);
    
}