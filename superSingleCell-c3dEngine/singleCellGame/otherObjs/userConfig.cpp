//
//  userConfig.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-1-31.
//
//

#include "userConfig.h"
#include "c3dFileUtils.h"
#include "c3dPathFunc.h"
#include "c3dDocuments.h"
#include "c3dPathPlatformDepend.h"
#include "c3dAssert.h"
static CuserConfig*s_userConfig=NULL;
CuserConfig*CuserConfig::sharedUserConfig(){
    if(s_userConfig==NULL){
        s_userConfig=new CuserConfig();
    }
    return s_userConfig;
}
bool CuserConfig::getIsConfigFileExistInDocuments(const string&filePath){
    string fileFullPath=Cc3dDocuments::sharedDocuments()->getDocumentsPath()+"/"+filePath;
    bool isExist=isFileExistsAtPath_plat(fileFullPath);
    return isExist;

}
void CuserConfig::writeToConfigFileInDocuments(const string&filePath)
//将设置写回文件
{
    string filePathFull=Cc3dDocuments::sharedDocuments()->getDocumentsPath()+"/"+filePath;
    //------------------获得绝对路径
    //打开文件
    FILE   *fp=NULL;
    fp=fopen(filePathFull.c_str(),"w+");
    if(fp==NULL){
        cout<<"Open "<<filePathFull<<" with w+ failed!"<<endl;
        perror("error");
        assert(false);
    }else{
        cout<<"Open "<<filePathFull<<" with w+ succ"<<endl;
    }
    //将mesh输出到文件
    //注意一定要每输出一个字段就输出一个空格
    {
        //输出项数
        int n=(int)unLockList.size();
        fprintf(fp,"%d ",n);//一定要有空格
        //输出各项
        for(int i=0;i<n;i++){
            string title="level";
            title+=numberToStr(i);
            fprintf(fp, "%s ",title.c_str());//一定要有空格
            fprintf(fp,"%d ",(int)unLockList[i]);
            fprintf(fp,"%d ",(int)starNumberList[i]);
        }
    }
    //关闭文件
    fclose(fp);
    
    
}
void CuserConfig::loadConfigFileFromDocuments(const string&fileNameWithExt){
    //------------------获得绝对路径
    string pathName=Cc3dDocuments::sharedDocuments()->getDocumentsPath()+"/"+fileNameWithExt;
    //打开文件
    FILE  *fp=NULL;
    fp=fopen(pathName.c_str(),"r");
    if(fp==NULL)
    {
        cout<<"Open "<<pathName<<" failed!"<<endl;
        assert(false);
    }
    //------------------提取文件信息，并建立列模型列表
    {
        char buffer[1024]={0};
        //提取项目数
        int n;
        fscanf(fp, "%d",&n);
        unLockList.resize(n);
        starNumberList.resize(n);
        //提取n个项目
        for(int i=0;i<n;i++){
            //--title
            fscanf(fp, "%s",buffer);
            //--unlock
            int isUnlocked;
            fscanf(fp, "%d",&isUnlocked);
            unLockList[i]=isUnlocked;
            //--starNumber
            int starNumber;
            fscanf(fp, "%d",&starNumber);
            starNumberList[i]=starNumber;
            
        }
    }
    //关闭文件
    fclose(fp);
}

void CuserConfig::copyConfigFileToDocuments(const string&configFilePathShort){
    //------------------获得绝对路径
    string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(configFilePathShort);
    //打开文件
    FILE  *fp=NULL;
    fp=fopen(pathName.c_str(),"r");
    if(fp==NULL)
    {
        cout<<"Open "<<pathName<<" failed!"<<endl;
        assert(false);
    }
    //----提取文件信息到临时变量
    vector<int> t_unLockList;
    vector<int> t_starNumberList;
    {
        char buffer[1024]={0};
        //提取项目数
        int n;
        fscanf(fp, "%d",&n);
        t_unLockList.resize(n);
        t_starNumberList.resize(n);
        //提取n个项目
        for(int i=0;i<n;i++){
            //--title
            fscanf(fp, "%s",buffer);
            //--unlock
            int isUnlocked;
            fscanf(fp, "%d",&isUnlocked);
            t_unLockList[i]=isUnlocked;
            //--starNumber
            int starNumber;
            fscanf(fp, "%d",&starNumber);
            t_starNumberList[i]=starNumber;
            
        }
    }
    //关闭文件
    fclose(fp);
    //----将临时变量写入到Documents/下的同名文件中
    {
        //提取configFilePathShort的最后一级（文件名）
        string clipPath,fileName;
        vector<string>rs=splitStrInTwoByLastBar(configFilePathShort);
        clipPath=rs[0];
        fileName=rs[1];
        //将fileName写入到Documents
        string pathName=Cc3dDocuments::sharedDocuments()->getDocumentsPath()+"/"+fileName;
        //打开文件
        FILE   *fp=NULL;
        fp=fopen(pathName.c_str(),"w+");
        if(fp==NULL){
            cout<<"Open "<<pathName<<" with w+ failed!"<<endl;
            perror("error");
            assert(false);
        }else{
            cout<<"Open "<<pathName<<" with w+ succ"<<endl;
        }
        //将临时变量输出到文件
        //注意一定要每输出一个字段就输出一个空格
        {
            //输出项数
            int n=(int)t_unLockList.size();
            fprintf(fp,"%d ",n);//一定要有空格
            //输出各项
            for(int i=0;i<n;i++){
                string title="level";
                title+=numberToStr(i);
                fprintf(fp, "%s ",title.c_str());//一定要有空格
                fprintf(fp,"%d ",(int)t_unLockList[i]);
                fprintf(fp,"%d ",(int)t_starNumberList[i]);
            }
        }
        //关闭文件
        fclose(fp);
    }
}
