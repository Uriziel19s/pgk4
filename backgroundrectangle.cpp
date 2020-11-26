#include "backgroundrectangle.h"



BackgroundRectangle::BackgroundRectangle(glm::vec3 centerOfCube) : centerOfCUbe(centerOfCube)
{
    setShaders();
    setBuffers();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(centerOfCube));
}
void BackgroundRectangle::setShaders()
{
    compileShaders(R"END(


                   #version 330
                   #extension GL_ARB_explicit_uniform_location : require
                   #extension GL_ARB_shading_language_420pack : require

                   layout(location = 0) in vec3 aPos;
                   layout(location = 0) uniform float scale;
                   layout(location = 1) uniform mat4  projection;
                   layout(location = 2) uniform mat4  view;
                   layout(location = 3) uniform mat4  model;
                   layout(location = 4) uniform float playerDistance;
                   out vec3 vColor;

                   void main(void)
                   {
                       vec4 temp = model * (vec4(aPos * scale, 1.0f));
                       float len = length(temp);
                       vColor = vec3(abs(temp.x / len)/2 / playerDistance, abs(temp.y / len)/2 / playerDistance, abs(temp.z / len)/2 / playerDistance);
                       gl_Position =  projection * view * model * vec4(aPos * scale, 1.0f);

                   }


                   )END", R"END(

                          #version 330
                          in vec3 vColor;

                          void main(void)
                          {
                              gl_FragColor = vec4(vColor, 1);

                          }


                          )END");
                          }



void BackgroundRectangle::setBuffers()
{
    bindBuffers();
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                 // attribute 0, must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,//24,             // stride
                (void*)0            // array buffer offset
                );
}


void BackgroundRectangle::draw(float scale, const glm::mat4 &view, const glm::mat4 projection, float playerNormalizedDistance)
{
    bindProgram();
    bindBuffers();
    glUniform1f(0, scale);  // scale  in vertex shader
    glUniformMatrix4fv(1, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(2, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(3, 1, GL_FALSE, &model[0][0]);
    glUniform1f(4, playerNormalizedDistance);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size());
}
