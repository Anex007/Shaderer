#version 460 core
layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoord;

//out vec3 fragPos;
out vec2 TexCoord;

void main()
{
    //fragPos = vec3(u_model * vec4(a_Pos, 1.0));
    TexCoord = a_TexCoord;
    gl_Position = vec4(a_Pos, 0.0, 1.0);
}