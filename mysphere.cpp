#include "mysphere.h"


void mySphere::generateTriangles(unsigned int numberOfDivides)
{
    float a = 2.0f / glm::sqrt(3.0f); //
    glm::vec3 A(-a / 2.0f, -1.0f / 3.0f, 0.0f);
    glm::vec3 B(a / 2.0f, - 1.0f / 3.0f, 0.0f);
    glm::vec3 C(0.0f,  2.0f / 3.0f, 0.0f);
    glm::vec3 D(0.0f, 0.0f, glm::sqrt(a * a + (1.0f * 2.0f / 3.0f) * (1.0f * 2.0f / 3.0f)));
    A *=5.0f;
    B *=5.0f;
    C *=5.0f;
    D *=5.0f;
    centerOfSphere = glm::vec3(0.0f, 0.0f, glm::sqrt(glm::pow(2.0f / 3.0f, 2.0f) + glm::pow(a, 2.0f)));
    divideTriangles(TriSphere(A, B, C), numberOfDivides);
    divideTriangles(TriSphere(A, B, D), numberOfDivides);
    divideTriangles(TriSphere(B, C, D), numberOfDivides);
    divideTriangles(TriSphere(C, A, D), numberOfDivides);

}

mySphere::mySphere(unsigned int numberOfDivides)
{
    generateTriangles(numberOfDivides);
    normalizeDistanceToTriangles();
    setShaders();
    setBuffers();

}
void mySphere::setSpherePosition(const glm::vec3 &spherePosition)
{
    centerOfSphere = spherePosition;
    model = glm::mat4(1.0f);
    model = glm::translate(model, spherePosition);
}


void mySphere::updateSpherePosition(const glm::vec3 &sphereMove)
{
    centerOfSphere += sphereMove;
    model = glm::mat4(1.0f);
    model = glm::translate(model, centerOfSphere);
}

void mySphere::divideTriangles(const TriSphere &tri, unsigned int deep)
{
    if(deep == 0)
    {
        return;
    }
    --deep;
    glm::vec3 D = tri.A + (tri.B - tri.A) / 2.0f;
    glm::vec3 E = tri.B + (tri.C - tri.B) / 2.0f;
    glm::vec3 F = tri.C + (tri.A - tri.C) / 2.0f;
    trianglesPositions.push_back(TriSphere(tri.A, D, F));
    divideTriangles(trianglesPositions.back(), deep);

    trianglesPositions.push_back(TriSphere(tri.B, E, D));
    divideTriangles(trianglesPositions.back(), deep);

    trianglesPositions.push_back(TriSphere(tri.C, F, E));
    divideTriangles(trianglesPositions.back(), deep);

    trianglesPositions.push_back(TriSphere(D, E, F));
    divideTriangles(trianglesPositions.back(), deep);

}

void mySphere::normalizeDistanceToTriangles()
{

    for(auto &triangle : trianglesPositions)
    {
        triangle.A = glm::normalize(triangle.A - centerOfSphere)/5.0f;
        triangle.B = glm::normalize(triangle.B - centerOfSphere)/5.0f;
        triangle.C = glm::normalize(triangle.C - centerOfSphere)/5.0f;
    }
}

void mySphere::setShaders()
{
        compileShadersFromFile("/home/uriziel/pgk04/MySphere.vs", "/home/uriziel/pgk04/MySphere.fs");
}

void mySphere::setBuffers()
{

    bindBuffers();

    std::vector<glm::vec3> tempVector;
    for(auto triangle : trianglesPositions)
    {
        tempVector.push_back(triangle.A);
        tempVector.push_back(triangle.B);
        tempVector.push_back(triangle.C);
    }
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, tempVector.size() * 3 * sizeof(float), tempVector.data(), GL_STATIC_DRAW );
    glVertexAttribPointer(
                0,                 // attribute 0, must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

}

void mySphere::draw(const glm::mat4 &view, const glm::mat4 &projection, float scale)
{

    bindProgram();
    bindBuffers();
    glUniformMatrix4fv(0, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(1, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(2, 1, GL_FALSE, &model[0][0]);
    glUniform1f(3, scale);
    glDrawArrays(GL_TRIANGLES, 0, 3*trianglesPositions.size()+100);
}
