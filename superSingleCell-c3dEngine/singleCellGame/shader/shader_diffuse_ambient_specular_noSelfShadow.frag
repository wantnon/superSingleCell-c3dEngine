//-------------------------------------
// do not use chinese in comment
//-------------------------------------
varying lowp vec4 mainColor;
varying lowp vec4 secondaryColor;
varying mediump vec2 texCoordOut;
varying highp vec4 lightViewportTexCoordDivW;

uniform sampler2D Texture;
uniform sampler2D Texture_shadowMap;

void main(void) {
    lowp float shadowFactor;
    highp float zLookup=texture2D(Texture_shadowMap,lightViewportTexCoordDivW.st).z;
    
    highp float zTolight=lightViewportTexCoordDivW.z;
    shadowFactor=((zTolight>zLookup)?0.65:1.0);

    lowp vec4 shadowFactorRGBA=vec4(shadowFactor,shadowFactor,shadowFactor,1.0);
    gl_FragColor = shadowFactorRGBA*mainColor * texture2D(Texture, texCoordOut)+secondaryColor;
    
}