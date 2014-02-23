//-------------------------------------
// do not use chinese in comment
//-------------------------------------
varying mediump vec2 texCoordOut;
uniform sampler2D Texture;
void main(void) {
    gl_FragColor = texture2D(Texture, texCoordOut);
}