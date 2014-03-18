

#include "c3dModel.h"
//#include "loadInfo.h"
 
Cc3dRange Cc3dModel::getRange()const {
    float xmin=c3d_INF;
    float xmax=-c3d_INF;
    float ymin=c3d_INF;
    float ymax=-c3d_INF;
    float zmin=c3d_INF;
    float zmax=-c3d_INF;
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        int nv=(int)m_meshList[i]->getMeshData()->vlist.size();
        for(int j=0;j<nv;j++){
            const Cc3dVertex&v=m_meshList[i]->getMeshData()->vlist[j];
            const Cc3dVector3&pos=v.getPos();
            if(pos.x()<xmin)xmin=pos.x();
            if(pos.x()>xmax)xmax=pos.x();
            if(pos.y()<ymin)ymin=pos.y();
            if(pos.y()>ymax)ymax=pos.y();
            if(pos.z()<zmin)zmin=pos.z();
            if(pos.z()>zmax)zmax=pos.z();
        }
    }//得到xmin,xmax,ymin,ymax,zmin,zmax
    Cc3dRange range;
    range.init(xmin, xmax, ymin, ymax, zmin, zmax);
    return range;
}
void Cc3dModel::scale_modifyVertexDirectly(float kx,float ky,float kz){//以直接修改顶点的方式进行放缩
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        int nv=(int)m_meshList[i]->getMeshData()->vlist.size();
        for(int j=0;j<nv;j++){
            Cc3dVertex&v=m_meshList[i]->getMeshData()->vlist[j];
            const Cc3dVector3&pos=v.getPos();
            v.setPos(Cc3dVector3(pos.x()*kx, pos.y()*ky, pos.z()*kz));
        }
    }
}


void Cc3dModel::genVBOBuffers(){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        pmesh->getIndexVBO()->genBuffers();
    }
}void Cc3dModel::setColor(float r,float g,float b,float a){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh==NULL)continue;
        pmesh->getMaterial()->setColor(r, g, b, a);
    }
    
}

void Cc3dModel::setDiffuseAlpha(float alpha){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh==NULL)continue;
        pmesh->getMaterial()->setDiffuseAlpha(alpha);
    }
}
void Cc3dModel::setDiffuseRGB(float r,float g,float b){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh==NULL)continue;
        pmesh->getMaterial()->setDiffuseRGB(r, g, b);
    }
}
void Cc3dModel::setAmbient(float r,float g,float b){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh==NULL)continue;
        pmesh->getMaterial()->setAmbient(r, g, b);
    }
    
}
void Cc3dModel::setSpecular(float r,float g,float b){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh==NULL)continue;
        pmesh->getMaterial()->setSpecular(r, g, b);
    }
    
}
void Cc3dModel::setShininess(float shininess){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh==NULL)continue;
        pmesh->getMaterial()->setShininess(shininess);
    }
}
void Cc3dModel::setIsHighlightUntransp(bool isHighlightUntransp){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh==NULL)continue;
        pmesh->getMaterial()->setIsHighlightUntransp(isHighlightUntransp);
    }
}
void Cc3dModel::submit(GLenum usage)//提交model数据
{
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh)pmesh->submit(usage);
    }
}
void Cc3dModel::submitVertex(GLenum usage){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh)pmesh->submitVertex(usage);
    }
}
void Cc3dModel::submitIndex(GLenum usage){
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*pmesh=m_meshList[i];
        if(pmesh)pmesh->submitIndex(usage);
    }
}
void Cc3dModel::addMesh(Cc3dMesh*mesh){
    assert(mesh);
    m_meshList.push_back(mesh);
    //mesh->setName("?");
    this->addChild(mesh);
    
}

void Cc3dModel::releaseIDtriList()//释放pmeshList中各mesh的IDtriList
{
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh&mesh=*m_meshList[i];
        vector<Cc3dIDTriangle>(mesh.getMeshData()->IDtriList).swap(mesh.getMeshData()->IDtriList);
    }
    
    
}
void Cc3dModel::releasevList()//释放pmeshList中各mesh的vlist
{
    int nMesh=(int)m_meshList.size();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh&mesh=*m_meshList[i];
        vector<Cc3dVertex>(mesh.getMeshData()->vlist).swap(mesh.getMeshData()->vlist);
    }
}



