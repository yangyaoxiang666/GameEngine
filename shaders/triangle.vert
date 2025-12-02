#version 330 core

layout(location = 0) in vec3 aPos;

uniform float u_OffsetY;   // 控制上下移动

void main()
{
    vec3 pos = aPos;
    pos.y += u_OffsetY;    // 往上/下偏移
    gl_Position = vec4(pos, 1.0);
}
