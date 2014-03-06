//
//  initGame.cpp
//  HelloOpenGL
//
//  Created by apple on 14-2-23.
//
//

#include "initGame.h"
//
#include "programSetUp.h"
#include "define.h"
#include "userConfig.h"
#include "c3dDeviceAndOSInfo.h"
#include "c3dRange.h"
#include "c3dLight.h"
#include "c3dCamera.h"
#include "c3dSceneManger.h"
//----pages
#include "homePage.h"
#include "menuPage.h"
#include "startPage.h"
#include "settingPage.h"
#include "aboutPage.h"
#include "endPage.h"
#include "loadingPage.h"
#include "succedPage.h"
#include "descPage2.h"
#include "descPage3.h"
#include "descPage.h"
#include "gameOverPage.h"
#include "pausePage.h"
#include "viewingTipPage.h"
#include "c3dDeviceAndOSInfo.h"
#include "c3dAppState.h"
#include "globalVars.h"

bool initGame(){
    //----初始化shader
    programSetUp();//new
    cout<<"shader setup done"<<endl;
    
    //xxx
    /*     //----地形纹理混合用到的各纹理
     {
     program_diffuse_ambient_noTransf_noSelfShadow_texBlend.useProgram();//必须先useProgram然后才能传值
     glUniform1i(program_diffuse_ambient_noTransf_noSelfShadow_texBlend.unifoSet.alphaMap.loc, 4);
     glUniform1i(program_diffuse_ambient_noTransf_noSelfShadow_texBlend.unifoSet.texture1.loc, 5);
     glUniform1i(program_diffuse_ambient_noTransf_noSelfShadow_texBlend.unifoSet.texture2.loc, 6);
     glUniform1i(program_diffuse_ambient_noTransf_noSelfShadow_texBlend.unifoSet.texture3.loc, 7);
     
     
     }
     */
    //----camera2D
    {
        float left=0;
        float right=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getWidth();
        float top=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getHeight();
        float bottom=0;
        float near=-1;
        float far=1;
        Cc3dRange range(left,right,bottom,top,near,far);
        camera2D=new Cc3dCamera();
        camera2D->init();
        camera2D->autorelease();
        camera2D->retain();//must retain
        camera2D->setRange(range);
        camera2D->setProjectionMode(ec3dOrthographicMode);
    }
    //----camera
    {
        camera=new Cc3dCamera();
        camera->init();
        camera->autorelease();
        camera->retain();//must retain
        camera->setFovy(70);
        camera->setAspect((float)Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getWidth()/Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getHeight());
        camera->setzNear(2);
        camera->setzFar(10000);
        float eyePos[3]={0,0,0};
        float eyeTg[3]={0,0,-1};
        float up[3]={0,1,0};
        camera->setEyePos(Cc3dVector3(eyePos).toV4(1));
        camera->setCenter(Cc3dVector3(eyeTg).toV4(1));
        camera->setUp(Cc3dVector3(up).toV4(0));
        camera->updateFrustum();
    }
    //----设置听者gain
    alListenerf(AL_GAIN, 1.0);
    //----startPage
    CstartPage*startPage=new CstartPage();
    startPage->autorelease();
    startPage->init();
    Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
    Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(startPage);
    //----加载用户设置
    if(CuserConfig::sharedUserConfig()->getIsConfigFileExistInDocuments("userConfig.txt")==false){
        CuserConfig::sharedUserConfig()->copyConfigFileToDocuments("data/userConfig/userConfig.txt");
    }
    CuserConfig::sharedUserConfig()->loadConfigFileFromDocuments("userConfig.txt");
    
    return true;

}