//-------------------------------------
// do not use chinese in comment
//-------------------------------------
//attribute pass from vbo or va
attribute vec3 position_local;
attribute vec3 normal_local;
attribute vec2 texCoordIn; 

//matrixs pass from outside
uniform mat4 projectionModelview;
uniform mat4 worldToLightViewportTexCoord;
uniform mat4 worldToLightViewportTexCoord2;

//vectors and scalars pass from outside
uniform vec3 lightPos_world;
uniform vec4 diffuseML;//diffuseML=vec4(vec3(diffuse_material)*vec3(diffuse_light),diffuse_material.a)
uniform vec4 ambientML;//ambientML=vec4(vec3(ambient_material)*vec3(ambient_light),0)

//pass to fragment shader
varying vec4 mainColor;
varying vec2 texCoordOut;
varying vec3 lightViewportTexCoordDivW;
varying vec3 lightViewportTexCoordDivW2;

void main(void) {
    //use normal_local as world space value
    //use position_local as world space value
    //----cal diffuse color
    vec3 posToLight=normalize(lightPos_world-position_local);
    float normDotPosToLight = max(0.0, dot(normal_local, posToLight));
    vec3 diffuseColor= normDotPosToLight*vec3(diffuseML);
    //----cal ambient color
    //use vec3(ambientML) directly
    //----set varying
    //the final alpha is equal to diffuseML.a(and is equal to diffuse_material.a)
    mainColor = vec4(vec3(ambientML)+diffuseColor,diffuseML.a);
    gl_Position = projectionModelview* vec4(position_local,1);
    texCoordOut = texCoordIn;
    vec4 t=worldToLightViewportTexCoord*vec4(position_local,1);
    lightViewportTexCoordDivW=vec3(t)/t.w;
    vec4 t2=worldToLightViewportTexCoord2*vec4(position_local,1);
    lightViewportTexCoordDivW2=vec3(t2)/t2.w;
}
