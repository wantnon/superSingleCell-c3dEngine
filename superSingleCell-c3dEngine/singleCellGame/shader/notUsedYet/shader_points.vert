
//-------------------------------------
// do not use chinese in comment
//-------------------------------------
//attribute pass from vbo or va
attribute vec3 position_local;
attribute vec2 texCoordIn;

//uniform
uniform mat4 projectionModelview;
uniform float pointSize;

//pass to fragment shader
varying vec2 texCoordOut;

void main(void) {
    gl_PointSize=pointSize;//gl_PointSize is float, see: http://www.opengl.org/sdk/docs/manglsl/xhtml/gl_PointSize.xml
    gl_Position = projectionModelview* vec4(position_local,1);
    texCoordOut = texCoordIn;
}