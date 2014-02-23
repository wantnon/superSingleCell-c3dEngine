//-------------------------------------
// do not use chinese in comment
//-------------------------------------
varying mediump vec2 texCoordOut;
uniform sampler2D Texture;
void main() 
{ 
lowp float depth=texture2D(Texture,texCoordOut).r; 
gl_FragColor=vec4(vec3(pow(depth,100.0)),1.0);
} 