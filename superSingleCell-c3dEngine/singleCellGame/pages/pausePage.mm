//
//  pausePage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "pausePage.h"
#include "globalVars.h"
#include "homePage.h"
#include "loadingPage.h"
#include "c3dSceneManger.h"
#include "playingPage.h"
#include "define.h"
#include "c3dGestureAnalyzer.h"
bool CpausePage::init(){
    //--quad_pause
    {
        m_quad_pause=new Ckey();
        m_quad_pause->autorelease();
        //
        float bx=0;
        float by=0;
        float w=200;
        float h=50;
        m_quad_pause->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_pause->getModelList()[0]->getMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_pause->getModelList()[0]->getMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("data/global/tex/quads/quad_pause.png"));
        m_quad_pause->setKeyUpColor(1, 1, 1, 1);
        m_quad_pause->useKeyUpColor();
        m_quad_pause->setCamera(camera2D);
        m_quad_pause->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_quad_pause->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_pause->setCallback(passUnifoCallback_noLight);
        m_quad_pause->setVisitDrawOrder(10000);
        //
        float _bx=60;
        float _by=380;
        m_quad_pause->reSetLeftDownCorner(_bx, _by);
        m_quad_pause->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_pause);
    }
    
    //--continueKey
    {
        m_continueKey=new Ckey();
        m_continueKey->autorelease();
        //
        float c[2]={0,0};
        m_continueKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_continueKey->getModelList()[0]->getMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_continueKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_continueKey->setKeyUpColor(1, 1, 1, 1);
        m_continueKey->useKeyUpColor();
        m_continueKey->getModelList()[0]->getMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("data/global/tex/keys/key_continue.png"));
        m_continueKey->setCamera(camera2D);
        m_continueKey->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_continueKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_continueKey->setCallback(passUnifoCallback_noLight);
        m_continueKey->setVisitDrawOrder(10000);
        //
        float center[2]={160,280};
        m_continueKey->reSetCenter(center[0], center[1]);
        m_continueKey->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_continueKey);
        
    }
    //--homeKey
    {
        m_homeKey=new Ckey();
        m_homeKey->autorelease();
        //
        float c[2]={0,0};
        m_homeKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_homeKey->getModelList()[0]->getMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_homeKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_homeKey->setKeyUpColor(1, 1, 1, 1);
        m_homeKey->useKeyUpColor();
        m_homeKey->getModelList()[0]->getMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("data/global/tex/keys/key_home.png"));
        m_homeKey->setCamera(camera2D);
        m_homeKey->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_homeKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_homeKey->setCallback(passUnifoCallback_noLight);
        m_homeKey->setVisitDrawOrder(10000);
        //
        float center[2]={193,340};
        m_homeKey->reSetCenter(center[0], center[1]);
        m_homeKey->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_homeKey);
    }
    //--restartKey
    {
        m_restartKey=new Ckey();
        m_restartKey->autorelease();
        //
        float c[2]={0,0};
        m_restartKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_restartKey->getModelList()[0]->getMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_restartKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_restartKey->setKeyUpColor(1, 1, 1, 1);
        m_restartKey->useKeyUpColor();
        m_restartKey->getModelList()[0]->getMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("data/global/tex/keys/key_restart.png"));
        m_restartKey->setCamera(camera2D);
        m_restartKey->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_restartKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_restartKey->setCallback(passUnifoCallback_noLight);
        m_restartKey->setVisitDrawOrder(10000);
        //
        float center[2]={125,340};
        m_restartKey->reSetCenter(center[0],center[1]);
        m_restartKey->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_restartKey);
    }
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->CreateBufferAndSource("data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    return true;
}
void CpausePage::update(){
    //----continueKey
    //continueKey响应单击
    if(m_continueKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
        Cc3dAudioCache::sharedAudioCache()->continuePausedSource();//继续播放暂停的source
        this->removeFromParentOnNextFrame();
        this->getFather()->setIsDoUpdate(true);
        
        
    }
    //continueKey响应按下和抬起
    if(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_continueKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
        m_continueKey->useKeyDownColor();
    }else{
        m_continueKey->useKeyUpColor();
    }
    
    //----homeKey
    //homeKey响应按下和抬起
    if(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_homeKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
        m_homeKey->useKeyDownColor();
    }else{
        m_homeKey->useKeyUpColor();
    }
    //homeKey响应单击
    if(m_homeKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
        this->getALSourceByName("switchPage")->play();
        ChomePage*homePage=new ChomePage();
        homePage->autorelease();
        homePage->init();
        Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
        Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(homePage);
    
    }
    
    //----restartKey
    if(m_restartKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
        CloadingPage*loadingPage=new CloadingPage();
        loadingPage->autorelease();
        loadingPage->init();
        Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
        Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(loadingPage);
    }
    //restartKey响应按下和抬起
    if(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_restartKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
        m_restartKey->useKeyDownColor();
    }else{
        m_restartKey->useKeyUpColor();
    }

}
CpausePage::CpausePage(){


}
CpausePage::~CpausePage(){
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}