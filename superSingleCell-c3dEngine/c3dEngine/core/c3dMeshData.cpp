//
//  c3dMeshData.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#include "c3dMeshData.h"
void Cc3dMeshData::addVertex(const float pos[3],const float texCoord[2],const float norm[3]){
    Cc3dVertex vertex(pos,texCoord,norm);
    vlist.push_back(vertex);
}
void Cc3dMeshData::addIDtri(int ID0,int ID1,int ID2){
    Cc3dIDTriangle IDtri(ID0,ID1,ID2);
    IDtriList.push_back(IDtri);
}