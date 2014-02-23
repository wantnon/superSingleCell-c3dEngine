//
//  bossCell.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-31.
//
//

#include "bossCell.h"
#include "c3dTextureCache.h"
#include "makeTemplate.h"

void CbossCellSet::loadConfig(const string&fileNameWithExt){
    
    //------------------获得绝对路径
    string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(fileNameWithExt);
    //打开文件
    FILE  *fp=NULL;
    fp=fopen(pathName.c_str(),"r");
    if(fp==NULL)
    {
        cout<<"Open "<<pathName<<" failed!"<<endl;
        exit(0);
    }
    //------------------提取文件信息，并建立模型列表
    {
        char buffer[1024]={0};
        //提取项目数
        int n;
        fscanf(fp, "%d",&n);
        //提取项目
        for(int i=0;i<n;i++){
            
            //----cellTemplateID
            int templateID;
            fscanf(fp,"%s",buffer);
            if(buffer[0]=='#'){
                cout<<"error:提前遇到#!"<<endl;
                exit(0);
            }
            fscanf(fp,"%d",&templateID);
            
            CbossCell*pbossCell=makeCellTemplate(templateID);

            //----pos
            float posx,posy,posz;
            fscanf(fp, "%f",&posx);
            fscanf(fp, "%f",&posy);
            fscanf(fp, "%f",&posz);
            pbossCell->getTransformPointer()->setPos(posx,posy,posz);
            if(pbossCell->isFly){
                ((Cship*)pbossCell->get_ship())->setTransform(((Cship*)pbossCell)->getTransform());
                //根据initPos和flyRange_relativeToInitPos计算flyRange
                pbossCell->flyRange=pbossCell->flyRange_relativeToInitPos;
                pbossCell->flyRange.mov(Cc3dVector4(posx,posy,posz,0));
                
            }
            this->addActor(pbossCell);
        }
        fscanf(fp, "%s",buffer);
        assert(buffer[0]=='#');//缺少#或实际条目数比n值大
    }
    //关闭文件
    fclose(fp);
}

