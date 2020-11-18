#include "myshape.h"

myShape::myShape(unsigned int dimensions, int seed) : kDimensions(dimensions), engine(std::random_device()())
{
    if(seed != 0)
    {
        engine.seed(static_cast<unsigned long>(seed));
    }
    setShaders();
    setMidpoints(-1, -1, -1, 2);
    setRotations();
    setBuffers();

}

void myShape::setMidpoints(float xStart, float yStart, float zStart, float dimensionSize)
{
    float x = xStart + 0.1f;
    float y = yStart + 0.1f;
    float z = zStart + 0.1f;
    const float kPositionStep = (dimensionSize - 0.2f) / kDimensions;
    for(unsigned int i = 0; i < kDimensions; ++i) //xy
    {
        for(unsigned int j = 0; j < kDimensions; ++j)//y
        {
            for(unsigned int k = 0; k < kDimensions; ++k)
            {
              //  if(dist(engine))
                //{
                    //squareMidpoints.push_back(glm::vec3{xStart + kPositionStep * i, yStart + kPositionStep *j, zStart + kPositionStep * k});
               // }
                //else
               // {
                    trianglesMidpoints.push_back(glm::vec3{xStart + kPositionStep * i, yStart + kPositionStep *j, zStart + kPositionStep * k});
                //}
                    std::cout << xStart + kPositionStep * i << " " << yStart + kPositionStep *j << " " << zStart + kPositionStep * k << "\n";

            }
        }
    }
}

void myShape::draw(float tx, float ty, float scale, glm::mat4 view)
{
    bindProgram();
    glBindVertexArray(VAO);
    glUniformMatrix4fv(0, 1, GL_FALSE, &view[0][0]);
    glm::mat4 mProjection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)800, 1.0f, 50.0f);
    glUniformMatrix4fv(1, 1, GL_FALSE, &mProjection[0][0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1000);


  //  glBindVertexArray(VAO[1]);
  //  glUniformMatrix4fv(0, 1, GL_FALSE, &view[0][0]);
   // glDrawArraysInstanced(GL_TRIANGLES, 0, 6, squareMidpoints.size());
}



void myShape::setShaders()
{
    compileShadersFromFile("/home/uriziel/pgk04/myShape.vs", "/home/uriziel/pgk04/myShape.fs");
}

void myShape::setBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &midpointsVBO);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);    //set triangle vertices relative position
    glBufferData(GL_ARRAY_BUFFER, triangleVertices.size() * sizeof(float), triangleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, midpointsVBO);//set triangle vertices middle position
    std::vector<glm::mat4> matrixes;
    for(unsigned int i = 0; i < trianglesMidpoints.size(); ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(trianglesMidpoints[i]));
        model = glm::rotate(model, glm::radians(trianglesAngles[i]), glm::vec3(1,1,1));
        matrixes.push_back(model);
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*trianglesMidpoints.size(), matrixes.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)16);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)32);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)48);
    glVertexAttribDivisor(1, 0);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);


}

void myShape::setRotations()
{
    for(int i = 0; i < kDimensions * kDimensions * kDimensions; ++i)
    {
        trianglesAngles.push_back(angleDist(engine));
    }
}


