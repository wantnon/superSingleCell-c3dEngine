//-------------------------------------
// do not use chinese in comment
//-------------------------------------
//attribute pass from vbo or va
attribute  vec3 position_local;

//matrixs pass from outside
uniform mat4 projectionModelview;


void main(void) {
    gl_Position = projectionModelview* vec4(position_local,1);
}