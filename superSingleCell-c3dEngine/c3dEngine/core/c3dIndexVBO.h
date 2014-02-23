//
//  Cc3dIndexVBO.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#ifndef __HelloOpenGL__Cc3dIndexVBO__
#define __HelloOpenGL__Cc3dIndexVBO__

#include <iostream>
#include <vector>
using namespace std;
#include "c3dTransform.h"
#include "c3dCamera.h"
#include "c3dLight.h"
#include "c3dVertex.h"
#include "c3dIDtriangle.h"
class Cc3dIndexVBO:public Cc3dObject
{
public:
    GLuint vertexbuffer;
    GLuint indexbuffer;
    int m_vertexCount;
    int m_indexCount;
    Cc3dIndexVBO(){
        vertexbuffer=0;
        indexbuffer=0;
        m_vertexCount=0;
        m_indexCount=0;
    }
    virtual~ Cc3dIndexVBO(){
        if(vertexbuffer!=0)glDeleteBuffers(1,&vertexbuffer);
        if(vertexbuffer!=0)glDeleteBuffers(1,&indexbuffer);
    }
    void genBuffers(){
        assert(vertexbuffer==0);
        assert(indexbuffer==0);
        glGenBuffers(1,&vertexbuffer);
        glGenBuffers(1,&indexbuffer);
    }

   void submitVertex(const vector<Cc3dVertex> &vlist,GLenum usage)
    {
        int vertexCount=(int)vlist.size();
        //----提交到显卡
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexCount*(3+2+3+2), &vlist[0], usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑定
        m_vertexCount=vertexCount;
        
    }
    void submitVertex(int vertexCount,const float vertexArray[],GLenum usage)
    {
        //----提交到显卡
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(float)*(3+2+3+2), vertexArray, usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑定
        m_vertexCount=vertexCount;
        
    }

    
    void submitIndex(const vector<Cc3dIDTriangle> &IDtriList,GLenum usage)
    {
        //填充indexCount
        int nIDtri=(int)IDtriList.size();
        int indexCount=3*nIDtri;
        //----由索引数组生成索引缓存
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indexCount, &IDtriList[0], usage);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//解绑定
        m_indexCount=indexCount;
    }
    void bindVertexBuffer(){
        //指定要绘制的顶点缓存
  
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    }
    void unbindVertexBuffer(){
        //指定要绘制的顶点缓存
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
   

        
    static void setPointers()//new
    //指定program中各属性在顶点缓存中的分布
    //必须放在bindVertexBuffer之后
    {
        const int posStep=3;
        const int texCoordStep=2;
        const int normStep=3;
        const int texCoord2Step=2;
        const int step=posStep+texCoordStep+normStep+texCoord2Step;
        glVertexAttribPointer(ATTRIB_LOC_position_local, posStep, GL_FLOAT, GL_FALSE, sizeof(float)*step,0);
        glVertexAttribPointer(ATTRIB_LOC_texCoord, texCoordStep, GL_FLOAT, GL_FALSE,sizeof(float)*step, (GLvoid*) (sizeof(float)* posStep));
        glVertexAttribPointer(ATTRIB_LOC_normal_local, normStep, GL_FLOAT, GL_FALSE, sizeof(float)*step, (GLvoid*) (sizeof(float) * (posStep+texCoordStep)));
        glVertexAttribPointer(ATTRIB_LOC_texCoord2, texCoord2Step, GL_FLOAT, GL_FALSE, sizeof(float)*step, (GLvoid*) (sizeof(float) * (posStep+texCoordStep+normStep)));
        
    }


    void bindIndexBuffer(){
        //指定要绘制的索引缓存
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    }
    void unbindIndexBuffer(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
 

    void drawIndexBuffer(GLenum mode=GL_TRIANGLES)
    {
        //绘制索引缓存
        if(m_indexCount!=0){
            assert(m_vertexCount!=0);
            //如果m_indexCount!=0而m_vertexCount==0，则说明忘记了提交顶点数据就开始画了
        }
        glDrawElements(mode,m_indexCount,GL_UNSIGNED_INT, 0);
        //最后一个参数Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
        //即使用的索引数据是int型，也要用GL_UNSIGNED_INT
        //第一个参数Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES,
        //GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
        //GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY
        //and GL_PATCHES are accepted.
        //注意其中没有GL_QUADS，但是多了个GL_PATCHES，这个还不会用。

    }

     
    
};

#endif /* defined(__HelloOpenGL__Cc3dIndexVBO__) */
