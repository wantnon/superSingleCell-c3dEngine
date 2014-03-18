//
//  c3dActor.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "c3dActor.h"
void Cc3dActor::addModel(Cc3dModel*pModel){
    assert(pModel);
    m_modelList.push_back(pModel);
  //  pModel->setName("?");
    this->addChild(pModel);
    
}

void Cc3dActor::submitVertex(GLenum usage){
    int nModel=(int)getModelCount();
    for(int i=0;i<nModel;i++){
        Cc3dModel*pmodel=m_modelList[i];
        int nMesh=(int)pmodel->getMeshCount();
        for(int j=0;j<nMesh;j++){
            Cc3dMesh*pmesh=pmodel->getMeshByIndex(j);
            pmesh->getIndexVBO()->submitVertex(pmesh->getMeshData()->vlist, usage);
        }
    }
    
}
void Cc3dActor::submitIndex(GLenum usage){
    int nModel=(int)getModelCount();
    for(int i=0;i<nModel;i++){
        Cc3dModel*pmodel=m_modelList[i];
        int nMesh=(int)pmodel->getMeshCount();
        for(int j=0;j<nMesh;j++){
            Cc3dMesh*pmesh=pmodel->getMeshByIndex(j);
            pmesh->getIndexVBO()->submitIndex(pmesh->getMeshData()->IDtriList, usage);
        }
    }
    
}

