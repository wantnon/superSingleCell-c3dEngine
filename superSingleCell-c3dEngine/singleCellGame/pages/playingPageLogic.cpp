//
//  playingPageLogic.cpp
//  HelloOpenGL
//
//  Created by apple on 14-2-23.
//
//

#include "playingPageLogic.h"
#include "playingPage.h"
#include "globalVars.h"
#include "define.h"
#include "c3dGestureAnalyzer.h"
void  CplayingPage::logic(){
    
    CbossCell*cell=m_cell;
    
    //---------------------------
    
    //--------------
    // C3DLOG("cell pos:");cell->getTransform().getPos().print();
    //cell是否绘制
    bool isCellShow=(cell->revTimeCounter_noHert.getCount()!=0&&Cc3dTimeCounter::sharedTimeCounter()->getCount()%10<5)||cell->revTimeCounter_noHert.getCount()==0;
    cell->setIsVisible(isCellShow);
    //----获得isHurtAble
    const bool isHurtAble=cell->revTimeCounter_noHert.getCount()==0;
    //----重置cell.rotLegs
    {
        cell->reSetRotLegs();
    }
    //----重置cell.isPuddleWater
    {
        cell->set_isPuddleWater(false);
    }
    //----重置cell.isMovingOnSolidSurface
    {
        cell->set_isMovingOnSolidSurface(false);
    }
    //----重置cell所在ship的isMoving
    {
        if(cell->get_onShip()){
            cell->get_ship()->set_isMoving(false);
        }
    }
    //----审计
    {
        //----输出各mobilePlatform的触发状态
        /*{
         int n=(int)mobilePlatformSet.pModelBaseList.size();
         for(int i=0;i<n;i++){
         CmobilePlatform*pPlat=(CmobilePlatform*)mobilePlatformSet.pModelBaseList[i];
         cout<<pPlat->triggered<<" ";
         }cout<<endl;
         }*/
        //----输出各shotBall的active状态
        /*  {
         cout<<"shotBallActive:"<<endl;
         int n=(int)cell.pshotBallList.size();
         for(int i=0;i<n;i++){
         bool isActive=cell.pshotBallList[i]->active;
         cout<<isActive;
         }cout<<endl;
         }*/
        
        
    }
    
    
    //----听者跟随主角
    {
        Cc3dVector4 pos=cell->getTransform().getPos();
        Cc3dVector4 up=cell->getUp();
        Cc3dVector4 dir=cell->getDir();
        Cc3dVector4 at=pos+dir;
        float ori[6]={at.x(),at.y(),at.z(),up.x(),up.y(),up.z()};//注意，ori有六个元素(at和up)，而不是三个
        alListenerfv(AL_POSITION,pos.getArray());
        alListenerfv(AL_ORIENTATION, ori);
        
    }
    
    //----如果主角位置低于一定值，则死亡，对于没有水面的情况下，如此处理坠落死亡
    {
        if(cell->getTransform().getPos().y()<=cell->deathY){
            cell->life.dec(cell->life.value*0.2);
            if(cell->life.value<10)cell->life.decToZero();
            //声音
            //cell->pALSource_oops->playIfNotPlaying();
            cell->getALSourceByName("oops")->playIfNotPlaying();
        }
    }
    //----更新各quad_key的透明度
    {
        int nkey=cell->nkey;
        int key_got=cell->key_got;
        for(int i=0;i<nkey;i++){
            Ckey*quad_key=m_quad_keyList[i];
            if(i<key_got){
                quad_key->getModelList()[0]->getMeshByIndex(0)->setTexture(m_tex_quad_key_colored);
            }else{
                quad_key->getModelList()[0]->getMeshByIndex(0)->setTexture(m_tex_quad_key_blackWhite);
            }
        }
    }
    //----主角onFire计时
    {
        cell->doOnFireTimeCount();
        if(cell->onFire==false){
            //  cell->pALSource_onFire->gainDec(0.01);
            cell->getALSourceByName("onFire")->gainDec(0.01);
            int nFire=(int)m_fireList.size();
            for(int i=0;i<nFire;i++){
                m_fireList[i]->setIsVisible(false);
            }
        }
    }
    //----主角无伤害倒计时
    {
        cell->revTimeCounter_noHert.doCount_dec();
        
    }
    //----更新quad_nCoinPositivePlaceList
    if(cell->nEateCoin.isChanged())//change时才更新
    {
        
        float nCoin=cell->get_nEateCoin();
        //      cout<<"nCoin:"<<nCoin<<endl;
        nCoin=minf(999.9,nCoin);
        //求nCoin的个位，十位，百位
        int a0=(int)nCoin%10;//个位
        int a1=((int)nCoin/10)%10;//十位
        int a2=((int)nCoin/100)%10;//百位
        m_quad_nCoinPositivePlaceList[0]->getModelList()[0]->getMeshByIndex(0)->setTexture(m_numberTexList[a2]);
        m_quad_nCoinPositivePlaceList[1]->getModelList()[0]->getMeshByIndex(0)->setTexture(m_numberTexList[a1]);
        m_quad_nCoinPositivePlaceList[2]->getModelList()[0]->getMeshByIndex(0)->setTexture(m_numberTexList[a0]);
        
        
    }
    //----更新quad_nKillEnemyPositivePlaceList
    if(cell->nKillEnemy.isChanged())//change时才更新
    {
        int nKillEnemy=cell->get_nKillEnemy();
        //       cout<<"nKillEnemy:"<<nKillEnemy<<endl;
        nKillEnemy=minf(999,nKillEnemy);
        //求nCoin的个位，十位，百位
        int a0=(int)nKillEnemy%10;//个位
        int a1=((int)nKillEnemy/10)%10;//十位
        int a2=((int)nKillEnemy/100)%10;//百位
        m_quad_nKillEnemyPositivePlaceList[0]->getModelList()[0]->getMeshByIndex(0)->setTexture(m_numberTexList[a2]);
        m_quad_nKillEnemyPositivePlaceList[1]->getModelList()[0]->getMeshByIndex(0)->setTexture(m_numberTexList[a1]);
        m_quad_nKillEnemyPositivePlaceList[2]->getModelList()[0]->getMeshByIndex(0)->setTexture(m_numberTexList[a0]);
    }
    
    //----对mobilePlatformSet中各元素进行变换，并提交变换后的顶点，并更新碰撞列表
    {
        int nMobilePlatform=(int)m_mobilePlatformSet.getActorCount();
        for(int i=0;i<nMobilePlatform;i++){
            CmobilePlatform&mobilePlatform=*(CmobilePlatform*)m_mobilePlatformSet.getActorByIndex(i);
            //变换前留底
            mobilePlatform.storeRTmat();
            //变换
            mobilePlatform.mov();
            //更新vlist
            mobilePlatform.updateAndSubmitVertex();
        }
        //更新collisionGrid_mobilePlatform
        {
            //制作屏蔽列表
            int n=(int)m_mobilePlatformSet.getActorCount();
            vector<bool> maskList(n);//屏蔽列表，初始全0
            m_collisionGrid_mobilePlatform.fillGrid(m_mobilePlatformSet.getActorList(),maskList);
        }
        
    }
    //----主角随其所在惯性平台一起运动
    {
        cell->movCellAndShotBallsWithInertMobilePlatform();
    }
    
    //----更新各mobilePlatform的IDboard的位置和朝向
    {
        int nMobilePlatform=(int)m_mobilePlatformSet.getActorCount();
        for(int i=0;i<nMobilePlatform;i++){
            CmobilePlatform&mobilePlatform=*(CmobilePlatform*)m_mobilePlatformSet.getActorByIndex(i);
            Csprite&IDboard0=*mobilePlatform.getIDBoard0();
            Csprite&IDboard1=*mobilePlatform.getIDBoard1();
            assert(IDboard0.getIsVisible()==IDboard1.getIsVisible());
            if(IDboard0.getIsVisible()){
                //IDboard0
                IDboard0.setPos(mobilePlatform.getTransform().getPos());
                IDboard0.adjustFace(camera->getEyePos());
                IDboard0.getModel()->getMeshByIndex(0)->submitVertex(GL_STREAM_DRAW);
                //IDboard1
                //计算IDboard1的位置
                Cc3dVector4 xvec=IDboard0.getXvec();
                float d=IDboard0.getRx()+IDboard1.getRx();//IDboard0和IDboard1的中心距离
                Cc3dVector4 vec_IDboard0ToIDboard1=xvec*d;
                Cc3dVector4 pos_IDboard1=IDboard0.getPos()+vec_IDboard0ToIDboard1;
                IDboard1.getModel()->setPos(pos_IDboard1);
                IDboard1.adjustFace(camera->getEyePos());
                IDboard1.getModel()->getMeshByIndex(0)->submitVertex(GL_STREAM_DRAW);
                
            }
        }
    }
    //----更新各bubble的IDboard的位置和朝向
    {
        int nbubble=(int)m_bubbleSet.getActorCount();
        for(int i=0;i<nbubble;i++){
            Cbubble&bubble=*(Cbubble*)m_bubbleSet.getActorByIndex(i);
            Csprite&IDboard0=*bubble.getIDBoard0();
            Csprite&IDboard1=*bubble.getIDBoard1();
            assert(IDboard0.getIsVisible()==IDboard1.getIsVisible());
            if(IDboard0.getIsVisible())
            {
                //IDboard0
                IDboard0.setPos(bubble.getPos());
                IDboard0.adjustFace(camera->getEyePos());
                IDboard0.getModel()->getMeshByIndex(0)->submitVertex(GL_STREAM_DRAW);
                //IDboard1
                //计算IDboard1的位置
                Cc3dVector4 xvec=IDboard0.getXvec();
                float d=IDboard0.getRx()+IDboard1.getRx();//IDboard0和IDboard1的中心距离
                Cc3dVector4 vec_IDboard0ToIDboard1=xvec*d;
                Cc3dVector4 pos_IDboard1=IDboard0.getPos()+vec_IDboard0ToIDboard1;
                IDboard1.setPos(pos_IDboard1);
                IDboard1.adjustFace(camera->getEyePos());
                IDboard1.getModel()->getMeshByIndex(0)->submitVertex(GL_STREAM_DRAW);
            }
        }
    }
    
    //----更新meteor的朝向和位置
    {
        //----获得diedList
        vector<bool> diedList=m_meteorSet.get_diedList();
        //----计时
        m_meteorSet.timing(diedList);
        //----设置朝向
        const Cc3dVector4 origin(0,0,0,1);
        m_meteorSet.adjustFaceAndSubmitVertex(origin,diedList);
        //----将died者重新定位
        int nMeteor=(int)m_meteorSet.getActorCount();
        for(int i=0;i<nMeteor;i++){
            Cmeteor&meteor=*(Cmeteor*)m_meteorSet.getActorByIndex(i);
            if(meteor.getDied()==false)continue;//跳过未died者
            //让meteor随机出现在高空某处
            //求meteor位置在xz平面上的投影
            float rxz=r_sky*0.7;//原点到meteor位置在xz平面上的投影长度
            float tx=(rand()%1000)-500;
            float tz=(rand()%1000)-500;
            Cc3dVector4 posxz(tx,0,tz,1);//meteor位置在xz平面的投影
            posxz=normalize(posxz);
            posxz=posxz*rxz;//得到posxz
            //求meteor位置
            float y=r_sky*0.7;
            Cc3dVector4 pos(posxz.x(),y,posxz.z(),1);
            //将meteor重定位
            meteor.setPos(pos);
            //设置meteor.died=false
            meteor.setDied(false);
            meteor.setIsVisible(true);
        }
        //----移动meteorSet，并保持在天际边缘
        for(int i=0;i<nMeteor;i++){
            Cmeteor&meteor=*(Cmeteor*)m_meteorSet.getActorByIndex(i);
            if(meteor.getDied())continue;
            float xproj=-0.81915204428899178968448838591684;//-cosf(35)
            float yproj=-0.57357643635104609610803191282616;//-sinf(35)
            Cc3dVector4 xvec;
            Cc3dVector4 yvec;
            xvec=meteor.getXvec();
            yvec=meteor.getYvec();
            Cc3dVector4 advanceVec=xvec*xproj+yvec*yproj;
            advanceVec=advanceVec*meteor.speed;
            //按advanceVec移动meteor
            meteor.setCenter(meteor.getCenter()+advanceVec);
            //将meteor推到天际
            //求原点到当前位置的向量，将此向量长度缩放为与天空球半径一致（需加一个向量的margin），并以此结果重设位置
            const float margin=300;
            Cc3dVector4 rvec=meteor.getCenter()-origin;
            rvec=normalize(rvec);
            rvec=rvec*(r_sky-margin);
            rvec.setw(1);
            meteor.setPos(rvec);
            
        }
        //如果metoer低于水面，则die
        {
            for(int i=0;i<nMeteor;i++){
                Cmeteor&meteor=*(Cmeteor*)m_meteorSet.getActorByIndex(i);
                if(meteor.getDied())continue;
                if(meteor.getPos().y()<m_water->Horizon){
                    meteor.setDied(true);
                    meteor.setIsVisible(false);
                }
            }
            
            
        }
        
    }
    //----更新bubble的朝向
    {
        m_bubbleSet.adjustFaceAndSubmitVertex(camera->getEyePos(),m_bubbleSet.get_diedList());
    }
    //----更新各bubble中的molecule的朝向
    {
        int nbubble=(int)m_bubbleSet.getActorCount();
        for(int i=0;i<nbubble;i++){
            Cmolecule*pmolecule=((Cbubble*)m_bubbleSet.getActorByIndex(i))->getMolecule();
            pmolecule->adjustFaceAndSubmitVertex(camera->getEyePos());
        }
    }
    //----播放cell.pmolecule的动画
    {
        int n=(int)m_fireList.size();
        for(int i=0;i<n;i++){
            m_fireList[i]->useNextFrameTex();
        }
    }
    //----播放各bubble.pmolecule的动画
    {
        int nbubble=(int)m_bubbleSet.getActorCount();
        for(int i=0;i<nbubble;i++){
            Cmolecule*pmolecule=((Cbubble*)m_bubbleSet.getActorByIndex(i))->getMolecule();
            pmolecule->useNextFrameTex();
        }
        
    }
    
    //----更新fakeLight的朝向
    {
        m_fakeLightSet.adjustFaceAndSubmitVertex(camera->getEyePos());
    }
    
    //----子弹发射计时
    {
        cell->revTimeCounter_shot.doCount_dec();
        float r,g,b,a;
        if(cell->isShotTimeLastsLong){
            r=1;
            g=1-(float)cell->revTimeCounter_shot.getCount()/shotTimeLasts_long;
            b=g;
            a=m_shotKey->keyUpColor[3];
        }else{
            r=1;
            g=1-0.5*(float)cell->revTimeCounter_shot.getCount()/shotTimeLasts_short;
            b=1-(float)cell->revTimeCounter_shot.getCount()/shotTimeLasts_short;
            a=m_shotKey->keyUpColor[3];
        }
        m_shotKey->setKeyUpColor(r, g, b, a);
    }
    //----发射
    {
        //   cout<<"inputSignal.shotKeyDown.get():"<<inputSignal.shotKeyDown.get()<<endl;
        //   cout<<"inputSignal.shotKeyTapTwice:"<<inputSignal.shotKeyTapTwice<<endl;
        //    cout<<(inputSignal.shotKeyTapTwice==true)<<endl;
        if(//inputSignal.shotKeyDown.get()
           m_shotKey->getIsDown()
           ){
            long oldValue=cell->revTimeCounter_shot.getCount();
            if(oldValue>shotTimeLasts_short){
                //无动作
            }else{//shotTimeLasts_short>=oldValue
                cell->revTimeCounter_shot.setValue(shotTimeLasts_short);
                cell->isShotTimeLastsLong=false;
            }
        }
        /*        //twice的判断一定要放在后面
         if(inputSignal.shotKeyTapTwice==true){
         cell->revTimeCounter_shot.setValue(shotTimeLasts_long);
         cell->isShotTimeLastsLong=true;
         }
         */
        
        
        
        if(cell->revTimeCounter_shot.getCount()!=0){
            //发射子弹
            if(Cc3dTimeCounter::sharedTimeCounter()->getCount()%5==0){
                cell->set_nGun(4);
                const int shotBallType=0;
                cell->shotBall_launch(m_bossCellSet.getLiveAndMovingBossList(),shotBallType,true);
            }
        }
        if(//inputSignal.shotBigKeyDown.get()
           m_shotBigKey->getIsDown()
           )
            //在shtoBigKey由down转up时发射
        {
            //发射炮弹
            cell->set_nGun(1);
            const int shotBallType=1;
            cell->shotBall_launch(m_bossCellSet.getLiveAndMovingBossList(),shotBallType);
        }
    }
    //----quad_oops计时
    {
        m_quad_oops->disapearCount();
        if(m_quad_oops->disapearCounter.getCount()==0){
            //     quad_oops.set_invisible(true);
            m_quad_oops->setIsVisible(false);
        }
    }
    //----quad_out_of_fuel计时
    {
        m_quad_out_of_fuel->disapearCount();
        if(m_quad_out_of_fuel->disapearCounter.getCount()==0){
            //    quad_out_of_fuel.set_invisible(true);
            m_quad_out_of_fuel->setIsVisible(false);
        }
    }
    //----quad_halfFuelHaveBeenUsed计时
    {
        m_quad_halfFuelHaveBeenUsed->disapearCount();
        if(m_quad_halfFuelHaveBeenUsed->disapearCounter.getCount()==0){
            //      quad_halfFuelHaveBeenUsed.set_invisible(true);
            m_quad_halfFuelHaveBeenUsed->setIsVisible(false);
            
        }
        
    }
    
    //----更新主角的环境信息
    {
        Cc3dVector4 cellpos=cell->getTransform().getPos();
        cell->environmentInfo.updateInfo(cellpos,cell->getRc(),cell->Rc_small_value_normal,m_ground, m_water,m_archScene,&m_mobilePlatformSet);
        
        /*   if(timeCounter.getCount()%60==0){
         printv3(cellpos);
         }//*/
        /*    if(timeCounter.getCount()%60==0){
         
         printMat(cell.transf.RTmat);
         }//*/
        
    }
    //----主角自动弹跳接受加速倒计时
    {
        cell->revTimeCounter_bouncingReceiveAccelerate.doCount_dec();
    }
    //----主角触发surface和ceil运动平台
    {
        cell->doTriggerCeilPlatform();
        cell->doTriggerSurfacePlatform();
    }
    //----当前主角的surfacePlat和ceilPlat解锁
    {
        if(cell->isJumping()){
            if(cell->environmentInfo.ceilType==CEILTYPE_MOBILEPLATFORM){
                CmobilePlatform*pPlatform=cell->environmentInfo.pCeilMobilePlatform;
                if(pPlatform!=NULL){
                    pPlatform->isTrigger_movLocked=false;
                    pPlatform->isTrigger_rotLocked=false;
                }
            }
            if(cell->environmentInfo.surfaceType==SURFACETYPE_MOBILEPLATFORM){
                CmobilePlatform*pPlatform=cell->environmentInfo.pSurfaceMobilePlatform;
                if(pPlatform!=NULL){
                    pPlatform->isTrigger_movLocked=false;
                    pPlatform->isTrigger_rotLocked=false;
                }
            }
        }
        
    }
    //----跳跃和着陆
    
    if(cell->get_onShip()==false)//如果主角未上船
    {
        //bool jumpCondition=inputSignal.jumpKeyDown.get()||inputSignal.jumpForwardKeyDown.get();
        bool jumpCondition=m_cell->jumpSignal||m_cell->jumpForwardSignal;
        if(jumpCondition){
            //跳
            if(cell->isJump.get()==false){//如果没起跳
                if(cell->getIsFallFlat()==false){//没摔扁才能跳
                    //----起跳
                    float v0up=cell->get_v0up_basic()+cell->get_vupInertial_causedByMobilePlatform();
                    cell->doJump(v0up);
                    // cell->pALSource_ya->play();//声音
                    if(true){
                        cell->getALSourceByName("ya")->play();
                    }
                    //----旋转legs
                    cell->setRotLegs(true);
                }
            }else{//如果已起跳
                //为自动弹跳加速
                cell->accelerateBouncing();
            }
        }
        //主角上升和下降
        cell->doRiseAndFall();
        
        //主角摔扁（放在调整姿态之前，以使调整姿态的力度能够根据是否摔扁来进行）
        if(cell->isJump.get()==false&&cell->isJump.isChanged()){
            //是否在固体表面
            bool isOnSolidSurface=(cell->environmentInfo.surfaceType==SURFACETYPE_GROUND
                                   ||cell->environmentInfo.surfaceType==SURFACETYPE_MOBILEPLATFORM
                                   ||cell->environmentInfo.surfaceType==SURFACETYPE_ARCH);
            //落在固体表面且起跳和落地高度差距大于一定的值时才摔扁
            float jumpH=cell->jumpH_landH.get_foe();//起跳高度
            float landH=cell->jumpH_landH.get();//落地高度
            if(isOnSolidSurface&&jumpH-landH>cell->get_H_causeFlat()){
                float H_extra=jumpH-landH-cell->get_H_causeFlat();
                const long duringTimeMin=27;
                cell->doFallFlat(min(duringTimeMin+H_extra*(0.5*duringTimeMin/cell->get_H_causeFlat()),duringTimeMin*1.6));
                //声音
                // cell->pALSource_fallFlat_paa_oops->play();
                cell->getALSourceByName("fallFlat_paa_oops")->play();
                //quad_oops置为可见，并设定消失时间
                //      quad_oops.set_invisible(false);
                m_quad_oops->setIsVisible(true);
                m_quad_oops->setDisapearCountValue(40);
                //掉血
                float lifeDec=20;//maxf(10,minf(H_extra*0.15,50));
                if(isHurtAble){
                    cell->life.dec(lifeDec);
                }
            }
        }
        //着陆(下降后必须立即着陆)
        if(cell->isJump.get()==false){
            if(cell->getIsFallFlat()){//摔扁
                //软调整，但不很软
                cell->adjustPosAndPostureToSurface(adjustSoftness_HARD);
            }else{//不摔扁
                //软调整
                cell->adjustPosAndPostureToSurface(adjustSoftness_VERYSOFT);
            }
        }
        
        
        
    }
    //----水对主角的伤害
    {
        if(cell->environmentInfo.surfaceType==SURFACETYPE_WATER&&cell->isTouchSurface(1.0)){
            if(isHurtAble){
                cell->life.dec(m_water->hurtValue);
                //声音
                // cell->pALSource_oops->playIfNotPlaying();
                cell->getALSourceByName("oops")->playIfNotPlaying();
                
            }
        }
        
    }
    
    
    
    
    //----摔扁计时
    {
        cell->doFlatTimeCount();
    }
    
    
    //----移动
    if(cell->get_onShip()==false)//如果未上船
    {
        cell->storeRTmat();//移动前保存RTmat以便恢复
        //主角的任何运动都必须放在cell.storeRTmat之后
        //右转
        if(//inputSignal.rotKeyDown.get()
           m_cell->rotSignal
           ){
            float dA=(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getTouchMoveSpeed().x())*0.007;//0.01;
            if(fabsf(dA)>0.01){
                cell->rotRight(dA);
                cell->setRotLegs(true);
            }
            
        }
        if (cell->getIsFallFlat()==false) {
            //后退
            if(//inputSignal.backKeyDown.get()
               m_cell->backSignal
               ){
                if(fabsf(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getTouchMoveSpeed().y())>1){
                    cell->movBack();
                }
                cell->setRotLegs(true);
                if(cell->environmentInfo.surfaceType==SURFACETYPE_WATER&&cell->isTouchSurface(1.0)){//在水面
                    cell->set_isPuddleWater(true);
                }
                if((cell->environmentInfo.surfaceType==SURFACETYPE_ARCH||cell->environmentInfo.surfaceType==SURFACETYPE_GROUND||cell->environmentInfo.surfaceType==SURFACETYPE_MOBILEPLATFORM)
                   &&cell->isTouchSurface(3)){//如果在建筑、地面、移动平台表面
                    cell->set_isMovingOnSolidSurface(true);
                    
                }
            }
            //前进
            if(//inputSignal.forwardKeyDown.get()||inputSignal.jumpForwardKeyDown.get()
               m_cell->forwardSignal||m_cell->jumpForwardSignal
               ){
                cell->movForward(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganTime(),Cc3dTimeCounter::sharedTimeCounter()->getCount(),10);
                cell->setRotLegs(true);
                if(cell->environmentInfo.surfaceType==SURFACETYPE_WATER&&cell->isTouchSurface(1.0)){//在水面
                    cell->set_isPuddleWater(true);
                }
                if((cell->environmentInfo.surfaceType==SURFACETYPE_ARCH||cell->environmentInfo.surfaceType==SURFACETYPE_GROUND||cell->environmentInfo.surfaceType==SURFACETYPE_MOBILEPLATFORM)
                   &&cell->isTouchSurface(3)){//如果在建筑、地面、移动平台表面
                    cell->set_isMovingOnSolidSurface(true);
                    
                }
            }
        }
        //主角与墙建筑墙壁碰撞(必须放在着陆之后)
        {
            //注意:主角与墙壁的碰撞必须放在主角的所有运动（前后左右，上升下降）都执行过之后
            //与墙壁的碰撞
            vector<int> skipTagList;
            skipTagList.push_back(tag_collisionMesh);
            cell->collisionAndReActWithArchWall(*m_archScene, true, skipTagList, 5);
            //再进行一次着陆
            //与墙壁的碰撞之后需再进行一次着陆，避免主角穿透地面或arch地面
            if(cell->isJump.get()==false){//未跳起才调整高度
                cell->adjustPosAndPostureToSurface(adjustSoftness_VERYSOFT);
            }
        }
        //主角与mobilePlatform的墙壁进行碰撞检测（必须放在着陆之后）
        {
            Cc3dVector4 cellpos=cell->getTransform().getPos();
            //获得cell周围的mobilePlatform列表
            CmobilePlatformSet mobilePlatformSet_selected;
            mobilePlatformSet_selected.setActorList(m_collisionGrid_mobilePlatform.getCollisonList(cellpos));
            //     cout<<"selectedMobilePlatCount:"<<(int)mobilePlatformSet_selected.pModelBaseList.size()<<endl;
            cell->collisionAndReActWithmobilePlatformWall(mobilePlatformSet_selected,true,true,5);
        }
        
    }
    //----主角旋转legs
    cell->legsRot();
    //----ship移动和升降
    if(cell->get_onShip()==true){
        Cship*pship= cell->get_ship();
        
        //更新ship环境信息
        pship->environmentInfo.updateInfo(pship->getTransform().getPos(), pship->getRc(), pship->Rc_small_value_normal,m_ground, m_water, m_archScene,&m_mobilePlatformSet);
        //保存漂行前矩阵
        pship->RTmat_store_beforeFloating=pship->getTransform().getRTmat();
        //----漂行
        float ds=pship->floatingInAir();
        //   cout<<ds<<endl;
        //----移动
        //右转
        if(//inputSignal.rotKeyDown.get()
           m_cell->rotSignal
           ){
            
            float dA=(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getTouchMoveSpeed().x())*0.007;//0.01;
            if(fabsf(dA)>0.005){
                pship->rotRight(dA);
            }
            
        }
        //保存RTmat以便恢复(在漂行之后保存，即不对漂行进行恢复，可实现弹性滑动效果)
        pship->storeRTmat();
        //会飞的ship上升与下降
        if(pship->get_isFlyable()){//ship会飞
            //上升
            if(//inputSignal.upKeyDown.get()
               m_cell->upSignal
               ){
                if(cell->fuel.value!=0){
                    pship->pushUp();
                    pship->set_isMoving(true);
                    if(m_quad_halfFuelHaveBeenUsed->haveBeenDisplayed==false&&cell->fuel.value<cell->fuel.maxValue/2){
                        //       quad_halfFuelHaveBeenUsed.set_invisible(false);
                        m_quad_halfFuelHaveBeenUsed->setIsVisible(true);
                        m_quad_halfFuelHaveBeenUsed->setDisapearCountValue(duringTime_quad_halfFuelHaveBeenUsed);
                        m_quad_halfFuelHaveBeenUsed->haveBeenDisplayed=true;
                    }
                }else{
                    // quad_out_of_fuel.set_invisible(false);
                    m_quad_out_of_fuel->setIsVisible(true);
                    m_quad_out_of_fuel->setDisapearCountValue(duringTime_quad_out_of_fuel);
                }
            }
            //下降
            if(//inputSignal.downKeyDown.get()
               m_cell->dnSignal
               ){
                if(cell->fuel.value!=0){
                    pship->pushDown();
                    pship->set_isMoving(true);
                    if(m_quad_halfFuelHaveBeenUsed->haveBeenDisplayed==false&&cell->fuel.value<cell->fuel.maxValue/2){
                        //         quad_halfFuelHaveBeenUsed.set_invisible(false);
                        m_quad_halfFuelHaveBeenUsed->setIsVisible(true);
                        m_quad_halfFuelHaveBeenUsed->setDisapearCountValue(duringTime_quad_halfFuelHaveBeenUsed);
                        m_quad_halfFuelHaveBeenUsed->haveBeenDisplayed=true;
                    }
                }else{
                    //     quad_out_of_fuel.set_invisible(false);
                    m_quad_out_of_fuel->setIsVisible(true);
                    m_quad_out_of_fuel->setDisapearCountValue(duringTime_quad_out_of_fuel);
                }
            }
        }
        
        //前进
        if(//inputSignal.forwardKeyDown.get()
           m_cell->forwardSignal
           ){
            if(cell->fuel.value!=0){
                pship->pushForward();
                pship->set_isMoving(true);
                if(m_quad_halfFuelHaveBeenUsed->haveBeenDisplayed==false&&cell->fuel.value<cell->fuel.maxValue/2){
                    //   quad_halfFuelHaveBeenUsed.set_invisible(false);
                    m_quad_halfFuelHaveBeenUsed->setIsVisible(true);
                    m_quad_halfFuelHaveBeenUsed->setDisapearCountValue(duringTime_quad_halfFuelHaveBeenUsed);
                    m_quad_halfFuelHaveBeenUsed->haveBeenDisplayed=true;
                }
            }else{
                //   quad_out_of_fuel.set_invisible(false);
                m_quad_out_of_fuel->setIsVisible(true);
                m_quad_out_of_fuel->setDisapearCountValue(duringTime_quad_out_of_fuel);
                //声音
                //  cell->pALSource_alert_out_of_fuel->updatePosition(cell->getTransform().getPos());
                //  cell->pALSource_alert_out_of_fuel->playIfNotPlaying();
                cell->getALSourceByName("alert_out_of_fuel")->updatePosition(cell->getTransform().getPos());
                cell->getALSourceByName("alert_out_of_fuel")->playIfNotPlaying();
            }
        }
        
        
        //ship与建筑碰撞
        {
            vector<int> skipTagList;
            skipTagList.push_back(tag_collisionMesh);
            pship->collisionAndReActWithArchWall(*m_archScene,true,skipTagList,5);
            
        }
        //ship与mobilePlatform碰撞检测
        {
            Cc3dVector4 shipPos=pship->getTransform().getPos();
            //获得ship周围的mobilePlatform列表
            CmobilePlatformSet mobilePlatformSet_selected;
            mobilePlatformSet_selected.setActorList(m_collisionGrid_mobilePlatform.getCollisonList(shipPos));
            pship->collisionAndReActWithmobilePlatformWall(mobilePlatformSet_selected,true,true,5);
        }
        //----着陆和着天花板
        pship->adjustPosAndPostureToSurface();
        pship->adjustPosToCeil();
        //----触发sourface和ceil运动平台
        pship->doTriggerCeilPlatform();
        pship->doTriggerSurfacePlatform();
        //----主角随飞船一起移动
        {
            if(cell->get_onShip()){
                Cship*pship= cell->get_ship();
                Cc3dMatrix4 seatRTmat=pship->getSeatRTmat();
                cell->getTransformPointer()->setRTmat(seatRTmat);
            }
        }
        //----根据pship处水深决定是否可以航行
        Cc3dVector4 shipPos=pship->getTransform().getPos();
        const float Hground=m_ground?m_ground->getH(shipPos.x() , shipPos.z()):-c3d_INF;
        const float Hwater=m_water?m_water->Horizon:-c3d_INF;
        if(pship->get_isFlyable()==false
           &&Hwater-Hground<=5)//如果不能飞且水过浅，则恢复位置
        {
            //注意，不是恢复为pship->RTmat_store，而是恢复为RTmat_store_beforeFloating
            pship->getTransformPointer()->setPos(pship->RTmat_store_beforeFloating.getAt(12),
                                                 pship->RTmat_store_beforeFloating.getAt(13),
                                                 pship->RTmat_store_beforeFloating.getAt(14));
        }else{//否则移动成功消耗颜料
            cell->fuel.dec(ds);//消耗燃料
        }
        
        
        
    }
    //----所有的未使用的ship也都要进行碰撞检测
    {
        int nShip=(int)m_shipSet.getActorCount();
        for(int i=0;i<nShip;i++){
            Cship*pship=(Cship*) m_shipSet.getActorByIndex(i);
            if(pship==cell->get_ship())continue;//跳过正在使用的ship
            
            //更新ship环境信息
            pship->environmentInfo.updateInfo(pship->getTransform().getPos(), pship->getRc(), pship->Rc_small_value_normal,m_ground, m_water, m_archScene,&m_mobilePlatformSet);
            //保存RTmat以便恢复（不需要弹性滑动效果，所以在漂行之前保存矩阵）
            pship->storeRTmat();
            //----漂行
            pship->floatingInAir();
            //----着陆和着天花板
            pship->adjustPosAndPostureToSurface();
            pship->adjustPosToCeil();
            //----触发sourface和ceil运动平台
            pship->doTriggerCeilPlatform();
            pship->doTriggerSurfacePlatform();
            //ship与建筑碰撞
            {
                vector<int> skipTagList;
                skipTagList.push_back(tag_collisionMesh);
                pship->collisionAndReActWithArchWall(*m_archScene,false,skipTagList,3);
            }
            //ship与mobilePlatform碰撞检测
            {
                Cc3dVector4 shipPos=pship->getTransform().getPos();
                //获得ship周围的mobilePlatform列表
                CmobilePlatformSet mobilePlatformSet_selected;
                mobilePlatformSet_selected.setActorList(m_collisionGrid_mobilePlatform.getCollisonList(shipPos));
                pship->collisionAndReActWithmobilePlatformWall(mobilePlatformSet_selected,true,false,3);
            }
            //----根据pship处水深决定是否可以航行
            Cc3dVector4 shipPos=pship->getTransform().getPos();
            const float Hground=m_ground?m_ground->getH(shipPos.x() , shipPos.z()):-c3d_INF;
            const float Hwater=m_water?m_water->Horizon:-c3d_INF;
            if(pship->get_isFlyable()==false
               &&Hwater-Hground<=5)//如果不能飞且水过浅，则恢复位置
            {
                pship->getTransformPointer()->setPos(pship->RTmat_store.getAt(12),pship->RTmat_store.getAt(13),pship->RTmat_store.getAt(14));
                
            }
            
            
            
            
        }
    }
    //----ship调整位姿
    {
        int n=(int)m_shipSet.getActorCount();
        for(int i=0;i<n;i++){
            Cship*pship=(Cship*) m_shipSet.getActorByIndex(i);
            if(pship->getIsVisible()==false)continue;
            float norm[4]={0,1,0,0};
            if(pship->isTouchSurface(1.0)==false){
                pship->setPosture_soft(norm, 0.05);
            }
        }
        
    }
    //----所有isFlyable==false的ship自动下沉
    {
        int n=(int)m_shipSet.getActorCount();
        for(int i=0;i<n;i++){
            Cship*pship=(Cship*)m_shipSet.getActorByIndex(i);
            if(pship->getIsVisible()==false)continue;
            if(pship->get_isFlyable())continue;
            pship->pushDown();
        }
    }
    
    //----求各bossCell当前距主角的曼哈顿距离，并计算active和simpleShow
    {
        Cc3dVector4 cellpos=cell->getTransform().getPos();
        int n=(int)m_bossCellSet.getActorCount();
        for(int i=0;i<n;i++){
            CbossCell*pBossCell=(CbossCell*)m_bossCellSet.getActorByIndex(i);
            if(pBossCell->getIsDied()||pBossCell->getIsVisible()==false)continue;
            if(pBossCell->isFly==false){//如果不会飞
                Cc3dVector4 bossCellPos=pBossCell->getTransform().getPos();
                pBossCell->MHD_fromMyCell=fabsf(bossCellPos.x()-cellpos.x())+fabsf(bossCellPos.y()-cellpos.y())+fabsf(bossCellPos.z()-cellpos.z());
                pBossCell->midFar=(pBossCell->MHD_fromMyCell>MHD_bossCellMidFar);
                pBossCell->tooFar=(pBossCell->MHD_fromMyCell>MHD_bossCellTooFar);
            }else{//如果会飞
                const float margin=200;//margin要小，否则使玩家可以在过远处对飞行boss事先射击，就没难度了
                Cc3dRange rangeExpanded=pBossCell->flyRange;
                rangeExpanded.expand(margin, margin, margin);
                //如果cell进入了rangeExpanded，则pBossCell开始运动
                if(rangeExpanded.containPoint(cell->getTransform().getPos())){
                    pBossCell->midFar=false;
                    pBossCell->tooFar=false;
                }else{
                    pBossCell->midFar=true;
                    pBossCell->tooFar=false;
                }
                
            }
            
        }
        
    }
    //----各bossCell的revTimeCounter_headingToTargetAgain计时
    {
        int n=(int)m_bossCellSet.getActorCount();
        for(int i=0;i<n;i++){
            CbossCell*pBossCell=(CbossCell*)m_bossCellSet.getActorByIndex(i);
            if(pBossCell->getIsDied()||pBossCell->midFar)continue;//没死且活动者才计时
            if(pBossCell->get_isFly()){//如果是飞行boss，则要对它的ship操作
                pBossCell->get_ship()->revTimeCounter_headingToTargetAgain.doCount_dec();
            }else{
                pBossCell->revTimeCounter_headingToTargetAgain.doCount_dec();
            }
            
        }
        
    }
    //----求各bossCell是否可见
    {
        int count_visible=0;
        int n=(int)m_bossCellSet.getActorCount();
        for(int i=0;i<n;i++){
            CbossCell*pBossCell=(CbossCell*)m_bossCellSet.getActorByIndex(i);
            if(pBossCell->getIsDied())continue;
            Cc3dVector4 bossCellPos=pBossCell->getTransform().getPos();
            pBossCell->setIsVisible(camera->getFrustum().ballIsPotentiallyVisible(bossCellPos, pBossCell->getRc()));
            if(pBossCell->getIsVisible())count_visible++;
        }
        //        cout<<"count_visible:"<<count_visible<<endl;
        
        
    }
    //----求各ship是否可见
    {
        int count_visible=0;
        int n=(int)m_shipSet.getActorCount();
        for(int i=0;i<n;i++){
            Cship*pship=(Cship*) m_shipSet.getActorByIndex(i);
            pship->setIsVisible(camera->getFrustum().ballIsPotentiallyVisible(pship->getTransform().getPos(), pship->getRc()));
            if(pship->getIsVisible())count_visible++;
        }
        //        cout<<"count_visible:"<<count_visible<<endl;
        
        
    }
    
    //----子弹飞行
    {
        cell->shotBall_mov(/**water*/);
        //统计未active的子弹数
        //     cout<<"active shotBall Count:"<<cell.getActiveShotBallCount()<<endl;
        
        
    }
    //----子弹与arch碰撞
    {
        cell->shotBall_collisionWithArch(*m_archScene,m_shotBlastSet,*camera);
        
    }
    //----子弹与mobilePlatformSet碰撞
    {
        cell->shotBall_collisionWithMobilePlatform(m_collisionGrid_mobilePlatform,m_shotBlastSet,*camera);
    }
    //----子弹与地面碰撞
    {
        cell->shotBall_collisionWithGround(m_ground,m_shotBlastSet,*camera);
        
    }
    //----shotBlast动画
    {
        m_shotBlastSet.doBlast(camera);
    }
    //----bubbleBlast动画
    {
        m_bubbleBlastSet.doBlast(camera);
    }
    
    
    
    //----push water
    if(m_water->isExist){//water存在
        if(cell->get_onShip()==false)//未在船上
        {
            if(cell->environmentInfo.surfaceType==SURFACETYPE_WATER
               &&cell->isTouchSurface(1.0)
               &&(cell->isJump.isChanged()
                  // ||inputSignal.forwardKeyDown.get()
                  ||m_cell->forwardSignal
                  // ||inputSignal.backKeyDown.get()
                  ||m_cell->backSignal
                  // ||inputSignal.rotKeyDown.get())
                  ||m_cell->rotSignal)
               ){
                cell->pushWater(*m_water);
            }
        }else{//在船上
            Cship*pship= cell->get_ship();
            if(pship->environmentInfo.surfaceType==SURFACETYPE_WATER&&pship->isTouchSurface(1.0)){
                if(//inputSignal.rotKeyDown.get()
                   m_cell->rotSignal
                   // ||inputSignal.forwardKeyDown.get()
                   ||m_cell->forwardSignal
                   // ||inputSignal.backKeyDown.get()
                   ||m_cell->backSignal
                   // ||inputSignal.downKeyDown.get()
                   ||m_cell->dnSignal
                   // ||inputSignal.upKeyDown.get()
                   ||m_cell->upSignal
                   ){
                    
                    pship->pushWater(*m_water);
                }
            }
        }
        
    }
    //----旋转pBossCellList的legs
    {
        int n=(int)m_bossCellSet.getActorCount();
        for(int i=0;i<n;i++){
            CbossCell*pbossCell=(CbossCell*)m_bossCellSet.getActorByIndex(i);
            if(pbossCell->midFar||pbossCell->getIsVisible()==false||pbossCell->getIsDied())continue;
            if(pbossCell->isFly)continue;
            pbossCell->legsRot();
        }
        
    }
    //----水面
    //水面流动
    {
        
        m_water->uvFlow(0.0002, 0);
    }
    //水面波动
    {
        m_water->wave(cell->getTransform().getPos());
    }
    //更新水面
    {
        Cc3dVector4 cellpos=cell->getTransform().getPos();
        //centerSquare
        if(cell->get_onShip()){//如果主角在ship上
            Cship*pship=cell->get_ship();
            //如果ship接触水面，则更新涟漪位置
            if(pship->environmentInfo.surfaceType==SURFACETYPE_WATER&&pship->isTouchSurface(1.0)){
                m_water->update_centerSquare(cellpos);
                m_water->submitStream_centerSquare();
            }
        }else{//如果主角不在ship上
            //如果主角接触水面，则更新涟漪位置
            if(cell->environmentInfo.surfaceType==SURFACETYPE_WATER&&cell->isTouchSurface(1.0)){//只有主角接触水面时涟漪位置才更新
                m_water->update_centerSquare(cellpos);
                m_water->submitStream_centerSquare();
            }
        }
        
        
        //texSquare
        m_water->update_arabesquiticSquare(cellpos,0,90,0,90);//70
        m_water->submitStream_arabesquiticSquare();
        //reflectSquare
        m_water->update_reflectSquare(cellpos);
        m_water->submitStream_reflectSquare();
        //设置water.maxAlpha
        m_water->maxAlpha=max(0.45,0.68-(cellpos.y()-m_water->Horizon)*0.0005);
    }
    
    //更新ground
    if(m_ground)m_ground->update(*camera);
    if(m_ground)m_ground->submitIndex(GL_STREAM_DRAW);//ground->submitStream_ground();
    //更新collisionGrid_boss，并将出界的boss标记为died
    {
        //制作屏蔽列表
        int n=(int)m_bossCellSet.getActorCount();
        vector<bool> maskList(n);//屏蔽列表，初始全0
        for(int i=0;i<n;i++){
            CbossCell*pbossCell=(CbossCell*)m_bossCellSet.getActorByIndex(i);
            if(pbossCell->midFar||pbossCell->getIsDied()){//不可见者并不屏蔽
                maskList[i]=true;
            }
        }
        //更新collisionGrid_boss
        vector<Cc3dNode*> outRangeList=m_collisionGrid_boss.fillGrid(m_bossCellSet.getActorList(),maskList);
        //将出界的地面boss标记为died
        int nOutRange=(int)outRangeList.size();
        for(int i=0;i<nOutRange;i++){
            CbossCell*pboss=(CbossCell*)outRangeList[i];
            if(pboss->get_isFly())continue;//跳过飞行boss
            pboss->setIsDied(true);
            pboss->setIsVisible(false);
            cout<<"warning: boss outRange"<<endl;
            //  pboss->pALSource_boss_scream->play();
            pboss->getALSourceByName("boss_scream")->play();
            cell->nKillEnemyAdd();
        }
    }
    //bossCellSet移动
    {
        int activeGroundBossCount=0;
        int activeFlyBossCount=0;
        int n=(int)m_bossCellSet.getActorCount();
        for(int i=0;i<n;i++){
            CbossCell*pbossCell=(CbossCell*)m_bossCellSet.getActorByIndex(i);
            if(pbossCell->midFar||pbossCell->getIsDied())continue;//不可见也要移动及与墙壁进行碰撞检测
            
            if(pbossCell->get_isFly()==false){//如果不会飞行
                activeGroundBossCount++;
                //更新环境信息
                {
                    Cc3dVector4 pos=pbossCell->getTransform().getPos();
                    pbossCell->environmentInfo.updateInfo(pos, pbossCell->getRc(),pbossCell->Rc_small_value_normal, m_ground, m_water, m_archScene,NULL);//bossCell不传mobilePlatformSet，即不与之碰撞
                }
                //移动前保存RTmat以便恢复
                pbossCell->storeRTmat();
                //移动
                pbossCell->autoMov(cell->getTransform().getPos());
                //上升和下降
                pbossCell->doRiseAndFall();
                //着陆（下降后要立即着陆）
                if(pbossCell->isJump.get()==false)pbossCell->adjustPosAndPostureToSurface(adjustSoftness_VERYSOFT);
                //与墙壁碰撞（必须放在着陆之后）
                vector<int> skipTagList;
                pbossCell->collisionWithArchWall_boss(*m_archScene,false,skipTagList,3);
                //再进行一次简化的着陆，防止穿透地面或arch地面
                if(pbossCell->isJump.get()==false)pbossCell->adjustH(adjustSoftness_VERYSOFT);
            }else{//如果会飞行
                activeFlyBossCount++;
                
                //----漂行和着陆
                Cship*pship= pbossCell->get_ship();
                if(pship==NULL){
                    cout<<"error:pship==NULL!"<<endl;
                    exit(0);
                }
                //更新ship环境信息
                pship->environmentInfo.updateInfo(pship->getTransform().getPos(), pship->getRc(), pship->Rc_small_value_normal, m_ground, m_water, m_archScene,&m_mobilePlatformSet);
                //保存RTmat以便恢复（不需要弹性滑动效果，所以在漂行之前保存矩阵）
                pship->storeRTmat();
                //----漂行
                pship->floatingInAir();
                //----着陆和着天花板
                pship->adjustPosAndPostureToSurface();
                pship->adjustPosToCeil();
                //----触发sourface和ceil运动平台
                pship->doTriggerCeilPlatform();
                pship->doTriggerSurfacePlatform();
                //----智能移动
                //智能移动
                pship->autoMov(cell->getTransform().getPos());
                //将ship囚禁在飞行范围之内
                {
                    if(pship->prisonInCube(pbossCell->flyRange)){
                        pship->randomRot();
                        //设定再次向目标前进的倒计时
                        pship->revTimeCounter_headingToTargetAgain.setValue(pship->afterHowLongHeadingToTargetAgain);
                    }
                }
                //ship与建筑碰撞
                {
                    if(pship->collisionAndReActWithArchWall_flyBoss(*m_archScene)){
                        pship->randomRot();
                        //设定再次向目标前进的倒计时
                        pship->revTimeCounter_headingToTargetAgain.setValue(pship->afterHowLongHeadingToTargetAgain);
                    }
                }
                //ship与mobilePlatform碰撞检测
                {
                    Cc3dVector4 shipPos=pship->getTransform().getPos();
                    //获得ship周围的mobilePlatform列表
                    CmobilePlatformSet mobilePlatformSet_selected;
                    mobilePlatformSet_selected.setActorList(m_collisionGrid_mobilePlatform.getCollisonList(shipPos));
                    if(pship->collisionAndReActWithmobilePlatformWall(mobilePlatformSet_selected,
                                                                      true,
                                                                      false,
                                                                      3)){
                        pship->randomRot();
                        //设定再次向目标前进的倒计时
                        pship->revTimeCounter_headingToTargetAgain.setValue(pship->afterHowLongHeadingToTargetAgain);
                        
                    }
                }
                //----着陆和着天花板
                pship->adjustPosAndPostureToSurface();
                pship->adjustPosToCeil();
                //----pbossCell随飞船一起移动
                {
                    Cc3dMatrix4 seatRTmat=pship->getSeatRTmat();
                    pbossCell->getTransformPointer()->setRTmat(seatRTmat);
                }
                //----pbossCell部件运动
                {
                    //wing运动
                    pbossCell->waveWing();
                    
                }
                
                //----ship调整位姿（调整为up与扰动轴一致）
                {
                    if(pship->isTouchSurface(1.0)==false){
                        pship->setPosture_soft(pship->up_disturbed, 0.05);
                    }
                }
                
                
            }
        }
        /*   if(timeCounter.getCount()%60==0){
         cout<<"activeGroundBossCount:"<<activeGroundBossCount<<endl;
         cout<<"activeFlyBossCount:"<<activeFlyBossCount<<endl;
         }*/
    }
    
    //boss之间的碰撞
    {
        int n=(int)m_bossCellSet.getActorCount();
        for(int i=0;i<n;i++){
            CbossCell*pbossCell=(CbossCell*)m_bossCellSet.getActorByIndex(i);
            if(pbossCell->midFar||pbossCell->getIsVisible()==false||pbossCell->getIsDied())continue;
            Cc3dVector4 pos=pbossCell->getTransform().getPos();
            //获得pos附近的bossCell列表
            vector<Cc3dNode*> pNearByCellList=m_collisionGrid_boss.getCollisonList(pos);
            //bossCell与pNearByCellList中各boss进行碰撞检测
            int nNearBy=(int)pNearByCellList.size();
            for(int j=0;j<nNearBy;j++){
                CbossCell*_pbossCell=(CbossCell*)pNearByCellList[j];//不管_pbossCell是否active或visible都要检测碰撞
                if(_pbossCell==pbossCell)continue;//跳过自身
                //bossCell与_bossCell碰撞检测
                Cc3dVector4 _pos=_pbossCell->getTransform().getPos();
                float centerDis2=square(pos.x()-_pos.x())+square(pos.y()-_pos.y())+square(pos.z()-_pos.z());//中心距离的平方
                if(centerDis2<square(pbossCell->getRc()+_pbossCell->getRc())){//碰撞
                    //沿连心线弹开，并且二者都旋转
                    //求回退距离
                    float d_back=1;//          float d_back=(pbossCell->Rc+_pbossCell->Rc-sqrtf_fast(centerDis2))/2;
                    //求bossCell的反弹向量
                    Cc3dVector4 back=pos-_pos;
                    back=normalize(back);
                    back=back*d_back;
                    back.sety(0);//去掉Y分量(要在求出back以后)
                    //求_bossCell反弹向量
                    float _back[3]={-back.x(),-back.y(),-back.z()};
                    //对bossCell和_bossCell进行反弹
                    if(pbossCell->get_isFly()){
                        ( pbossCell->get_ship())->getTransformPointer()->moveRelativeToFather(back.x(), back.y(), back.z());
                    }else{
                        pbossCell->getTransformPointer()->moveRelativeToFather(back.x(), back.y(), back.z());
                    }
                    if(_pbossCell->get_isFly()){
                        ( _pbossCell->get_ship())->getTransformPointer()->moveRelativeToFather(_back[0], _back[1], _back[2]);
                    }else{
                        _pbossCell->getTransformPointer()->moveRelativeToFather(_back[0], _back[1], _back[2]);
                    }
                    //对bossCell和_bossCell进行旋转
                    if(pbossCell->get_isFly()){
                        ( pbossCell->get_ship())->rotLeft();
                    }else{
                        pbossCell->rotLeft();
                    }
                    if(_pbossCell->get_isFly()){
                        ( _pbossCell->get_ship())->rotRight();
                    }else{
                        _pbossCell->rotRight();
                    }
                    
                    break;//只要一个碰撞，其余的boss就不考虑了
                    
                }
            }
        }
        
    }
    //主角与飞船的碰撞
    if(cell->get_onShip()==false)
    {
        Cc3dVector4 cellpos=cell->getTransform().getPos();
        float cellRc=cell->getRc();
        int nShip=(int)m_shipSet.getActorCount();
        for(int i=0;i<nShip;i++){
            Cship*pShip= (Cship*)m_shipSet.getActorByIndex(i);
            //主角与pShip碰撞
            Cc3dVector4 shippos=pShip->getTransform().getPos();
            float shipRc_getOnShip=pShip->Rc_getOnShip;
            float d2=getLength2(cellpos-shippos);
            if(d2<=square(cellRc+shipRc_getOnShip)){//碰撞
                //上船
                cell->set_ship(pShip);
                m_goDownShipKey->setIsVisible(true);
                m_goDownShipKey->setIsDoUpdate(true);
                
                //声音
                //   pShip->pALSource_shipEngineSound->play();
                if(pShip->getALSourceByName("shipEngineSound")){
                    pShip->getALSourceByName("shipEngineSound")->play();
                }
                
                break;
            }
        }
    }
    //飞船与飞船碰撞
    {
        int n=(int)m_shipSet.getActorCount();
        for(int i=0;i<n;i++){
            Cship*pship= (Cship*)m_shipSet.getActorByIndex(i);
            Cc3dVector4 pos=pship->getTransform().getPos();
            for(int j=0;j<n;j++){
                Cship*_pship= (Cship*)m_shipSet.getActorByIndex(j);
                if(_pship==pship)continue;//跳过自身
                //pshipl与_pship碰撞检测
                //用Rc_small
                Cc3dVector4 _pos=_pship->getTransform().getPos();
                float centerDis2=square(pos.x()-_pos.x())+square(pos.y()-_pos.y())+square(pos.z()-_pos.z());//中心距离的平方
                if(centerDis2<square(pship->Rc_small+_pship->Rc_small)){//碰撞
                    //沿连心线弹开
                    //求回退距离
                    float d_back=(pship->Rc_small+_pship->Rc_small-sqrtf(centerDis2))/2;
                    //求pship的反弹向量
                    Cc3dVector4 e_back;
                    e_back=pos-_pos;
                    e_back=normalize(e_back);
                    Cc3dVector4 back;
                    back=e_back*d_back;
                    // //     back[1]=0;//ship之间的碰撞反映不只限制在平面上，所以不将y强制为0
                    //求_pship反弹向量
                    Cc3dVector4 _e_back(-e_back.x(),-e_back.y(),-e_back.z(),0);
                    Cc3dVector4 _back(-back.x(),-back.y(),-back.z(),0);
                    //对pship和_pship进行反弹
                    pship->getTransformPointer()->moveRelativeToFather(back.x(), back.y(), back.z());
                    _pship->getTransformPointer()->moveRelativeToFather(_back.x(), _back.y(), _back.z());
                    //再分别给pship和_pship一个速度
                    const float v_backLen=4;
                    Cc3dVector4 v_byOtherFoce=e_back*v_backLen;
                    Cc3dVector4 _v_byOtherFoce=_e_back*v_backLen;
                    
                    pship->v_byOhterFoce=v_byOtherFoce;
                    
                    _pship->v_byOhterFoce=_v_byOtherFoce;
                    
                    
                    break;//只要一个碰撞，其余的ship就不考虑了
                    
                }
            }
        }
        
        
    }
    //主角与boss碰撞
    {
        Cc3dVector4 pos=cell->getTransform().getPos();
        vector<Cc3dNode*> pNearByCellList=m_collisionGrid_boss.getCollisonList(pos);
        //cell与pNearByCellList中的boss进行碰撞
        int nNearBy=(int)pNearByCellList.size();
        for(int i=0;i<nNearBy;i++){
            CbossCell&_cell=*(CbossCell*)pNearByCellList[i];
            //_cell与cell碰撞
            Cc3dVector4 _pos=_cell.getTransform().getPos();
            float centerXZDis2=square(pos.x()-_pos.x())+square(pos.z()-_pos.z());//中心在xz平面上的投影的距离的平方
            float dH=pos.y()-_pos.y();//中心高度差
            if(fabsf(dH)<cell->Rc_small+_cell.Rc_small&&centerXZDis2<square(cell->getRc()+_cell.getRc())){//碰撞
                if(cell->onFire==false){
                    //沿连心线弹开
                    //求回退距离
                    float d_back=(cell->getRc()+_cell.getRc()-sqrtf(centerXZDis2))/2;//各退一半
                    d_back*=0.2;//乘以个系数，软回退
                    //求cell的反弹向量
                    Cc3dVector4 back=pos-_pos;
                    back=normalize(back);
                    back=back*d_back;
                    back.sety(0);//只在水平方向上后退
                    //求bossCell反弹向量
                    float _back[3]={-back.x(),-back.y(),-back.z()};
                    //对cell和bossCell进行反弹
                    cell->getTransformPointer()->moveRelativeToFather(back.x(), back.y(), back.z());
                    if(_cell.get_isFly()){//如果是飞行boss，移ship而不是boss
                        ( _cell.get_ship())->getTransformPointer()->moveRelativeToFather(_back[0], _back[1], _back[2]);
                    }else{
                        _cell.getTransformPointer()->moveRelativeToFather(_back[0], _back[1], _back[2]);
                    }
                    //主角少血
                    if(isHurtAble){
                        cell->life.dec(1);
                        //声音
                        // cell->pALSource_oops->playIfNotPlaying();
                        cell->getALSourceByName("oops")->playIfNotPlaying();
                    }
                    if(cell->life.value==0){
                        cell->setIsDied(true);
                        //  cell->setIsVisible(false);
                    }
                    //不break,其余boss也要继续考虑
                }else{//cell.onFire==true
                    //令bossCell死亡
                    _cell.life.decToZero();
                    _cell.setIsDied(true);
                    _cell.setIsVisible(false);
                    cell->nKillEnemyAdd();
                    //主角不少血
                    //声音
                    // _cell.pALSource_boss_scream->updatePosition(_cell.getTransform().getPos().getArray());
                    // _cell.pALSource_boss_scream->play();
                    _cell.getALSourceByName("boss_scream")->updatePosition(_cell.getTransform().getPos().getArray());
                    _cell.getALSourceByName("boss_scream")->play();
                }
            }
        }
    }
    //boss与ship碰撞
    {
        //各ship与各boss碰撞
        int nShip=(int)m_shipSet.getActorCount();
        for(int u=0;u<nShip;u++){
            Cship*pship= (Cship*)m_shipSet.getActorByIndex(u);
            Cc3dVector4 pos=pship->getTransform().getPos();
            vector<Cc3dNode*> pNearByCellList=m_collisionGrid_boss.getCollisonList(pos);
            //ship与pNearByCellList中的boss进行碰撞
            int nNearBy=(int)pNearByCellList.size();
            for(int i=0;i<nNearBy;i++){
                Ccell&_cell=*(Ccell*)pNearByCellList[i];
                //_cell与ship碰撞
                Cc3dVector4 _pos=_cell.getTransform().getPos();
                float centerDis2=square(pos.x()-_pos.x())+square(pos.y()-_pos.y())+square(pos.z()-_pos.z());//中心距离的平方
                if(centerDis2<square(cell->getRc()+_cell.getRc())){//碰撞
                    //_cell回退，ship不动
                    //心线弹开
                    //求回退距离
                    float d_back=(cell->getRc()+_cell.getRc()-sqrtf(centerDis2));
                    d_back*=0.8;//乘以个系数，软回退
                    //求_cell的反弹向量
                    Cc3dVector4 _back=_pos-pos;
                    _back=normalize(_back);
                    _back=_back*d_back;
                    _back.sety(0);//只在水平方向上后退
                    //_cell进行回退
                    _cell.getTransformPointer()->moveRelativeToFather(_back.x(), _back.y(), _back.z());
                    //如果此ship是主角所在ship，则主角要掉血
                    if(cell->get_ship()==pship){
                        //主角少血
                        if(isHurtAble){
                            cell->life.dec(1);
                            //声音
                            //cell->pALSource_oops->playIfNotPlaying();
                            cell->getALSourceByName("oops")->playIfNotPlaying();
                        }
                        if(cell->life.value==0){
                            cell->setIsDied(true);
                            cell->setIsVisible(false);
                        }
                        
                    }
                    //不break,其余boss也要继续考虑
                    
                }
            }
        }
    }
    
    //子弹与boss的碰撞
    {
        vector<CshotBall*> shotBallList=cell->getShotBallList();
        int nball=(int)shotBallList.size();
        for(int i=0;i<nball;i++){
            CshotBall&ball=*shotBallList[i];
            if(ball.getIsActive()==false){
                assert(ball.getIsVisible()==false);
                continue;
                
            }
            Cc3dVector4 ballpos=ball.getTransform().getPos();
            //获得ball周围的boss列表
            vector<Cc3dNode*> pNearByCellList=m_collisionGrid_boss.getCollisonList(ballpos);
            //ball与pNearByCellList中各boss碰撞
            int nNearBy=(int)pNearByCellList.size();
            for(int j=0;j<nNearBy;j++){
                Ccell&bossCell=*(Ccell*)pNearByCellList[j];
                if(bossCell.getIsDied())continue;
                //ball与bossCell碰撞
                Cc3dVector4 bosspos=bossCell.getTransform().getPos();
                float centerDis2=square(bosspos.x()-ballpos.x())+square(bosspos.y()-ballpos.y())+square(bosspos.z()-ballpos.z());//中心距离的平方
                if(centerDis2<square(bossCell.getRc())+square(ball.getRc()))
                {//碰撞
                    ball.turnToInActive();
                    ball.setIsVisible(false);
                    m_shotBlastSet.layBlast(ballpos,*camera,ball.get_blastType());
                    //bossCell减血
                    bossCell.life.dec(ball.get_hurt());
                    //敌人挨打声音
                    //bossCell.pALSource_boss_och->updatePosition(bossCell.getTransform().getPos());
                    //bossCell.pALSource_boss_och->play();
                    bossCell.getALSourceByName("boss_och")->updatePosition(bossCell.getTransform().getPos());
                    bossCell.getALSourceByName("boss_och")->play();
                    if(bossCell.life.value==0){
                        bossCell.setIsDied(true);
                        bossCell.setIsVisible(false);
                        cell->nKillEnemyAdd();
                        //敌人死亡声音
                        //  bossCell.pALSource_boss_scream->updatePosition(bossCell.getTransform().getPos());
                        //  bossCell.pALSource_boss_scream->play();
                        bossCell.getALSourceByName("boss_scream")->updatePosition(bossCell.getTransform().getPos());
                        bossCell.getALSourceByName("boss_scream")->play();
                    }
                    
                    break;//只要有一个碰撞，其余的boss就不考虑了
                    
                    
                }
            }
        }
    }
    //子弹与ship的碰撞（跳过cell所在ship）
    {
        vector<CshotBall*> shotBallList=cell->getShotBallList();
        int nball=(int)shotBallList.size();
        for(int i=0;i<nball;i++){
            CshotBall&ball=*shotBallList[i];
            if(ball.getIsActive()==false){
                assert(ball.getIsVisible()==false);
                continue;
            }
            Cc3dVector4 ballpos=ball.getTransform().getPos();
            //ball与shipSet中各ship碰撞
            int nShip=(int)m_shipSet.getActorCount();
            for(int j=0;j<nShip;j++){
                Cship*pship= (Cship*)m_shipSet.getActorByIndex(j);
                if(pship==cell->get_ship())continue;//跳过cell所在ship
                //ball与ship碰撞
                Cc3dVector4 shipPos=pship->getTransform().getPos();
                float centerDis2=square(shipPos.x()-ballpos.x())+square(shipPos.y()-ballpos.y())+square(shipPos.z()-ballpos.z());//中心距离的平方
                if(centerDis2<square(pship->Rc_small))//由于子弹半径很小，所以不用再加子弹半径
                {//碰撞
                    ball.turnToInActive();
                    ball.setIsVisible(false);
                    m_shotBlastSet.layBlast(ballpos, *camera,ball.get_blastType());
                    // ball.pALSource_collisionWithSolidSound->updatePosition(ball.getTransform().getPos());
                    //ball.pALSource_collisionWithSolidSound->play();
                    ball.getALSourceByName("collisionWithSolidSound")->updatePosition(ball.getTransform().getPos());
                    ball.getALSourceByName("collisionWithSolidSound")->playIfNotPlaying();
                    break;//只要有一个碰撞，其余的boss就不考虑了
                }
            }
        }
    }
    
    //传送门molecule计时
    {
        int nbubble=(int)m_bubbleSet.getActorCount();
        for (int i=0; i<nbubble; i++) {
            Cbubble*pbubble=(Cbubble*)m_bubbleSet.getActorByIndex(i);
            Cmolecule*pmolecule=pbubble->getMolecule();
            if(pmolecule->funcID==MOLECULE_FUNC_transport){
                pmolecule->revTimeCounter_restartTrans.doCount_dec();
            }
        }
    }
    //主角与bubble的碰撞
    {
        Cc3dVector4 cellpos=cell->getTransform().getPos();
        //获得cell周围的bubble列表
        vector<Cc3dNode*> pNearByCellList=m_collisionGrid_bubble.getCollisonList(cellpos);
        //cell与pNearByCellList中各bubble碰撞
        int nNearBy=(int)pNearByCellList.size();
        //    cout<<"nNearBy:"<<nNearBy<<endl;
        for(int j=0;j<nNearBy;j++){
            Cbubble&bubble=*(Cbubble*)pNearByCellList[j];
            //cell与bubble碰撞
            if(bubble.getDied()==false){
                const Cc3dVector4&bubblepos=bubble.getCenter();
                float centerDis2=square(bubblepos.x()-cellpos.x())+square(bubblepos.y()-cellpos.y())+square(bubblepos.z()-cellpos.z());//中心距离的平方
                if(centerDis2<square(bubble.getRc()+cell->getRc())
                   &&fabs(bubblepos.y()-cellpos.y())<bubble.getRc()+cell->Rc_small)//考虑主角是扁的
                {//碰撞
                    if(bubble.getMolecule()->funcID==MOLECULE_FUNC_transport){
                        //传送门不消失
                    }else{
                        bubble.setDied(true);
                        bubble.setIsVisible(false);
                    }
                    
                    cell->useFunc(bubble.getMolecule()->funcID);
                    if(bubble.getMolecule()->funcID==MOLECULE_FUNC_noMatch){
                        for(int i=0;i<(int)m_fireList.size();i++){
                            m_fireList[i]->setIsVisible(true);
                        }
                        //声音
                        //cell->pALSource_onFire->setGain(0.4);
                        //cell->pALSource_onFire->play();
                        cell->getALSourceByName("onFire")->setGain(0.4);
                        cell->getALSourceByName("onFire")->play();
                    }
                    if(bubble.getMolecule()->funcID==MOLECULE_FUNC_blast_and_lifeDec){
                        if(isHurtAble){
                            cell->life.dec(20);
                            //声音
                            //cell->pALSource_oops->playIfNotPlaying();
                            cell->getALSourceByName("oops")->playIfNotPlaying();
                        }
                        m_bubbleBlastSet.layBlast(bubblepos, *camera,BLASTTYPE_blast_big0);
                    }
                    if(bubble.getMolecule()->funcID==MOLECULE_FUNC_lifeAdd){
                        cell->life.add(40);
                    }
                    if(bubble.getMolecule()->funcID==MOLECULE_FUNC_coinAdd){
                        cell->nEateCoinAdd(1);
                    }
                    if(bubble.getMolecule()->funcID==MOLECULE_FUNC_checkPointUpdate){
                        m_checkPoint.save(*cell);
                    }
                    if(bubble.getMolecule()->funcID==MOLECULE_FUNC_transport//是传送molecule
                       &&bubble.getMolecule()->revTimeCounter_restartTrans.getCount()==0//且已恢复传送
                       ){
                        //在bubbleSet中找配对的transport bubble
                        int nBubble=(int)m_bubbleSet.getActorCount();
                        for(int k=0;k<nBubble;k++){
                            Cbubble&_bubble=*(Cbubble*)m_bubbleSet.getActorByIndex(k);
                            //看_bubble的molecule的startName是否等于bubble的molecule的destinationName
                            if(_bubble.getMolecule()->startName==bubble.getMolecule()->destinationName){
                                //_bubble是目的地
                                //将cell传送到_bubble处
                                //获得_bubble位置
                                Cc3dVector4 _bubblePos=_bubble.getPos();
                                //设定cell的位置为_bubblePos
                                cell->getTransformPointer()->setPos(_bubblePos.x(), _bubblePos.y(), _bubblePos.z());
                                if(cell->get_onShip()){//如果在船上，则船的位置也要移动
                                    ( cell->get_ship())->getTransformPointer()->setPos(_bubblePos.x(), _bubblePos.y(), _bubblePos.z());
                                }
                                //更新cell.jumpH_landH
                                cell->jumpH_landH.set(cell->getTransform().getPos().y());
                                //设置恢复传送倒计时
                                //注意，是对目的molecule(即_bubble->pmolecule)设置倒计时，而不是对源moluecule设置倒计时
                                _bubble.getMolecule()->revTimeCounter_restartTrans.setValue(RESTARTTRANSCOUNTVALUE);
                                break;//不用再继续搜索了
                            }
                            
                        }
                        
                    }
                    if(bubble.getMolecule()->funcID==MOLECULE_FUNC_keyAdd){
                        if(cell->get_isAllKeyGot()==false){//如果还没有获得所有key，则播放声音
                            // bubble.pALSource_eaten->updatePosition(bubble.getPos());
                            // bubble.pALSource_eaten->play();
                            bubble.getALSourceByName("eaten")->updatePosition(bubble.getPos());
                            bubble.getALSourceByName("eaten")->play();
                        }
                    }else{
                        //bubble.pALSource_eaten->updatePosition(bubble.getPos());
                        //bubble.pALSource_eaten->play();
                        if(bubble.getALSourceByName("eaten")){
                            bubble.getALSourceByName("eaten")->updatePosition(bubble.getPos());
                            bubble.getALSourceByName("eaten")->play();
                        }
                    }
                    break;//只要有一个碰撞，其余的bubble就不考虑了
                }
            }
        }
    }
    
    //子弹与bubble的碰撞
    {
        vector<CshotBall*> shotBallList=cell->getShotBallList();
        int nball=(int)shotBallList.size();
        for(int i=0;i<nball;i++){
            CshotBall&ball=*shotBallList[i];
            if(ball.getIsActive()==false){
                assert(ball.getIsVisible()==false);
                continue;
            }
            Cc3dVector4 ballpos=ball.getTransform().getPos();
            //获得ball周围的bubble列表
            vector<Cc3dNode*> pNearByCellList=m_collisionGrid_bubble.getCollisonList(ballpos);
            //ball与pNearByCellList中各bbubble碰撞
            int nNearBy=(int)pNearByCellList.size();
            for(int j=0;j<nNearBy;j++){
                Cbubble&bubble=*(Cbubble*)pNearByCellList[j];
                if(bubble.getDied())continue;
                //ball与bbubble碰撞
                const Cc3dVector4&bubblepos=bubble.getCenter();
                float centerDis2=square(bubblepos.x()-ballpos.x())+square(bubblepos.y()-ballpos.y())+square(bubblepos.z()-ballpos.z());//中心距离的平方
                if(centerDis2<square(bubble.getRc()))//由于子弹半径很小，所以不用再加子弹半径
                {//碰撞
                    //子弹消失并放置burst
                    ball.turnToInActive();
                    ball.setIsVisible(false);
                    m_shotBlastSet.layBlast(ballpos, *camera,ball.get_blastType());
                    //声音
                    //ball.pALSource_collisionWithSolidSound->updatePosition(ball.getTransform().getPos());
                    //ball.pALSource_collisionWithSolidSound->play();
                    if(ball.getALSourceByName("collisionWithSolidSound")){
                        //xxx why?
                        ball.getALSourceByName("collisionWithSolidSound")->updatePosition(ball.getTransform().getPos());
                        ball.getALSourceByName("collisionWithSolidSound")->playIfNotPlaying();
                    }
                    //跳过不接受子弹触发的bubble
                    if(bubble.getMolecule()!=NULL&&bubble.getMolecule()->funcID==MOLECULE_FUNC_transport)continue;//跳过传送门
                    if(bubble.getMolecule()!=NULL&&bubble.getMolecule()->funcID==MOLECULE_FUNC_checkPointUpdate)continue;//跳过检查点
                    if(bubble.getMolecule()!=NULL&&bubble.getMolecule()->funcID==MOLECULE_FUNC_keyAdd)continue;//跳过钥匙
                    //bubble生命减少
                    bubble.life.dec(1);
                    if(bubble.life.value==0){
                        bubble.setDied(true);
                        bubble.setIsVisible(false);
                        cell->useFunc(bubble.getMolecule()->funcID);//主角获得技能
                        if(bubble.getMolecule()->funcID==MOLECULE_FUNC_blast_and_lifeDec){
                            m_bubbleBlastSet.layBlast(bubblepos, *camera,BLASTTYPE_blast_big0);
                        }
                        if(bubble.getMolecule()->funcID==MOLECULE_FUNC_lifeAdd){
                            cell->life.add(40);
                        }
                        if(bubble.getMolecule()->funcID==MOLECULE_FUNC_coinAdd){
                            cell->nEateCoinAdd(1);
                        }
                        //声音
                        //bubble.pALSource_eaten->play();
                        bubble.getALSourceByName("eaten")->play();
                    }
                    break;//只要有一个碰撞，其余的bubble就不考虑了
                    
                    
                }
            }
        }
    }
    
    
    //----更新cell.pmoleculeList中各元素的位置及朝向
    if(cell->onFire)//仅当onFire时才执行
    {
        Cc3dVector4 pos=cell->getTransform().getPos();
        //计算cell.pmoleculeList[0～2]的位置
        {
            //求cell的法向量
            
            float cellNorm_local[4]={0,1,0,0};
            Cc3dVector4 cellNorm=cell->getTransform().convertToWorldSpace(cellNorm_local);
            //求视点到cell中心的向量
            Cc3dVector4 eyeToCell_e;//由视点指向cell中心的单位向量
            const Cc3dVector4&cellpos=pos;
            const Cc3dVector4&eyePos=camera->getEyePos();
            eyeToCell_e=cellpos-eyePos;
            eyeToCell_e=normalize(eyeToCell_e);//得到eyeToCell_e
            if(eyeToCell_e.y()>-0.7){
                eyeToCell_e.sety(0);
            }
            //由cellNorm和eyeToCell_e叉乘得到与cell在公告牌上的投影平行的向量
            const float R=cell->getRc();
            Cc3dVector4 rightArmVec,leftArmVec;
            rightArmVec=cross(eyeToCell_e, cellNorm);
            rightArmVec=rightArmVec*R;//得到rightArmVec
            leftArmVec=Cc3dVector4(0,0,0,0)-rightArmVec;//得到leftArmVec
            //类似地，计算backArmVec和backArmEnd
            Cc3dVector4 backArmVec=eyeToCell_e*R;
            Cc3dVector4 backArmEnd=cellpos+backArmVec;
            //计算backRightArmEnd和backLeftArmEnd
            Cc3dVector4 backRightArmEnd,backLeftArmEnd;
            backRightArmEnd=rightArmVec+backArmVec;
            backRightArmEnd=backRightArmEnd*0.7071;
            backRightArmEnd=cellpos+backRightArmEnd;//得到backRightArmEnd
            backLeftArmEnd=leftArmVec+backArmVec;
            backLeftArmEnd=backLeftArmEnd*0.7071;
            backLeftArmEnd=cellpos+backLeftArmEnd;//得到backLeftArmEnd
            //将backRightArmEnd、backLeftArmEnd和backArmEnd用作cell.pmoleculeList[0～2]的位置
            m_fireList[1]->setPos(backRightArmEnd);
            m_fireList[2]->setPos(backLeftArmEnd);
            m_fireList[0]->setPos(backArmEnd);
        }
        //更新cell.pmoleculeList[0~2]的朝向，注意，不传camera.pos，而传cellpos
        const float *cellpos=pos.getArray();
        m_fireList[0]->adjustFace_centerAtBottomAndSubmitVertex(cellpos);
        m_fireList[1]->adjustFace_centerAtBottomAndSubmitVertex(cellpos);
        m_fireList[2]->adjustFace_centerAtBottomAndSubmitVertex(cellpos);
        
    }
    
    
    
    //----更新生命条(必须放在末尾)
    {
        m_lifeBar->update((float)cell->life.value/cell->life.maxValue, GL_STREAM_DRAW);
    }
    //----更新燃料条
    {
        m_fuelBar->update((float)cell->fuel.value/cell->fuel.maxValue, GL_STREAM_DRAW);
    }
    //处理声音
    makeSound();
    
    
}
