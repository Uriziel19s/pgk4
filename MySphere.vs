#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) in vec3 aPos;

layout(location = 0) uniform mat4  projection;
layout(location = 1) uniform mat4  view;
layout(location = 2) uniform mat4 model;
layout(location = 3) uniform float scale;
out vec3 vColor;

void main(void)
        {
            if(gl_VertexID%3 == 0)
            {
            vColor = vec3(1, 0, 0);
            }
            else if(gl_VertexID%3 == 1)
            {
                vColor = vec3(0, 1, 0);
            }
            else
            {
                vColor = vec3(0, 0, 1);
            }
            gl_Position =  projection * view * model * vec4(aPos * scale, 1.0f);

        }
