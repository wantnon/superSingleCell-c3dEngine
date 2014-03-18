//-------------------------------------
// do not use chinese in comment
//-------------------------------------
//attribute pass from vbo or va
attribute vec3 position_local;
attribute vec2 texCoordIn;

//matrixs pass from outside
uniform mat4 projectionModelview;

//pass to fragment shader
varying vec2 texCoordOut;

void main(void) {
    gl_Position = projectionModelview* vec4(position_local,1);
    texCoordOut = texCoordIn;
}