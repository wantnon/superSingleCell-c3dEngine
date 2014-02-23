
varying mediump vec2 texCoordOut;
uniform sampler2D Texture;
void main(void) {
    lowp float alpha=texture2D(Texture,texCoordOut).r;
    gl_FragColor =vec4(1.0,1.0,1.0,alpha);
    
}