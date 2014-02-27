//
//  descPage2.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "descPage2.h"
#include "globalVars.h"
#include "descPage3.h"
#include "descPage.h"
#include "c3dSceneManger.h"
#include "define.h"
#include "c3dDeviceAndOSInfo.h"
#include "c3dGestureAnalyzer.h"
#include "passUnifoCallBack.h"
bool CdescPage2::init(){
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    //--quad_descPage2_backGround
    {
        m_quad_descPage2_backGround=new Ckey();
        m_quad_descPage2_backGround->autorelease();
        //
        string imagePathShort="data/global/tex/pageTexs/descPage2Tex/backGround_";
        imagePathShort+=resoStr+".png";
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imagePathShort);
        const float texW=ptex->getWidth();
        const float texH=ptex->getHeight();
        float bx=0;
        float by=0;
        m_quad_descPage2_backGround->genAndInit(bx, bx+width, by, by+height,0,wReso/texW,1-hReso/texH,1);
        m_quad_descPage2_backGround->getModelList()[0]->getMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_descPage2_backGround->getModelList()[0]->getMeshByIndex(0)->setTexture(ptex);
        m_quad_descPage2_backGround->setKeyUpColor(1, 1, 1, 1);
        m_quad_descPage2_backGround->useKeyUpColor();
        m_quad_descPage2_backGround->setCamera(camera2D);
        m_quad_descPage2_backGround->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_quad_descPage2_backGround->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_descPage2_backGround->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        m_quad_descPage2_backGround->reSetLeftDownCorner(0, 0);
        m_quad_descPage2_backGround->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_descPage2_backGround);
    }
    //----foeKey
    {
        m_foeKey=new Ckey();
        m_foeKey->autorelease();
        //
        Cc3dTexture* ptexture=Cc3dTextureCache::sharedTextureCache()->addImage("data/global/tex/keys/key_foe.png");
        float c[2]={0,0};
        m_foeKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key/2,c[1]+r_key/2, 0,1,0,1);
        m_foeKey->getModelList()[0]->getMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_foeKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_foeKey->setKeyUpColor(1, 1, 1, 1);
        m_foeKey->useKeyUpColor();
        m_foeKey->getModelList()[0]->getMeshByIndex(0)->setTexture(ptexture);
        m_foeKey->setCamera(camera2D);
        m_foeKey->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_foeKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_foeKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={width/2-50,20};
        m_foeKey->reSetCenter(center[0], center[1]);
        m_foeKey->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_foeKey);
        
    }
    //--nextKey
    {
        m_nextKey=new Ckey();
        m_nextKey->autorelease();
        //
        Cc3dTexture* ptexture_nextKey=Cc3dTextureCache::sharedTextureCache()->addImage("data/global/tex/keys/key_next.png");
        float c[2]={0,0};
        m_nextKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key/2,c[1]+r_key/2, 0,1,0,1);
        m_nextKey->getModelList()[0]->getMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_nextKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_nextKey->setKeyUpColor(1, 1, 1, 1);
        m_nextKey->useKeyUpColor();
        m_nextKey->getModelList()[0]->getMeshByIndex(0)->setTexture(ptexture_nextKey);
        m_nextKey->setCamera(camera2D);
        m_nextKey->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_nextKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_nextKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={width/2+50,20};
        m_nextKey->reSetCenter(center[0], center[1]);
        m_nextKey->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_nextKey);
    }
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->CreateBufferAndSource("data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    return true;
}
void CdescPage2::update(){

    //----foeKey
    {
        //foeKey响应单击
        if(m_foeKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            CdescPage*descPage=new CdescPage();
            descPage->autorelease();
            descPage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(descPage);
        }
        //foeKey响应按下和抬起
        if(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_foeKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_foeKey->useKeyDownColor();
        }else{
            m_foeKey->useKeyUpColor();
        }
    }
    //----nextKey
    //nextKey响应单击
    if(m_nextKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
        this->getALSourceByName("switchPage")->play();
        CdescPage3*descPage3=new CdescPage3();
        descPage3->autorelease();
        descPage3->init();
        Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
        Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(descPage3);
    }
    //nextKey响应按下和抬起
    if(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_nextKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
        m_nextKey->useKeyDownColor();
    }else{
        m_nextKey->useKeyUpColor();
    }

}
CdescPage2::CdescPage2(){

}
CdescPage2::~CdescPage2(){
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}


