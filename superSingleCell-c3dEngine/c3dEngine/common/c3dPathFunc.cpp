//
//  c3dPathFunc.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//


#include "c3dPathFunc.h"
string getFileNameFromFilePath(string filePath){
    string clipPath,fileName;
    vector<string> rs=splitStrInTwoByLastBar(filePath);
    clipPath=rs[0];
    fileName=rs[1];
    return fileName;
    
}
vector<string> splitStrInTwoByLastDot(const string&str)
//用最后一个.将str划分为两部，part1和prat2，part1和part2都不含有用于切割的.
//如果str中不含.，则part1=str,part2=""
{
    string part1="";
    string part2="";
    //找最后一个"/"的位置
    int i_lastDot=-1;
    int len=(int)str.size();
    for(int i=len-1;i>=0;i--){
        if(str[i]=='.'){
            i_lastDot=i;
            break;
        }
    }
    if(i_lastDot==-1){//未找到
        part1=str;
        part2="";
        vector<string> rs;
        rs.push_back(part1);
        rs.push_back(part2);
        return rs;
    }
    //找到
    //用i_lastDot切割str
    for(int i=0;i<len;i++){
        if(i<i_lastDot){
            part1+=str[i];
        }else if(i>i_lastDot){
            part2+=str[i];
        }//i==i_lastDot无动作
    }//得到part1,part2
    vector<string> rs;
    rs.push_back(part1);
    rs.push_back(part2);
    return rs;
    
    
}

vector<string> splitStrInTwoByLastBar(const string&str)
//用最后一个/将str划分为两部，part1和prat2，part1和part2都不含有用于切割的/
//如果str中不含/，则part1=str,part2=""
{
    string part1="";
    string part2="";
    //找最后一个"/"的位置
    int i_lastBar=-1;
    int len=(int)str.size();
    for(int i=len-1;i>=0;i--){
        if(str[i]=='/'){
            i_lastBar=i;
            break;
        }
    }
    if(i_lastBar==-1){//未找到
        part1=str;
        part2="";
        vector<string> rs;
        rs.push_back(part1);
        rs.push_back(part2);
        return rs;
    }
    //找到
    //用i_lastBar切割str
    for(int i=0;i<len;i++){
        if(i<i_lastBar){
            part1+=str[i];
        }else if(i>i_lastBar){
            part2+=str[i];
        }//i==i_lastBar无动作
    }//得到part1,part2
    vector<string> rs;
    rs.push_back(part1);
    rs.push_back(part2);
    return rs;
    
}


