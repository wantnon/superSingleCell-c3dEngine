//-------------------------------------
// do not use chinese in comment
//-------------------------------------
varying mediump vec2 texCoordOut;
uniform lowp vec4 color;
uniform sampler2D Texture;
void main(void) {
    gl_FragColor = color*texture2D(Texture, texCoordOut); 
}