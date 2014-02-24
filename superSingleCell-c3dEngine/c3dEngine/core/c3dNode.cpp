//
//  c3dNode.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-15.
//
//

#include "c3dNode.h"
string c3dDefaultProgramName="shader_texColorOnly";
void c3dDefaultShadersSetup(){
    //
    {
        string programName="shader_texColorOnly";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programName+".vert", programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
    }
    //
    {
        //...
    }
}
void c3dDefaultPassUnifoCallback(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    //   C3DLOG("program name:%s",program->getName().c_str());
    assert(program->getName()==c3dDefaultProgramName);
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=node->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=node->getCamera()->calculateViewMat();
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    program->passUnifoValue1i("Texture", 0);//texture attach point 0
    program->passUnifoValueMatrixNfv("projectionModelview", PVMmat.getArray(), PVMmat.getArrayLen());
}

void Cc3dNode::visitUpdate(){
    if(this->getIsDoUpdateRecursively()){
        if(this->getIsDoUpdate()){
            this->update();
        }
        
        //sort children by their visitDrawOrder
        int nchild=(int)m_childList.size();
        if(nchild!=0){
            //sort
            stable_sort(m_childList.begin(), m_childList.end(),comp_smallervisitUpdateOrder());
            for(int i=0;i<nchild;i++){
                assert(m_childList[i]);
                m_childList[i]->visitUpdate();
            }
            
        }
    }
 

}
void Cc3dNode::visitDraw(){
 //   cout<<"enter node: "<<m_name<<endl;
    if(this->getIsVisibleRecursively()){
        Cc3dModelMatStack::sharedModelMatStack()->pushMatrix();
        if(!m_isIgnorTransform)this->transform();
        
        if(this->getIsVisible()){
            this->draw();
        }
        
        
        //sort children by their visitDrawOrder
        int nchild=(int)m_childList.size();
        if(nchild!=0){
            //sort
            stable_sort(m_childList.begin(), m_childList.end(),comp_smallerVisitDrawOrder());
            for(int i=0;i<nchild;i++){
                assert(m_childList[i]);
                m_childList[i]->visitDraw();
            }
            
        }
        Cc3dModelMatStack::sharedModelMatStack()->popMatrix();
    }
  //  cout<<"quit node: "<<m_name<<endl;
}

