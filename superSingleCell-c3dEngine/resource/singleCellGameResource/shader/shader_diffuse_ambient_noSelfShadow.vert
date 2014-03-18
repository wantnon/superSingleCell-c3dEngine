//-------------------------------------
// do not use chinese in comment
//-------------------------------------
//attribute pass from vbo or va
attribute vec3 position_local;
attribute vec3 normal_local;
attribute vec2 texCoordIn; 

//matrixs pass from outside
uniform mat4 projectionModelview;
uniform mat4 modelMat;
uniform mat4 normMat_toWorld;
uniform mat4 worldToLightViewportTexCoord;

//vectors and scalars pass from outside
uniform vec3 lightPos_world;
uniform vec4 diffuseML;//diffuseML=vec4(vec3(diffuse_material)*vec3(diffuse_light),diffuse_material.a)
uniform vec4 ambientML;//ambientML=vec4(vec3(ambient_material)*vec3(ambient_light),0)

//pass to fragment shader
varying vec4 mainColor;
varying vec2 texCoordOut;
varying vec4 lightViewportTexCoordDivW;

void main(void) {
//----get normal in world space
    //suppose no scaling, then we can spare normalize
    vec3 norm_world = vec3(normMat_toWorld*vec4(normal_local,0));
    //----get pos in world space
    vec3 pos_world = vec3(modelMat*vec4(position_local,1));
    //----cal diffuse color
    vec3 posToLight=normalize(lightPos_world-pos_world);
    float normDotPosToLight = max(0.0, dot(norm_world, posToLight));
    vec3 diffuseColor= normDotPosToLight*vec3(diffuseML);
    //----cal ambient color
    //vec3(ambientML)
    //----set varying
    //the final alpha is equal to diffuseML.a(and is equal to diffuse_material.a)
    mainColor = vec4(vec3(ambientML)+diffuseColor,diffuseML.a);
    gl_Position = projectionModelview* vec4(position_local,1);
    texCoordOut = texCoordIn; 
    vec4 t=worldToLightViewportTexCoord*vec4(pos_world,1);//note: use pos_world

   lightViewportTexCoordDivW=t/t.w;
}