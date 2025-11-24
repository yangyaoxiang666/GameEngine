//片段着色器
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
//告诉Gpu三角形的颜色（橘色）