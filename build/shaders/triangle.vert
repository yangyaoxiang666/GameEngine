//顶点着色器
#version 330 core
layout(location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
//告诉Gpu三角形三个顶点的位置