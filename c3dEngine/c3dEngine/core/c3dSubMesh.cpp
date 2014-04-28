//
//  c3dSubMesh.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#include "c3dSubMesh.h"
#include "c3dMatrix.h"
#include "c3dVector.h"

void Cc3dSubMesh::submit(GLenum usage){
    this->m_indexVBO->submitVertex(this->getSubMeshData()->vlist, usage);
    this->m_indexVBO->submitIndex(this->getSubMeshData()->IDtriList , usage);
}
void Cc3dSubMesh::submitVertex(GLenum usage){
    this->m_indexVBO->submitVertex(this->getSubMeshData()->vlist, usage);
}
void Cc3dSubMesh::submitIndex(GLenum usage){
    this->m_indexVBO->submitIndex(this->getSubMeshData()->IDtriList , usage);
}
void Cc3dSubMesh::clearMeshData(){
    m_subMeshData->clear();
}
void Cc3dSubMesh::addVertex(const float pos[3],const float texCoord[2],const float norm[3]){
    m_subMeshData->addVertex(pos, texCoord, norm);
}
void Cc3dSubMesh::addIDtri(int ID0,int ID1,int ID2){
    m_subMeshData->addIDtri(ID0, ID1, ID2);
}
void Cc3dSubMesh::addIDtri(const Cc3dIDTriangle&IDtri){
	m_subMeshData->addIDtri(IDtri);

}