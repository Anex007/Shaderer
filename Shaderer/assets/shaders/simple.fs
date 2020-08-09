#version 460 core 

in vec4 gl_FragCoord;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec2 u_Resolution;
uniform vec4 u_Mouse;
uniform float u_Time;
uniform uint u_Frame;
uniform float u_DeltaTime;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

/* Only Modify Whatever is below this line */

void main()
{
	FragColor = vec4(0.419, 0.196, 0.823, 1.0);
};