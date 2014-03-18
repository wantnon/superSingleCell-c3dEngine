//
//  gameOverPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "gameOverPage.h"
#include "globalVars.h"
#include "loadingPage.h"
#include "c3dSceneManger.h"
#include "playingPage.h"
#include "define.h"
#include "c3dGestureAnalyzer.h"
bool CgameOverPage::init(){
    //whether check point is empty
    bool isCheckPointEmpty=(( CplayingPage*)Cc3dSceneManager::sharedSceneManager()->getRoot()->getChildByName("playingPage"))->getIsCheckPointEmpty();

    //--quad_gameover
    {
        m_quad_gameover=new Ckey();
        m_quad_gameover->autorelease();
        //
        float bx=0;
        float by=0;
        float w=220;
        float h=55;
        m_quad_gameover->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_gameover->getModelList()[0]->getMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_gameover->getModelList()[0]->getMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_gameover.png"));
        m_quad_gameover->setKeyUpColor(1, 1, 1, 1);
        m_quad_gameover->useKeyUpColor();
        m_quad_gameover->setCamera(camera2D);
        m_quad_gameover->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_quad_gameover->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_gameover->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_gameover->setVisitDrawOrder(10000);
        //
        float _bx=52;
        float _by=340;
        m_quad_gameover->reSetLeftDownCorner(_bx, _by);
        m_quad_gameover->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_gameover);
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
        m_restartKey->getModelList()[0]->getMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_restart.png"));
        m_restartKey->setCamera(camera2D);
        m_restartKey->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_restartKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_restartKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_restartKey->setVisitDrawOrder(10000);
        //
        float center[2];
        if(isCheckPointEmpty){
            center[0]=160;
            center[1]=300;
        }else{
            center[0]=120;
            center[1]=300;
        }
        m_restartKey->reSetCenter(center[0],center[1]);
        m_restartKey->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_restartKey);
    }
    //--rebirthKey
    {
  
        m_rebirthKey=new Ckey();
        m_rebirthKey->autorelease();
        //
        Cc3dTexture* ptex_rebirthKey=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_rebirth.png");
        float c[2]={0,0};
        m_rebirthKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_rebirthKey->getModelList()[0]->getMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_rebirthKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_rebirthKey->setKeyUpColor(1, 1, 1, 1);
        m_rebirthKey->useKeyUpColor();
        m_rebirthKey->getModelList()[0]->getMeshByIndex(0)->setTexture(ptex_rebirthKey);
        m_rebirthKey->setCamera(camera2D);
        m_rebirthKey->getModelList()[0]->setIsWriteDepthBuffer(false);
        m_rebirthKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_rebirthKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_rebirthKey->setVisitDrawOrder(10000);
        //
        float center[2]={200,300};
        m_rebirthKey->reSetCenter(center[0],center[1]);
        m_rebirthKey->getModelList()[0]->getMeshByIndex(0)->submit(GL_STATIC_DRAW);
        m_rebirthKey->setIsVisible(!isCheckPointEmpty);//根据checkPoint是否为空决定是否可见
        //
        addChild(m_rebirthKey);
    }

    
    return true;
}
void CgameOverPage::update(){
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
    //----rebirthKey
    if(m_rebirthKey->getIsVisible()){
        if(m_rebirthKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            (( CplayingPage*)Cc3dSceneManager::sharedSceneManager()->getRoot()->getChildByName("playingPage"))->resetToCheckPoint();
            this->removeFromParentOnNextFrame();
            this->getFather()->setIsDoUpdate(true);
        }
        //rebirthKey响应按下和抬起
        if(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_rebirthKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_rebirthKey->useKeyDownColor();
        }else{
            m_rebirthKey->useKeyUpColor();
        }
    }
}
CgameOverPage::CgameOverPage(){

}
CgameOverPage::~CgameOverPage(){
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}


