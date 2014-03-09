//
//  mesh.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#include "c3dMesh.h"
#include "c3dMatrix.h"
#include "c3dVector.h"

void Cc3dMesh::submit(GLenum usage){
    this->m_indexVBO->submitVertex(this->getMeshData()->vlist, usage);
    this->m_indexVBO->submitIndex(this->getMeshData()->IDtriList , usage);
}
void Cc3dMesh::submitVertex(GLenum usage){
    this->m_indexVBO->submitVertex(this->getMeshData()->vlist, usage);
}
void Cc3dMesh::submitIndex(GLenum usage){
    this->m_indexVBO->submitIndex(this->getMeshData()->IDtriList , usage);
}
void Cc3dMesh::clearMeshData(){
    m_meshData->clear();
}
void Cc3dMesh::addVertex(const float pos[3],const float texCoord[2],const float norm[3]){
    m_meshData->addVertex(pos, texCoord, norm);
}
void Cc3dMesh::addIDtri(int ID0,int ID1,int ID2){
    m_meshData->addIDtri(ID0, ID1, ID2);
}