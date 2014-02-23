//-------------------------------------
// do not use chinese in comment
//-------------------------------------
varying lowp vec4 mainColor;
varying lowp vec4 secondaryColor;
varying mediump vec2 texCoordOut;
uniform sampler2D Texture; 

void main(void) {
    gl_FragColor = mainColor * texture2D(Texture, texCoordOut)+secondaryColor; 
}