//
//  key.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-17.
//
//

#ifndef __HelloOpenGL__key__
#define __HelloOpenGL__key__

#include <iostream>
#include "c3dMesh.h"
#include "timeCounter.h"
#include "c3dActor.h"
class Ckey:public Cc3dActor
{
public:
    float keyDownColor[4];
    float keyUpColor[4];
    float xmin,xmax,ymin,ymax;
    float transpColor[4];
    bool isGrey;//变灰
 //   bool invisible;//不可见（同时也不再起作用）
    bool haveBeenDisplayed;//已经显示过
    CtimeCounter disapearCounter;//多长时间消失
    bool isDown;
    Ckey(){
        haveBeenDisplayed=false;
        isGrey=false;
   //     invisible=false;
        isDown=false;
        transpColor[0]=0;
        transpColor[1]=0;
        transpColor[2]=0;
        transpColor[3]=0;
        
        keyDownColor[0]=0;
        keyDownColor[1]=0;
        keyDownColor[2]=0;
        keyDownColor[3]=1;
   
        keyUpColor[0]=1;
        keyUpColor[1]=1;
        keyUpColor[2]=1;
        keyUpColor[3]=1;

        xmin=xmax=ymin=ymax=0;
    }
    void setIsDown(bool value){
        isDown=value;
    }
    bool getIsDown()const{return isDown;}
    void disapearCount(){
        disapearCounter.doCount_dec();
        if(disapearCounter.getCount()==0){
      //      invisible=true;
            setIsVisible(false);
        }
    }
    void setDisapearCountValue(long disapearCountValue){
        disapearCounter.setValue(disapearCountValue);
    }
    void set_isGrey(bool _isGrey){
        isGrey=_isGrey;
    }
    bool get_isGrey()const{
        return isGrey;
    }
 /*   void set_invisible(bool _invisible){
        invisible=_invisible;
    }
    bool get_invisible()const{
        return invisible;
    }*/

    void reSetWidth(float _width){//重设宽度
        xmax=xmin+_width;
        //更新顶点坐标(1号顶点和2号顶点)
        Cc3dVertex&v1=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist[1];
        Cc3dVertex&v2=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist[2];
        Cc3dVector3 pos1=v1.getPos();
        Cc3dVector3 pos2=v2.getPos();
        pos1.setx(xmax);
        pos2.setx(xmax);
        v1.setPos(pos1);
        v2.setPos(pos2);
    
    }
    void reSetHeight(float _height){//重设高度
        ymax=ymin+_height;
        //更新顶点坐标(0号顶点和1号顶点)
        Cc3dVertex&v2=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist[2];
        Cc3dVertex&v3=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist[3];
        Cc3dVector3 pos2=v2.getPos();
        Cc3dVector3 pos3=v3.getPos();
        pos2.sety(ymax);
        pos3.sety(ymax);
        v2.setPos(pos2);
        v3.setPos(pos3);
        
    }
    float getCenterX()const {//获得中心x坐标
        return (xmin+xmax)/2;
    }
    float getCenterY()const {//获得中心y坐标
        return (ymin+ymax)/2;
    }
    void reSetCenter(float x,float y){//重设位置
        //求原中心位置
        float xf=(xmin+xmax)/2;
        float yf=(ymin+ymax)/2;
        //求平移量
        float dx=x-xf;
        float dy=y-yf;
        //更新范围
        xmin+=dx;
        xmax+=dx;
        ymin+=dy;
        ymax+=dy;
        //更新顶点
        int nv=(int)getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist.size();
        for(int i=0;i<nv;i++){
            Cc3dVertex&v=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist[i];
            Cc3dVector3 pos=v.getPos();
            pos.setx(pos.x()+dx);
            pos.sety(pos.y()+dy);
            v.setPos(pos);
        }
    
    }
    void reSetLeftDownCorner(float x,float y){//重设位置
        //求原左下角位置
        float xf=xmin;
        float yf=ymin;
        //求平移量
        float dx=x-xf;
        float dy=y-yf;
        //更新范围
        xmin+=dx;
        xmax+=dx;
        ymin+=dy;
        ymax+=dy;
        //更新顶点
        int nv=(int)getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist.size();
        for(int i=0;i<nv;i++){
            Cc3dVertex&v=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist[i];
            Cc3dVector3 pos=v.getPos();
            pos.setx(pos.x()+dx);
            pos.sety(pos.y()+dy);
            v.setPos(pos);
        }
        
    }

    void genAndInit(
                    float _xmin,float _xmax,float _ymin,float _ymax,//坐标范围
                    float _smin,float _smax,float _tmin,float _tmax//纹理坐标范围
               ){
                   assert((int)this->getModel()->getMeshCount()==0);
        xmin=_xmin;
        xmax=_xmax;
        ymin=_ymin;
        ymax=_ymax;
        //制作网格
        Cc3dMesh*mesh=new Cc3dMesh();
                   mesh->autorelease();
                   mesh->init();
        this->addMesh(mesh);
        Cc3dSubMesh*subMesh=new Cc3dSubMesh();
                   subMesh->autorelease();
                   subMesh->init();
        this->getModel()->getMeshByIndex(0)->addSubMesh(subMesh);
        float vertexArray[4][3+2+3+2]={
            //顶点z值取1，落在正交投影的近剪裁面，这样可以防止UI元素被场景物体裁切
            //当然，还有一个办法就是在绘制UI元素的时候禁用深度测试（当UI元素较多时值得这样做）
            {xmin,ymin,1,_smin,_tmax,0,0,0,0,0},//左下角
            {xmax,ymin,1,_smax,_tmax,0,0,0,0,0},
            {xmax,ymax,1,_smax,_tmin,0,0,0,0,0},
            {xmin,ymax,1,_smin,_tmin,0,0,0,0,0}
        };
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist.resize(4);
        memcpy(&(getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->vlist[0]), vertexArray, sizeof(vertexArray));
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->IDtriList.clear();
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->addIDtri(0, 1, 2);
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->addIDtri(0, 2, 3);
    }
    bool isContainPoint(const Cc3dVector2&point)
    {
        float x=point.x();
        float y=point.y();
        if (x>=xmin&&x<=xmax&&y>=ymin&&y<=ymax){
            return true;
        }else{
            return false;
        }
    }
    void setKeyDownColor(float r,float g,float b,float a){
        keyDownColor[0]=r;
        keyDownColor[1]=g;
        keyDownColor[2]=b;
        keyDownColor[3]=a;
        
    }
    void setKeyUpColor(float r,float g,float b,float a){
        keyUpColor[0]=r;
        keyUpColor[1]=g;
        keyUpColor[2]=b;
        keyUpColor[3]=a;
        
    }
    void useKeyDownColor(){
        this->getModel()->getMeshByIndex(0)->setColor(keyDownColor[0],keyDownColor[1],keyDownColor[2],keyDownColor[3]);
        
    }
    void useKeyUpColor(){
        this->getModel()->getMeshByIndex(0)->setColor(keyUpColor[0],keyUpColor[1],keyUpColor[2],keyUpColor[3]);
    }

    
};
#endif /* defined(__HelloOpenGL__key__) */
