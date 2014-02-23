//
//  c3dFileUtils.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#include "c3dFileUtils.h"
#include "c3dPathFunc.h"
static Cc3dFileUtils*s_fileUtils=NULL;
Cc3dFileUtils*Cc3dFileUtils::sharedFileUtils(){
    if(s_fileUtils==NULL){
        s_fileUtils=new Cc3dFileUtils();
    }
    return s_fileUtils;
}
string Cc3dFileUtils::getFullPath(const string&filePath){
    string fileName;//文件名
    string ext;//后缀
    vector<string> rs=splitStrInTwoByLastDot(filePath);
    fileName=rs[0];
    ext=rs[1];
    NSString *fileName_NS=[[[NSString alloc] initWithUTF8String:fileName.c_str()] autorelease];
    NSString *ext_NS=[[[NSString alloc] initWithUTF8String:ext.c_str()] autorelease];
    NSString* fullPath_NS;
    if((int)ext.size()==0){
        fullPath_NS= [[NSBundle mainBundle] pathForResource:fileName_NS ofType:nil];
    }else{
        fullPath_NS= [[NSBundle mainBundle] pathForResource:fileName_NS ofType:ext_NS];
    }
    assert(fullPath_NS!=nil);
    string fullPath=[fullPath_NS cStringUsingEncoding:NSASCIIStringEncoding];
    return fullPath;
}


bool Cc3dFileUtils::getIsFileOrClipExistUnderPath(const string&path,const string&fileOrClipName)
{
    if(path.empty()){
        assert(fileOrClipName.empty()==false);
        assert(fileOrClipName[(int)fileOrClipName.size()-1]!='/');
        string fileOrClipFullPath=getFullPath(path);
        NSString *fileOrClipFullPath_NS=[[[NSString alloc] initWithUTF8String:fileOrClipFullPath.c_str()] autorelease];
        bool isExist =  [[NSFileManager defaultManager] fileExistsAtPath:fileOrClipFullPath_NS];
        return isExist;
    }else{
        assert(path.empty()==false);
        assert(path[(int)path.size()-1]!='/');
        assert(fileOrClipName.empty()==false);
        assert(fileOrClipName[(int)fileOrClipName.size()-1]!='/');
        string fileOrClipFullPath=getFullPath(path)+"/"+fileOrClipName;
        NSString *fileOrClipFullPath_NS=[[[NSString alloc] initWithUTF8String:fileOrClipFullPath.c_str()] autorelease];
        bool isExist =  [[NSFileManager defaultManager] fileExistsAtPath:fileOrClipFullPath_NS];
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