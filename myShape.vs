#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) in vec3 aPos;
layout(location = 1) in mat4 model;
layout(location = 0) uniform mat4  view;
layout(location = 1) uniform mat4  projection;

void main(void)
	{
            gl_Position =  projection * view * model * vec4(aPos * 0.1, 1.0f);
        }

