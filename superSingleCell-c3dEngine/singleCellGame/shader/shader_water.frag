//-------------------------------------
// do not use chinese in comment
//-------------------------------------
varying mediump vec2 texCoordOut;
//varying mediump vec2 texCoordOut2;
uniform sampler2D Texture;
//uniform sampler2D alphaMap;
uniform lowp float maxAlpha;

void main(void) {
    lowp vec3 texColor=vec3(texture2D(Texture, texCoordOut));
   // lowp float alpha=texture2D(alphaMap, texCoordOut2).r;
  //  gl_FragColor = vec4(texColor,min(alpha,maxAlpha));
 
    gl_FragColor = vec4(texColor,maxAlpha);
}
