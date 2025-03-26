#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_SpriteCoord;

uniform sampler2D u_Sprite;

void main()
{
   vec4 texColor = texture(u_Sprite, v_SpriteCoord);
   color = texColor;
}