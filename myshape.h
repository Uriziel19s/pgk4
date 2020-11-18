#ifndef MYTRIANGLE_H
#define MYTRIANGLE_H
#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <AGL3Drawable.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
class myShape : public AGLDrawable
{
public:
    myShape(unsigned int dimensions, int seed);
    void setMidpoints(float xStart, float yStart, float zStart, float dimensionSize);
    void draw(float tx, float ty, float scale, glm::mat4 view);
private:
    void setShaders();
    void setBuffers();
    void setRotations();
    const unsigned int kDimensions;
    std::mt19937 engine;
    std::uniform_int_distribution<unsigned int> dist{0, 1};
    std::uniform_real_distribution<float> angleDist{0, 180.0f};
    unsigned int VAO;
    unsigned int VBO;
    unsigned int midpointsVBO;
    std::array<float, 9> triangleVertices = {-0.5773f, 0.0f, 0.0f,
                                              0.5773f, 0.0f, 0.0f,
                                              0.0f   , 1.0f, 0.0f};

    std::array<float, 18> squareVertices= {-0.5f, -0.5f, 0.0f,
                                            0.5f, -0.5f, 0.0f,
                                            0.5f,  0.5f, 0.0f,
                                            0.5f,  0.5f, 0.0f,
                                           -0.5f,  0.5f, 0.0f,
                                           -0.5f, -0.5f, 0.0f};

    std::vector<glm::vec3> trianglesMidpoints;
    std::vector<float> trianglesAngles;


};

#endif // MYTRIANGLE_H
