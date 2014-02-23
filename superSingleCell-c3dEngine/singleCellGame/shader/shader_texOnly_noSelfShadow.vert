//-------------------------------------
// do not use chinese in comment
//-------------------------------------
//attribute pass from vbo or va
attribute vec3 position_local;
attribute vec2 texCoordIn;

//matrixs pass from outside
uniform mat4 projectionModelview;
uniform mat4 modelMat;
uniform mat4 worldToLightViewportTexCoord;

//vectors and scalars pass from outside
uniform vec3 lightPos_world;

//pass to fragment shader
varying vec2 texCoordOut;
varying vec4 lightViewportTexCoordDivW;

void main(void) {
    //----get pos in world space
    vec3 pos_world = vec3(modelMat*vec4(position_local,1));
    //----set varying

    gl_Position = projectionModelview* vec4(position_local,1);
    texCoordOut = texCoordIn;
    vec4 t=worldToLightViewportTexCoord*vec4(pos_world,1);//note: use pos_world
    
    lightViewportTexCoordDivW=t/t.w;
}