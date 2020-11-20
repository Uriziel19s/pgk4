#include "myshape.h"

myShape::myShape(unsigned int dimensions, int seed) : kDimensions(dimensions), engine(std::random_device()())
{
    if(seed != 0)
    {
        engine.seed(static_cast<unsigned long>(seed));
    }
    setShaders();
    setMidpoints(-10, -10, -10, 10);
    setRotations();
    setScales();
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
                    trianglesMidpoints.push_back(glm::vec3{xStart + kPositionStep * i, yStart + kPositionStep *j, zStart + kPositionStep * k});
            }
        }
    }
}

void myShape::draw(float tx, float ty, float scale, glm::mat4 view, const glm::mat4 &projection)
{
    bindProgram();
    glBindVertexArray(VAO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(0, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(1, 1, GL_FALSE, &projection[0][0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1000);
}

bool myShape::isCollision(glm::vec3 spherePosition, float sphereRay)
{
    sphereRay += 3;
    bool firstCheckXY;
    bool secondCheckXY;
    bool thirdCheckXY;
    bool firstCheckYZ;
    bool secondCheckYZ;
    bool thirdCheckYZ;
    for(int i = 0; i < trianglesMidpoints.size(); ++i)
    {
        std::vector<float> scaledTriangleVertices;
        scaledTriangleVertices.resize(triangleVertices.size());
        for(int j = 0; j < triangleVertices.size(); ++j)
        {
            scaledTriangleVertices[j] = triangleVertices[j] * trianglesScales[i];
            std::cout << triangleVertices[j] << " * " << trianglesScales[i] << " = " <<  scaledTriangleVertices[j] << std::endl;
        }

        for(int k = 0; k < triangleVertices.size(); k = k + 3)
        {
            //glm::vec3 tempVec = glm::rotate(glm::vec3(scaledTriangleVertices[0 + k], scaledTriangleVertices[1 + k], scaledTriangleVertices[2 + k]), glm::radians(trianglesAngles[i]), glm::vec3(1, 1, 1));
            glm::vec3 tempVec = glm::vec3(scaledTriangleVertices[0 + k], scaledTriangleVertices[1 + k], scaledTriangleVertices[2 + k]);
            tempVec *= trianglesMidpoints[i];
            scaledTriangleVertices[0 + k] = tempVec.x;
            scaledTriangleVertices[1 + k] = tempVec.y;
            scaledTriangleVertices[2 + k] = tempVec.z;
            std::cout << triangleVertices[k] << " * " << trianglesAngles[i] << " = " <<  scaledTriangleVertices[k] << " XXXXX " << std::endl;
        }



        glm::vec3 sphereTriangleVector = trianglesMidpoints[i] - spherePosition;
        sphereTriangleVector = glm::normalize(sphereTriangleVector) * sphereRay;
        sphereTriangleVector = sphereTriangleVector + spherePosition; //calculating endpoint position on the line between triangle and sphere midpoints

        glm::vec2 A = glm::vec2(spherePosition.x, spherePosition.y); //casting to 2d xy space
        glm::vec2 B = glm::vec2(sphereTriangleVector.x, sphereTriangleVector.y);

        glm::vec2 C = glm::vec2(scaledTriangleVertices[0], scaledTriangleVertices[1]);
        glm::vec2 D = glm::vec2(scaledTriangleVertices[3], scaledTriangleVertices[4]);
        firstCheckXY = isIntersection(A, B, C, D); //check sphere-triangle line and 1, 2 triangle vertices
        D = glm::vec2(scaledTriangleVertices[6], scaledTriangleVertices[7]);
        secondCheckXY = isIntersection(A, B, C, D); //check sphere-triangle line and 1, 3 triangle vertices
        C = glm::vec2(scaledTriangleVertices[3], scaledTriangleVertices[4]);
        thirdCheckXY = isIntersection(A, B, C, D); //check sphere-triangle line and 2, 3 triangle vertices

        A = glm::vec2(spherePosition.y, spherePosition.z); //casting to 2d yz space
        B = glm::vec2(sphereTriangleVector.y, sphereTriangleVector.z);

        C = glm::vec2(scaledTriangleVertices[1], scaledTriangleVertices[2]);
        D = glm::vec2(scaledTriangleVertices[4], scaledTriangleVertices[5]);
        firstCheckYZ = isIntersection(A, B, C, D); //check sphere-triangle line and 1, 2 triangle vertices
        D = glm::vec2(scaledTriangleVertices[7], scaledTriangleVertices[8]);
        secondCheckYZ = isIntersection(A, B, C, D); //check sphere-triangle line and 1, 3 triangle vertices
        C = glm::vec2(scaledTriangleVertices[4], scaledTriangleVertices[5]);
        thirdCheckYZ = isIntersection(A, B, C, D); //check sphere-triangle line and 2, 3 triangle vertices

        if((firstCheckXY || secondCheckXY || thirdCheckXY) && (firstCheckYZ || secondCheckYZ || thirdCheckYZ))
            std::cout <<"Dziala jak ta lala kurna" << std::endl;
    }
}



void myShape::setShaders()
{
    compileShadersFromFile("/home/uriziel/pgk04/myShape.vs", "/home/uriziel/pgk04/myShape.fs");
}

void myShape::setBuffers()
{
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &midpointsVBO);
    glGenBuffers(1, &scalesVBO);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);    //set triangle vertices relative position
    glBufferData(GL_ARRAY_BUFFER, triangleVertices.size() * sizeof(float), triangleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);//enabling vertex atrributes for model layout
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, midpointsVBO);//set triangle vertices middle position
    std::vector<glm::mat4> matrixes;

    for(unsigned int i = 0; i < trianglesMidpoints.size(); ++i)//creating model matrix
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(trianglesMidpoints[i]));
       // model = glm::rotate(model, glm::radians(trianglesAngles[i]), glm::vec3(1,1,1));
        matrixes.push_back(model);
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*trianglesMidpoints.size(), matrixes.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)16);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)32);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)48);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, scalesVBO);
    glBufferData(GL_ARRAY_BUFFER, trianglesScales.size() * sizeof(float), trianglesScales.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glVertexAttribDivisor(5, 1);

}

void myShape::setRotations()
{
    for(int i = 0; i < kDimensions * kDimensions * kDimensions; ++i)
    {
        trianglesAngles.push_back(angleDist(engine));
    }
}

void myShape::setScales()
{
    for(int i = 0; i < trianglesMidpoints.size(); ++i)
    {
        trianglesScales.push_back(scaleDist(engine));
    }
}

bool myShape::isIntersection(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D)
{
    GLfloat detABxAC = vectorProduct(A.x, A.y, B.x, B.y, C.x, C.y);
    GLfloat detABxAD = vectorProduct(A.x, A.y, B.x, B.y, D.x, D.y);
    GLfloat detCDxCA = vectorProduct(C.x, C.y, D.x, D.y, A.x, A.y);
    GLfloat detCDxCB = vectorProduct(C.x, C.y, D.x, D.y, B.x, B.y);
    if(detABxAC * detABxAD < 0 && detCDxCA * detCDxCB < 0)
    {
        return true;
    }
    return false;
}

float myShape::vectorProduct(float xA, float yA, float xB, float yB, float xC, float yC)
{
    GLfloat x1 = xB - xA;
    GLfloat y1 = yB - yA;
    GLfloat x2 = xC - xA;
    GLfloat y2 = yC - yA;
    return x1*y2 - x2*y1;
}

