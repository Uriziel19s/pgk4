#ifndef MYSPHERE_H
#define MYSPHERE_H
#include <vector>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "AGL3Drawable.hpp"
#include <glm/gtx/string_cast.hpp>
class mySphere : public AGLDrawable
{
public:
    struct TriSphere
    {
        TriSphere(glm::vec3 A, glm::vec3 B, glm::vec3 C) : A(A), B(B), C(C)
        {

        }
     glm::vec3 A;
     glm::vec3 B;
     glm::vec3 C;
    };

    mySphere(unsigned int numberOfDivides);
    void updateSpherePosition(const glm::vec3 &spherePosition);
    void draw(const glm::mat4 &view, const glm::mat4 &projection, float scale);
private:
    void generateTriangles(unsigned int numberOfDivides);
    void divideTriangles(const TriSphere &tri, unsigned int deep);
    void normalizeDistanceToTriangles();
    void setShaders();
    std::vector<TriSphere> trianglesPositions;
    void setBuffers();
    glm::mat4 model;
    glm::vec3 centerOfSphere;
};

#endif // MYSPHERE_H
