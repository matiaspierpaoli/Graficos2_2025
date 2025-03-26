#version 330 core
layout(location = 0) in vec4 aPos;   // the position variable has attribute position 0
layout(location = 1) in vec2 spriteCoord;   // the uv variable has attribute position 1

out vec2 v_SpriteCoord; // output a color to the fragment shader

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * aPos;
    v_SpriteCoord = spriteCoord; // set v_SpriteCoord to the input uv we got from the vertex data
}