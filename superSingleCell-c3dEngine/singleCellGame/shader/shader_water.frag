//-------------------------------------
// do not use chinese in comment
//-------------------------------------
varying mediump vec2 texCoordOut;
uniform sampler2D Texture;
uniform lowp float maxAlpha;

void main(void) {
    lowp vec3 texColor=vec3(texture2D(Texture, texCoordOut));
 
    gl_FragColor = vec4(texColor,maxAlpha);
}
