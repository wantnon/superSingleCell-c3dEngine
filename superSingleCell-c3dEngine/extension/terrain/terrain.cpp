//
//  terrain.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//
#include "geoMath.h"
#include "terrain.h"
#include "c3dFileUtils.h"
inline void Cterrain::readLandMat(){
    int bmpSideLength=pow(2,m_quadtreeDepth-1);
    //landMat开辟空间
    landMat.resize(bmpSideLength);
    for(int i=0;i<(int)landMat.size();i++){
        landMat[i].resize(bmpSideLength);
    }
    //读取高程数据
    const int MAKEUP=(4-(bmpSideLength*3)%4)%4;
    //data和_data只是两个临时中转用的数组，如果申请静态数组，会使栈空间不够用，
    //所以这里改用申请动态空间
    BYTE *data=new BYTE[bmpSideLength*(bmpSideLength*3+MAKEUP)];//位图数据缓冲
    BYTE *_data=new BYTE[bmpSideLength*(bmpSideLength*3)];//提取数据的数组
    //读取位图
    FILE  *infile=NULL;
    //打开文件
    string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(m_heightMapFileName);
    infile=fopen(pathName.c_str(),"rb");//Open 24 bit bitmap file
    assert(infile);
    //读文件头
    BYTE  Head[54];
    fread(Head,54,1,infile);
    //读数据
    int len=bmpSideLength*(bmpSideLength*3+MAKEUP);
    fread(data,len,1,infile);
    fclose(infile);
    //分析文件头
    int bmpWidth =Head[18]+Head[19]*256;
    int bmpHeight=Head[22]+Head[23]*256;
    if(bmpWidth!=bmpSideLength||bmpHeight!=bmpSideLength){
        cout<<"bmpWidth:"<<bmpWidth<<endl;
        cout<<"bmpHeight:"<<bmpHeight<<endl;
        cout<<"bmpSideLength:"<<bmpSideLength<<endl;
        cout<<"配置不匹配!"<<endl;
        exit(0);
    }
    int makeup=(4-(bmpWidth*3)%4)%4;
    if(makeup!=MAKEUP){
        cout<<"配置不匹配!"<<endl;
        exit(0);
    }
    //提取数据
    
    int p=0;
    int _p=0;
    for(int i=0;i<bmpHeight;i++){
        for(int j=0;j<bmpWidth;j++){
            _data[_p++]=data[p++];
            _data[_p++]=data[p++];
            _data[_p++]=data[p++];
        }
        p+=MAKEUP;
    }
    //将_data填充到landMat并放缩
    //注意bmp的通道顺序是bgr
    //另外像素序列是反的
    _p=0;
    for(int i=bmpSideLength-1;i>=0;i--){//倒序
        for(int j=0;j<bmpSideLength;j++){//正序
            float b=_data[_p++];//blue表附加凹陷
            float g=_data[_p++];//g表基础凸起
            float r=_data[_p++];//r表附加凸起
            landMat[i][j]=m_heightScale*(g-b+r);
        }
    }

    delete []_data;
    delete []data;
}
inline void Cterrain::fillNormalMat()//根据高程数据landMat填充法向数据normalMat
{
    //为normalMat开辟空间
    normalMat.resize((int)landMat.size());
    for(int i=0;i<(int)normalMat.size();i++){
        normalMat[i].resize(landMat[i].size());
    }
    //根据landMat填充normalMat
    float dw=m_range.getSpanX()/normalMat[0].size();
    float dh=m_range.getSpanZ()/normalMat.size();
    for(int i=0;i<(int)normalMat.size();i++){
        for(int j=0;j<(int)normalMat[i].size();j++){
            //填充normalMatMat[I][J][i][j]
            float hct=landMat[i][j];//本格高度(中心高度)
            float hrt;//右格高度
            float hdn;//下格高度
            if(j+1<(int)normalMat[i].size()){//j+1不越界
                hrt=landMat[i][j+1];
            }else{//j+1越界
                normalMat[i][j].init(0,1,0,0);
                continue;
            }
            if(i+1<(int)normalMat.size()){//i+1不越界
                hdn=landMat[i+1][j];
            }else{//i+1越界
                normalMat[i][j].init(0,1,0,0);
                continue;
            }
            //      (0,hct,0)
            //         *-------> (w,hrt,0)
            //    vec1 |   vec2
            //        \|/
            //     (0,hdn,h)
            Cc3dVector4 vec1(0,hdn-hct,dh,0);
            Cc3dVector4 vec2(dw,hrt-hct,0,0);
            Cc3dVector4 norm=cross(vec1, vec2);
            norm=normalize(norm);
            normalMat[i][j]=norm;
        }
    }
}
bool Cterrain::init(const string&heightMapFileName,const Cc3dRect&rect,float heightScale,int quadtreeDepth,
                   Cc3dTexture*texture){
    assert(rect.getWidth()==rect.getHeight());
    m_heightScale=heightScale;
    m_quadtreeDepth=quadtreeDepth;
    m_heightMapFileName=heightMapFileName;
    //生成model
    Cc3dModel*model=new Cc3dModel();
    model->init();
    model->autorelease();
    addModel(model);
    //读取高程数据
    readLandMat();
    //求高度范围
    float Hmin=c3d_INF;
    float Hmax=-c3d_INF;
    for(int i=0;i<(int)landMat.size();i++){//倒序
        for(int j=0;j<(int)landMat[i].size();j++){//正序
            if(landMat[i][j]>Hmax){
                Hmax=landMat[i][j];
            }
            if(landMat[i][j]<Hmin){
                Hmin=landMat[i][j];
            }
        }
    }//得到Hmin和Hmax
    //设置m_range
    m_range.init(rect.getMinX(), rect.getMaxX(), Hmin, Hmax, rect.getMinY(), rect.getMaxY());
    //计算步长
    int markMatSideLength=pow(2,m_quadtreeDepth-1)+1;
    gridSize=m_range.getSpanX()/(markMatSideLength-1);
    //填充法向数据
    fillNormalMat();
    //markmat开辟空间
    markmat.resize(markMatSideLength);
    for(int i=0;i<(int)markmat.size();i++){
        markmat[i].resize(markMatSideLength);
        for(int j=0;j<(int)markmat[i].size();j++){
            markmat[i][j]=0;
        }
    }
    //制作ground mesh
    Cc3dMesh*mesh=new Cc3dMesh();
    mesh->autorelease();
    mesh->init();
    mesh->setTexture(texture);
    mesh->getIndexVBO()->genBuffers();
    this->getModel()->addMesh(mesh);
    //make mesh
    makeMesh();
    
    //xxxx下面要去掉
    
    //申请空间不光要考虑矩形，还要考虑三角形
    //另外注意一个矩形要拆成两个三角形，所以是六个顶点（而非四个）
    //矩形最大数量：BMPHEIGHT*BMPWIDTH
    //矩形分裂出的最大三角形数量：BMPHEIGHT*BMPWIDTH*2
    //补洞三角形最大数量：等于mesh网格的非边缘边个数，等于BMPHEIGHT*(BMPWIDTH-1)+BMPWIDTH*(BMPHEIGHT-1)
    //所以总共有BMPHEIGHT*BMPWIDTH*2+(BMPHEIGHT*(BMPWIDTH-1)+BMPWIDTH*(BMPHEIGHT-1))个三角形
    int bmpHeight=(int)landMat.size();
    int bmpWidth=(int)landMat[0].size();
    this->getModel()->getMeshByIndex(0)->getMeshData()->IDtriList.reserve(bmpHeight*bmpWidth*2+(bmpHeight*(bmpWidth-1)+bmpWidth*(bmpHeight-1)));
    return true;
    
}
void Cterrain::makeMesh(){
    int nVertex=(int)markmat.size()*(int)markmat[0].size();
    int nStep=3+2+3+2;
    float *vertexArray=new float[nVertex*nStep];
    //填充vertexArray
    for(int i=0;i<(int)markmat.size();i++){
        for(int j=0;j<(int)markmat[i].size();j++){
            float x,y,z;
            x=m_range.getMinX()+j*gridSize;
            y=landMat[min(i,(int)landMat.size()-1)][min(j,(int)landMat[0].size()-1)];
            z=m_range.getMinZ()+i*gridSize;
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+0)=x;
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+1)=y;
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+2)=z;
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+3)=m_uvScale*j/(int)markmat[0].size();
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+4)=m_uvScale*i/(int)markmat.size();
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+5)=normalMat[min(i,(int)landMat.size()-1)][min(j,(int)landMat[0].size()-1)].x();
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+6)=normalMat[min(i,(int)landMat.size()-1)][min(j,(int)landMat[0].size()-1)].y();
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+7)=normalMat[min(i,(int)landMat.size()-1)][min(j,(int)landMat[0].size()-1)].z();
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+8)=(x-m_range.getMinX())/m_range.getSpanX();//alpha map texCoord s
            *(vertexArray+i*(int)markmat[0].size()*(3+2+3+2)+j*(3+2+3+2)+9)=(z-m_range.getMinZ())/m_range.getSpanZ();//alpha map texCoord t
        }
    }
    this->getModel()->getMeshByIndex(0)->getMeshData()->vlist.clear();
    for(int i=0;i<nVertex;i++){
        Cc3dVertex vertex;
        vertex.setPos(vertexArray+i*nStep);
        vertex.setTexCoord(vertexArray+i*nStep+3);
        vertex.setNorm(vertexArray+i*nStep+3+2);
        vertex.setTexCoord2(vertexArray+i*nStep+3+2+3);
        this->getModel()->getMeshByIndex(0)->getMeshData()->vlist.push_back(vertex);
    }
    //释放vertexArray
    delete []vertexArray;
}
void Cterrain::makeUp(int jmin,int jmax,int imin,int imax)
{
    if(imin+1==imax){//是原子格，不用修补
        return;
    }else{
        
        //本区域中心
        int imid=(imin+imax)>>1;//除2
        int jmid=(jmin+jmax)>>1;
        
        if(markmat[imid][jmid]){//如果区域中心mark，即本区域要细分
            
            //向下走
            //对四个孩子继续递归
            makeUp(jmin,jmid,imin,imid);
            makeUp(jmin,jmid,imid,imax);
            makeUp(jmid,jmax,imid,imax);
            makeUp(jmid,jmax,imin,imid);
            
        }else{
            
            //看上方同尺寸区域中心是否mark
            {
                int _imid=imid-(imax-imin);
                int _jmid=jmid;
                if(_imid>=0&&markmat[_imid][_jmid]){//上方同尺寸区域中心mark
                    
                    //用对三角面补漏
                    //     p[2]
                    //    /   \
                    //  p[0]--p[1]
                    
                    //添加到mesh
                    this->getModel()->getMeshByIndex(0)->addIDtri(imin*(int)markmat[0].size()+jmin,
                                                 imin*(int)markmat[0].size()+jmax,
                                                 imin*(int)markmat[0].size()+jmid);
                }
            }
            //看下方同尺寸区域中心是否mark
            {
                int _imid=imid+(imax-imin);
                int _jmid=jmid;
                if(_imid<(int)markmat.size()&&markmat[_imid][_jmid]){//下方同尺寸区域中心mark
                    
                    //用对三角面补漏
                    //  p[0]--p[2]
                    //    \    /
                    //     p[1]
                    this->getModel()->getMeshByIndex(0)->addIDtri(imax*(int)markmat[0].size()+jmin,
                                                 imax*(int)markmat[0].size()+jmid,
                                                 imax*(int)markmat[0].size()+jmax);
                    
                }
            }
            //看左方同尺寸区域中心是否mark
            {
                int _imid=imid;
                int _jmid=jmid-(jmax-jmin);
                if(_jmid>=0&&markmat[_imid][_jmid]){//左方同尺寸区域中心mark
                    
                    //用对三角面补漏
                    //        p[0]
                    //      /  |
                    //  p[1]   |
                    //      \  |
                    //        p[2]
                    this->getModel()->getMeshByIndex(0)->addIDtri(imin*(int)markmat[0].size()+jmin,
                                                 imid*(int)markmat[0].size()+jmin,
                                                 imax*(int)markmat[0].size()+jmin);
                }
            }
            //看右方同尺寸区域中心是否mark
            {
                int _imid=imid;
                int _jmid=jmid+(jmax-jmin);
                if(_jmid<(int)markmat[0].size()&&markmat[_imid][_jmid]){//右方同尺寸区域中心mark
                    
                    //用对三角面补漏
                    //        p[0]
                    //         |  \
                    //         |   p[2]
                    //         |  /
                    //        p[1]
                    this->getModel()->getMeshByIndex(0)->addIDtri(imin*(int)markmat[0].size()+jmax,
                                                 imax*(int)markmat[0].size()+jmax,
                                                 imid*(int)markmat[0].size()+jmax);
                }
            }
            
        }
    }
    
}

void Cterrain::showAndMark(int jmin,int jmax,int imin,int imax,int curDepth)
{
    //检查当前节点是否与视截体相交
    //求节点p所表示区域的保守包围盒
    //    上面
    //    p[0]--p[3]
    //     |     |
    //    p[1]--p[2]
    //    下面
    //    p[4]--p[7]
    //     |     |
    //    p[5]--p[6]
    float xmin=m_range.getMinX()+gridSize*jmin;
    float xmax=m_range.getMinX()+gridSize*jmax;
    float zmin=m_range.getMinZ()+gridSize*imin;
    float zmax=m_range.getMinZ()+gridSize*imax;
    float ymin=m_range.getMinY();
    float ymax=m_range.getMaxY();
    float c[3]={(xmax+xmin)/2,(ymin+ymax)/2,(zmin+zmax)/2};
    float r=max(xmax-xmin,ymax-ymin)*0.86602540378443864676372317075294;//由于zmax-zmin与xmax-xmin相等，所以不用考虑
    //看球体(c,r)是否都在planeList中某个面的反面，如果是则可剔除
    bool visible=true;
    for(int i=0;i<5;i++){//不考虑远平面
        const Cplane&plane=this->getModel()->getMeshByIndex(0)->getCamera()->getFrustum().getPlaneByIndex(i);
        //看球体(c,r)是否在plane的背面
        float PND=PND_point_plane(plane, c);
        if(PND<-r){//如果在背面
            //断定为不可见，不用再继续检测
            visible=false;
            break;
        }
    }//得到visible
    if(visible){//如果可见
        bool needDiv=false;//是否需要再分
        //求needDiv
        if(imin+1==imax){//无须再分，因为已经无法再分
            needDiv=false;
        }else{//进一步判断
            //求c到视点的距离
            //指数值越大，LOD效应越明显
            float d=square(this->getModel()->getMeshByIndex(0)->getCamera()->getEyePos().x()-c[0])
            +square(minf(0.6*(this->getModel()->getMeshByIndex(0)->getCamera()->getEyePos().y()-c[1]),700))//Y乘以一个比1小的系数是为了让LOD对高度变化不敏感
            +square(this->getModel()->getMeshByIndex(0)->getCamera()->getEyePos().z()-c[2]);
            float e=xmax-xmin;//边长
            if(d<e*reso)needDiv=true;
        }//得到needDiv
        if(needDiv){//继续分
            int imid=(imin+imax)>>1;//除2
            int jmid=(jmin+jmax)>>1;
            markmat[imid][jmid]=true;
            markedElementIndexList.push_back(Cij(imid,jmid));
            //对四个孩子继续递归
            showAndMark(jmin,jmid,imin,imid,curDepth+1);
            showAndMark(jmin,jmid,imid,imax,curDepth+1);
            showAndMark(jmid,jmax,imid,imax,curDepth+1);
            showAndMark(jmid,jmax,imin,imid,curDepth+1);
        }else{//不分
            const int vIDMat_imin=(int)markmat[0].size()*imin;//vIDMat[imin];
            const int vIDMat_imax=(int)markmat[0].size()*imax;//vIDMat[imax];
            const int ID0=vIDMat_imin+jmin;
            const int ID1=vIDMat_imax+jmin;
            const int ID2=vIDMat_imax+jmax;
            const int ID3=vIDMat_imin+jmax;
            this->getModel()->getMeshByIndex(0)->addIDtri(ID0, ID1, ID2);
            this->getModel()->getMeshByIndex(0)->addIDtri(ID0, ID2, ID3);
        }
    }
}


void Cterrain::update(const Cc3dCamera&camera){

    //清除四叉树上的标记
    int nMarked=(int)markedElementIndexList.size();
    for(int i=0;i<nMarked;i++){
        const Cij&index=markedElementIndexList[i];
        markmat[index.getI()][index.getJ()]=0;
    }
    //清空markedElementIndexList
    markedElementIndexList.clear();
    //清空model_ground的indexList
    this->getModel()->getMeshByIndex(0)->getMeshData()->IDtriList.clear();
    //渲染四叉树并对节点的分割情况作标记
    showAndMark(0,(int)markmat.size()-1,0,(int)markmat[0].size()-1,1);//进入第一层（根节点规定为第一层）
    //修补缝隙
    makeUp(0,(int)markmat.size()-1,0,(int)markmat[0].size()-1);
}
float Cterrain::getHAndNormal(float x,float z,Cc3dVector4&norm)const{
    int j=(x-m_range.getMinX())/gridSize+0.5;//加0.5为实现四舍五入
    int i=(z-m_range.getMinZ())/gridSize+0.5;
    if(i>=0&&j>=0&&i<(int)landMat.size()&&j<(int)landMat[0].size()){
        norm=normalMat[i][j];
        return landMat[i][j];
    }else{
        norm.init(0,1,0,0);
        return -c3d_INF;
    }
}
float Cterrain::getH(float x,float z)const {

    int j=(x-m_range.getMinX())/gridSize+0.5;//加0.5为实现四舍五入
    int i=(z-m_range.getMinZ())/gridSize+0.5;
    if(i>=0&&j>=0&&i<(int)landMat.size()&&j<(int)landMat[0].size()){
        return landMat[i][j];
    }else{
        return -c3d_INF;
    }
    
}
