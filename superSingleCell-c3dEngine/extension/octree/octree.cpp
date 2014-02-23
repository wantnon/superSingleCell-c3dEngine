//
//  octree.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-23.
//
//

#include "octree.h"

//-----------------CocNode
void CocNode::clearpIDtriExListWithoutReleaseReferredElements(){
    m_pIDtriExList.clear();
}
void CocNode::clearpIDtriExListWithoutReleaseReferredElements_freeMem(){
    {vector<CIDTriForOctree*>().swap(m_pIDtriExList);}
    assert((int)m_pIDtriExList.size()==0);
}
float CocNode::getBoundingSphereRadius()const{
    float xSpan=m_range.getMaxX()-m_range.getMinX();
    float ySpan=m_range.getMaxY()-m_range.getMinY();
    float zSpan=m_range.getMaxZ()-m_range.getMinZ();
    assert(isEqual(xSpan,ySpan,0.01)&&isEqual(xSpan, zSpan,0.01));
    float R=xSpan*0.86602540378443864;//sqrtf(3)/2
    return R;
}
Cc3dVector4 CocNode::getCenter()const{
    return m_range.getCenter();
}
int CocNode::getIDtriExCount()const {
    return (int)m_pIDtriExList.size();
}
CIDTriForOctree* CocNode::getpIDtriExByIndex(int index)const{
    assert(index>=0&&index<getIDtriExCount());
    return m_pIDtriExList[index];
}
CocNode*CocNode::getChildByIndex(int index)const  {
    assert(index>=0&&index<=7);
    return m_children[index];
}
void CocNode::setChildByIndex(int index,CocNode*node){
    assert(index>=0&&index<=7);
    assert(m_children[index]==NULL);
    m_children[index]=node;
}

void CocNode::addpIDtriEx(CIDTriForOctree*pIDtriEx){
    m_pIDtriExList.push_back(pIDtriEx);
}
void CocNode::setRange(const Cc3dRange&range){
    m_range=range;
    assert(isEqual(m_range.getSpanX(),m_range.getSpanY(),0.01));
    assert(isEqual(m_range.getSpanX(),m_range.getSpanZ(),0.01));
}

//------------------Coctree
Cc3dRange Coctree::getRangeOfIDtris()const{
    return m_range;
    
}


void Coctree::initMembers(){
    m_IDtriCount=0;
    m_leafCount=0;
    m_pRoot=NULL;
}
void Coctree::destory(){
    //销毁pIDtriExList中挂的各IDtriEx
    {
        int nIDtri=(int)m_pIDtriList.size();
        for(int i=0;i<nIDtri;i++){
            delete m_pIDtriList[i];
        }
    }
    //销毁所有节点
    destoryAllNode();//pRoot已置为NULL
    //清空pVisibleLeafList,pVisibleIDtriExList,pCollisionIDtriExList,pCollisionLeafList
    m_pVisibleLeafList.clear();
    m_pVisibleIDtriList.clear();

    //复位为默认初始值（必须放在最后，否则可能破坏其它的销毁工作）
    initMembers();
    
}
void Coctree::destoryAllNode(){
    destoryAllNode_inn(m_pRoot);
    m_pRoot=NULL;
    
}
void Coctree::destoryAllNode_inn(CocNode*pNode){
    if(pNode==NULL)return;
    //先销毁八个孩子
    for(int i=0;i<8;i++){
        destoryAllNode_inn(pNode->getChildByIndex(i));
    }
    //后销毁自己
    delete pNode;
    pNode=NULL;//置为NULL
    
}
void Coctree::updateVisibleIDTriList(const vector<int>&skipTagList)
//显示不透明面
{
    m_pVisibleIDtriList.clear();//清空可见三角形列表
    updateVisibleNodeList();//获得与视锥相交的叶子列表
    //由pVisibleLeafList生成pVisibleIDTriExList
    int nVL=m_pVisibleLeafList.size();
    for(int i=0;i<nVL;i++){
        CocNode*pOcNode=m_pVisibleLeafList[i];
        int nIDtriEx=pOcNode->getIDtriExCount();
        for(int j=0;j<nIDtriEx;j++){
            CIDTriForOctree*pIDtriEx=pOcNode->getpIDtriExByIndex(j);
            int index=findFirst(skipTagList, this->getModel()->getMeshByIndex(pIDtriEx->getMeshID())->getTag());
            if(index>=0)continue;//if found, continue
            if(pIDtriEx->getIsAdded())continue;
            m_pVisibleIDtriList.push_back(pIDtriEx);
            pIDtriEx->setIsAdded(true);
        }
    }//得到pVisibleIDtriExList
    //将pVisibleIDtriExList中三角形的added复位
    int nVisibleIDtriEx=m_pVisibleIDtriList.size();
    for(int i=0;i<nVisibleIDtriEx;i++){
        m_pVisibleIDtriList[i]->setIsAdded(false);
    }
    //       cout<<"visible leaf:"<<octree.pVisibleLeafList.top<<endl;
    //       cout<<"visible IDtri:"<<octree.pVisibleIDtriExList.top<<endl;
}
void Coctree::submitVisibleIDTriList(){

    //画pVisibleIDtriExList
    {
        //----清空各mesh的IDtriList
        int nMesh=(int)this->getModel()->getMeshCount();
        for(int i=0;i<nMesh;i++){
            this->getModel()->getMeshByIndex(i)->getMeshData()->IDtriList.clear();
        }
        //----将octree.pVisibleIDtriExList中的三角形填充到相应mesh.IDtriList中
        int nIDtriEx=m_pVisibleIDtriList.size();
        for(int i=0;i<nIDtriEx;i++){
            CIDTriForOctree*pIDtriEx=m_pVisibleIDtriList[i];
            int meshID=pIDtriEx->getMeshID();
            this->getModel()->getMeshByIndex(meshID)->getMeshData()->IDtriList.push_back(pIDtriEx->getIDtri());
        }
        //----提交各mesh的索引表
        for(int i=0;i<nMesh;i++){
            this->getModel()->getMeshByIndex(i)->getIndexVBO()->submitIndex(this->getModel()->getMeshByIndex(i)->getMeshData()->IDtriList, GL_STREAM_DRAW);
        }
        
    }

}



void Coctree::updateVisibleNodeList()
//获得可见叶子节点列表pVisibleLeafList
{
    m_pVisibleLeafList.clear(); //清空可见叶节点列表
    //获得可见叶子列表
    updateVisibleNodeList_inn(m_pRoot);
}
void Coctree::updateVisibleNodeList_inn(CocNode*pNode){
    if(pNode==NULL)return;
    //检查当前节点是否与视截体相交
    const float *c=pNode->getCenter().getArray();
    const float R=pNode->getBoundingSphereRadius();
    //看c到planeList各面中，是否到某个面的距离为负且绝对值大于R，若是，则pNode不可见
    //否则认为可见
    assert(this->getModel()->getMeshCount()!=0);
    Cc3dCamera*camera=this->getModel()->getMeshByIndex(0)->getCamera();
    assert(camera);
    bool visible=true;
    for(int i=0;i<=4;i++){
        const Cplane&plane=camera->getFrustum().getPlaneByIndex(i);
        //求c到plane的距离
        float PND=PND_point_plane(plane,c);
        if(PND<0&&-PND>R){
            visible=false;
            break;
        }
    }//得到visible
    if(visible){//如果可见
        //如果是叶子节点，则添加到pVsibleLeafList
        if(pNode->getIsLeaf()){
            m_pVisibleLeafList.push_back(pNode);
        }else{//否则继续递归
            for(int i=0;i<8;i++){
                updateVisibleNodeList_inn(pNode->getChildByIndex(i));
            }
        }
        
    }
}


void Coctree::deletepIDtriExListForEachNONLeafNode(CocNode*pNode)
//清除所有非叶节点的三角面列表
{
    if(pNode==NULL)return;//必须加这句，因为我们将空叶子制成了NULL，
    //所以虽然每个节点都必定有八个孩子，
    //但其中有可能为NULL者
    if(pNode->getIsLeaf()){
        return;
    }
    //删除pNode->pIDtriExList
    {
        //注意，对象不能删除，因为它们已挂到叶子节点了
        //销毁指针列表
        pNode->clearpIDtriExListWithoutReleaseReferredElements_freeMem();
    }
    //向下递归
    for(int i=0;i<8;i++){
        deletepIDtriExListForEachNONLeafNode(pNode->getChildByIndex(i));
    }
    
}
int Coctree::getLeafCount(){
    int leafCount=0;
    getLeafCount_inn(m_pRoot,leafCount);
    return leafCount;
}
void Coctree::getLeafCount_inn(CocNode*pNode,int&leafCount){
    if(pNode==NULL)return;
    if(pNode->getIsLeaf()){
        leafCount++;
        
    }
    for(int i=0;i<8;i++){
        getLeafCount_inn(pNode->getChildByIndex(i),leafCount);
    }
}
vector<CIDTriForOctree*> Coctree::getCollisionIDtriList(const Cc3dVector4&c,float R,const vector<int>&skipTagList){
    vector<CIDTriForOctree*> pCollisionIDtriExList;
    vector<CocNode*> pCollisionLeafList=getpCollisionLeafList(c,R);
    //由pCollisionLeafList计算pCollisionIDtriList
    //即将pCollisionLeafList中叶子中的三角面收集起来（其中进行除重）得到pCollisionIDtriList
    int nCollisionLeaf=(int)pCollisionLeafList.size();
    for(int k=0;k<nCollisionLeaf;k++)
    {
        CocNode*pNode=pCollisionLeafList[k];
        //将pNode中的三角面添加到
        int nIDtriEx=pNode->getIDtriExCount();
        for(int i=0;i<nIDtriEx;i++){
            CIDTriForOctree*pIDtriEx=pNode->getpIDtriExByIndex(i);
            int index=findFirst(skipTagList,this->getModel()->getMeshByIndex(pIDtriEx->getMeshID())->getTag());
            if(index>=0)continue;//if found, continue
            if(pIDtriEx->getIsAdded()==false){
                pCollisionIDtriExList.push_back(pIDtriEx);
                pIDtriEx->setIsAdded(true);
            }
        }
    }
    //得到pCollisionIDtriList
    //将pCollisionIDtriList中各三角面的added复位
    {
        int nCollisionIDtri=(int)pCollisionIDtriExList.size();
        for(int i=0;i<nCollisionIDtri;i++){
            pCollisionIDtriExList[i]->setIsAdded(false);
        }
    }
    return pCollisionIDtriExList;

}
vector<CtriangleWithNorm> Coctree::getCollisionTriangleList(const Cc3dVector4&c,float R,const vector<int>&skipTagList){
    vector<CIDTriForOctree*> pCollisionIDtriExList=getCollisionIDtriList(c, R, skipTagList);
    //将pCollisionIDtriList转成triangleWithNormList并返回
    vector<CtriangleWithNorm> triWithNormList;
    {
        int nCollisionIDtri=(int)pCollisionIDtriExList.size();
        for(int i=0;i<nCollisionIDtri;i++){
            CIDTriForOctree*IDtriEx=pCollisionIDtriExList[i];
            Cc3dVertex&v0=this->getModel()->getMeshByIndex(IDtriEx->getMeshID())->getMeshData()->vlist[IDtriEx->getIDtri().vID[0]];
            Cc3dVertex&v1=this->getModel()->getMeshByIndex(IDtriEx->getMeshID())->getMeshData()->vlist[IDtriEx->getIDtri().vID[1]];
            Cc3dVertex&v2=this->getModel()->getMeshByIndex(IDtriEx->getMeshID())->getMeshData()->vlist[IDtriEx->getIDtri().vID[2]];
            Cc3dVector4 pos0=v0.getPos().toV4(1);
            Cc3dVector4 pos1=v1.getPos().toV4(1);
            Cc3dVector4 pos2=v2.getPos().toV4(1);
            Cc3dVector4 norm=IDtriEx->getNorm();
            CtriangleWithNorm triWithNorm(pos0,pos1,pos2,norm);
            triWithNormList.push_back(triWithNorm);
        }
    }
    return triWithNormList;
    
}
vector<CocNode*> Coctree::getpCollisionLeafList(const Cc3dVector4&c, float R){
    vector<CocNode*> pCollisionLeafList;
    getpCollisionLeafList_inn(m_pRoot, c, R,pCollisionLeafList);
    return pCollisionLeafList;
}
void Coctree::getpCollisionLeafList_inn(CocNode*pNode,const Cc3dVector4&c,float R, vector<CocNode*>&pCollisionLeafList)
//获得与c为球心R为半径球体的碰撞叶子节点
{
    if(pNode==NULL)return;
    //检查球体cR是否与pNode碰撞
    bool collision=false;
    {
        const float *c_node=pNode->getCenter().getArray();
        const float R_node=pNode->getBoundingSphereRadius();
        float d2=square(c_node[0]-c.x())+square(c_node[1]-c.y())+square(c_node[2]-c.z());
        if(d2<square(R+R_node)){//相交
            collision=true;
        }
    }//得到collision
    if(collision){//如果碰撞
        if(pNode->getIsLeaf()){
            //将pNode加入到pCollisionLeafList
            pCollisionLeafList.push_back(pNode);
        }else{
            //去考察pNode的孩子
            for(int i=0;i<8;i++){
                getpCollisionLeafList_inn(pNode->getChildByIndex(i),c,R,pCollisionLeafList);
            }
        }
    }
}


int Coctree::getIDtriCount()
//只在各mesh的IDtriList均未被破坏之前可使用
{
    int count=0;
    int nMesh=(int)this->getModel()->getMeshCount();
    for(int i=0;i<nMesh;i++){
        count+=this->getModel()->getMeshByIndex(i)->getMeshData()->IDtriList.size();
    }
    return count;
    
}
Cc3dRange Coctree::getRangeOfIDtrisWithTags(const vector<int>&tagList)const{
    Cc3dRange range;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    vector<CIDTriForOctree*> pIDtriExList_cbTri=getIDtrisWithTags(tagList);//获得cbTri三角形列表
    int ncbTri=(int)pIDtriExList_cbTri.size();
    if(ncbTri==0){
        xmin=0;
        xmax=0;
        ymin=0;
        ymax=0;
        zmin=0;
        zmax=0;
        range.init(xmin, xmax, ymin, ymax, zmin, zmax);
        return range;
    }
    xmin=c3d_INF;
    xmax=-c3d_INF;
    ymin=c3d_INF;
    ymax=-c3d_INF;
    zmin=c3d_INF;
    zmax=-c3d_INF;
    for(int i=0;i<ncbTri;i++){
        CIDTriForOctree*pIDtriEx=pIDtriExList_cbTri[i];
        int meshID=pIDtriEx->getMeshID();
        int vID[3]={pIDtriEx->getIDtri().vID[0],pIDtriEx->getIDtri().vID[1],pIDtriEx->getIDtri().vID[2]};
        Cc3dVector4 v[3];//三角形三个顶点
        v[0]=this->getModel()->getMeshByIndex(meshID)->getMeshData()->vlist[vID[0]].getPos().toV4(1);
        v[1]=this->getModel()->getMeshByIndex(meshID)->getMeshData()->vlist[vID[1]].getPos().toV4(1);
        v[2]=this->getModel()->getMeshByIndex(meshID)->getMeshData()->vlist[vID[2]].getPos().toV4(1);
        for(int j=0;j<3;j++){
            if(v[j].x()<xmin)xmin=v[j].x();
            if(v[j].x()>xmax)xmax=v[j].x();
            if(v[j].y()<ymin)ymin=v[j].y();
            if(v[j].y()>ymax)ymax=v[j].y();
            if(v[j].z()<zmin)zmin=v[j].z();
            if(v[j].z()>zmax)zmax=v[j].z();
            
        }
        
    }//得到xmin,xmax,ymin,ymax,zmin,zmax
    range.init(xmin, xmax, ymin, ymax, zmin, zmax);
    return range;
}

vector<CIDTriForOctree*> Coctree::getIDtrisWithTags(const vector<int>&tagList)const{
    vector<CIDTriForOctree*> pFilteredIDtriExList;
    int nIDtriEx=(int)m_pIDtriList.size();
    for(int i=0;i<nIDtriEx;i++){
        CIDTriForOctree*pIDtriEx=m_pIDtriList[i];
        int index=findFirst(tagList, this->getModel()->getMeshByIndex(pIDtriEx->getMeshID())->getTag());
        if(index>=0){//found
            pFilteredIDtriExList.push_back(pIDtriEx);
        }
    }
    return pFilteredIDtriExList;
}
void Coctree::makeOctree()
//对pmesh建立八叉树
{
    //--------------------建立根节点
    m_pRoot=new CocNode();
    //----求范围（包括cbTri）
    float xmin=c3d_INF;
    float xmax=-c3d_INF;
    float ymin=c3d_INF;
    float ymax=-c3d_INF;
    float zmin=c3d_INF;
    float zmax=-c3d_INF;
    {
        int nMesh=(int)this->getModel()->getMeshCount();
        for(int i=0;i<nMesh;i++){
            Cc3dMesh&mesh=*this->getModel()->getMeshByIndex(i);
            int nv=(int)mesh.getMeshData()->vlist.size();
            for(int j=0;j<nv;j++){
                Cc3dVertex&v=mesh.getMeshData()->vlist[j];
                Cc3dVector3 pos=v.getPos();
                if(pos.x()<xmin)xmin=pos.x();
                if(pos.x()>xmax)xmax=pos.x();
                if(pos.y()<ymin)ymin=pos.y();
                if(pos.y()>ymax)ymax=pos.y();
                if(pos.z()<zmin)zmin=pos.z();
                if(pos.z()>zmax)zmax=pos.z();
            }
        }
    }//得到xmin,xmax,ymin,ymax,zmin,zmax
    float c[3]={(xmin+xmax)/2,(ymin+ymax)/2,(zmin+zmax)/2};//范围中心
    //求最大边长
    float Lx=xmax-xmin;
    float Ly=ymax-ymin;
    float Lz=zmax-zmin;
    float Lmax=max(max(Lx,Ly),Lz);//最大边长
    const float margin=1;//留些余量
    float dmax=Lmax/2+margin;//边心距
    //重新计算xmin,xmax,ymin,ymax,zmin,zmax，使区域为一个正方体
    xmin=c[0]-dmax;
    xmax=c[0]+dmax;
    ymin=c[1]-dmax;
    ymax=c[1]+dmax;
    zmin=c[2]-dmax;
    zmax=c[2]+dmax;//得到正方体
    cout<<"八叉树空间范围"<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<" "<<zmin<<" "<<zmax<<endl;
    m_range.init(xmin, xmax, ymin, ymax, zmin, zmax);
    //------------------建立根节点
    m_pRoot->setRange(Cc3dRange(xmin,xmax,ymin,ymax,zmin,zmax));
    //制作根节点的pIDtriExList
    int nMesh=(int)this->getModel()->getMeshCount();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh&mesh=*this->getModel()->getMeshByIndex(i);
        int nIDtri=(int)mesh.getMeshData()->IDtriList.size();
        for(int j=0;j<nIDtri;j++){
            Cc3dIDTriangle&IDtri=mesh.getMeshData()->IDtriList[j];
            //由IDtri生成pIDtriEx
            CIDTriForOctree*pIDtriEx=new CIDTriForOctree();
            pIDtriEx->setIDtri(IDtri);
            pIDtriEx->setMeshID(i);
            Cc3dVertex&v0=mesh.getMeshData()->vlist[IDtri.vID[0]];
            Cc3dVertex&v1=mesh.getMeshData()->vlist[IDtri.vID[1]];
            Cc3dVertex&v2=mesh.getMeshData()->vlist[IDtri.vID[2]];
            Cc3dVector4 pos0=v0.getPos().toV4(1);
            Cc3dVector4 pos1=v1.getPos().toV4(1);
            Cc3dVector4 pos2=v2.getPos().toV4(1);
            Cc3dVector4 norm=calculateNormOfTri(pos0,pos1,pos2);
            pIDtriEx->setNorm(norm);
            //将pIDtriEx添加到pRoot->pIDtriExList
            m_pRoot->addpIDtriEx(pIDtriEx);
        }
    }
    //制作this->pIDtriExList
    //直接将pRoot->pIDtriExList拷贝过来即可
    this->m_pIDtriList=m_pRoot->getpIDTriExList();
    //------------------建立孩子节点
    makeOctree_inn(m_pRoot,this->getModel()->getMeshList());
    //------------------求八叉树叶子数和三角形数
    m_leafCount=getLeafCount();
    m_IDtriCount=getIDtriCount();
    //------------------清除各非叶节点的三角面列表
    deletepIDtriExListForEachNONLeafNode(m_pRoot);
    //------------------为各列表开辟空间
    m_pVisibleLeafList.reserve(m_leafCount);
    m_pVisibleIDtriList.reserve(m_IDtriCount);
    cout<<"octree leafCount:"<<m_leafCount<<endl;
    cout<<"octree IDtriExCount:"<<m_IDtriCount<<endl;
}

void Coctree::makeOctree_inn(CocNode*&pNode,const vector<Cc3dMesh*>&pmeshList){//为pNode建立八个孩子节点
    //递归停止条件
    //光靠节点内面数限制不足以避免无限分割，所以必须给节点定一个尺寸下限
    if(pNode->getRange().getSpanX()<=60){//on PC, use 30
        pNode->setIsLeaf(true);
        return;
    }
    if(pNode->getIDtriExCount()<=16){//on PC, use 8
        pNode->setIsLeaf(true);
        return;
    }
    //-------------------生成八个孩子节点
    Cc3dRange nodeRange=pNode->getRange();
    float xmin=nodeRange.getMinX();
    float xmax=nodeRange.getMaxX();
    float ymin=nodeRange.getMinY();
    float ymax=nodeRange.getMaxY();
    float zmin=nodeRange.getMinZ();
    float zmax=nodeRange.getMaxZ();
    float xmid=(xmin+xmax)/2;
    float ymid=(ymin+ymax)/2;
    float zmid=(zmin+zmax)/2;
    float L=(xmax-xmin)/2;//pNode表示的正方体边长
    Cc3dRange childRangeArray[8]={
        Cc3dRange(xmin,xmin+L,ymin,ymin+L,zmin,zmin+L),
        Cc3dRange(xmid,xmid+L,ymin,ymin+L,zmin,zmin+L),
        Cc3dRange(xmin,xmin+L,ymid,ymid+L,zmin,zmin+L),
        Cc3dRange(xmid,xmid+L,ymid,ymid+L,zmin,zmin+L),
        Cc3dRange(xmin,xmin+L,ymin,ymin+L,zmid,zmid+L),
        Cc3dRange(xmid,xmid+L,ymin,ymin+L,zmid,zmid+L),
        Cc3dRange(xmin,xmin+L,ymid,ymid+L,zmid,zmid+L),
        Cc3dRange(xmid,xmid+L,ymid,ymid+L,zmid,zmid+L),
    };
    for(int i=0;i<8;i++){
        CocNode*pocNode=new CocNode();
        const Cc3dRange&childRange=childRangeArray[i];
        pocNode->setRange(childRange);
        //判断pNode->pIDtriExList中哪些三角形与pocNode相交
        int nIDtri=pNode->getIDtriExCount();
        for(int j=0;j<nIDtri;j++){
            CIDTriForOctree*pIDtriEx=pNode->getpIDtriExByIndex(j);
            Cc3dVertex&v0=pmeshList[pIDtriEx->getMeshID()]->getMeshData()->vlist[pIDtriEx->getIDtri().vID[0]];
            Cc3dVertex&v1=pmeshList[pIDtriEx->getMeshID()]->getMeshData()->vlist[pIDtriEx->getIDtri().vID[1]];
            Cc3dVertex&v2=pmeshList[pIDtriEx->getMeshID()]->getMeshData()->vlist[pIDtriEx->getIDtri().vID[2]];
            bool intersect=intersect_IDTri_AABBBox(v0.getPos().getArray(),v1.getPos().getArray(),v2.getPos().getArray(),
                                                   childRange.getMinX(),childRange.getMaxX(),
                                                   childRange.getMinY(),childRange.getMaxY(),
                                                   childRange.getMinZ(),childRange.getMaxZ());
            if(intersect){
                pocNode->addpIDtriEx(pIDtriEx);
            }
        }//得到pocNode->pIDtriExList
        //如果pocNode->pIDtriExList为空，则pocNode应删除
        if((int)pocNode->getIDtriExCount()==0){
            delete pocNode;
        }else{//否则
            //将孩子连到父节点上
            pNode->setChildByIndex(i,pocNode);
            //继续递归
            makeOctree_inn(pocNode,pmeshList);
            
        }
    }
}
bool Coctree::intersect_IDTri_AABBBox(const float v0[3],const float v1[3],const float v2[3],
                             float xmin,float xmax,float ymin,float ymax,float zmin,float zmax){//判断三角形是否与AABB盒相交
    //判断三个顶点是否都在AABB盒某一个面的外侧
    //等号要算作相交，这样才能保证不泄漏
    if(v0[0]<xmin&&v1[0]<xmin&&v2[0]<xmin)return false;
    if(v0[0]>xmax&&v1[0]>xmax&&v2[0]>xmax)return false;
    if(v0[1]<ymin&&v1[1]<ymin&&v2[1]<ymin)return false;
    if(v0[1]>ymax&&v1[1]>ymax&&v2[1]>ymax)return false;
    if(v0[2]<zmin&&v1[2]<zmin&&v2[2]<zmin)return false;
    if(v0[2]>zmax&&v1[2]>zmax&&v2[2]>zmax)return false;
    return true;
}