#version 400

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_texIndex;

out vec4 o_Color;
out vec2 o_TexCoord;
out float o_TexIndex;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(a_Pos, 0.0, 1.0);

    o_Color = a_Color;
    o_TexCoord = a_TexCoord;
    o_TexIndex = a_texIndex;
}