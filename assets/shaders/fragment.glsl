#version 400

in vec4 o_Color;
in vec2 o_TexCoord;
in float o_TexIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

void main()
{
    int index = int(o_TexIndex);
    FragColor = texture(u_Textures[index], o_TexCoord) * o_Color;
}