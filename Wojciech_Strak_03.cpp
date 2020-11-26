// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage
//===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <sstream>
#include <memory>
#include <array>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "AGL3Window.hpp"
#include "myboard.h"
#include "myline.h"
#include "backgroundrectangle.h"
#include "camera.h"
#include "myshape.h"
#include "mysphere.h"


// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment program
// ==========================================================================




// ==========================================================================
// Window Main Loop Inits ...................................................aaa
// ==========================================================================


class MyWin : public AGLWindow {
public:
    MyWin() {}
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, vers, fullscr), lastMouseXPosition(_wd / 2.0f), lastMouseYPosition(_ht / 2.0f) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void MainLoop(const int seed, const unsigned int dimensions);
    void ScrollCB(double xp, double yp);
    void MousePosCB(double xp, double yp);
    void display();
private:
    Camera camera{glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(-3.0f, -3.0f, -3.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f)};
    Camera camera2{glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(7.5f, 25.0f, 7.5f),
    glm::vec3(0.0f, 1.0f, 0.0f)};
    float lastMouseXPosition;
    float lastMouseYPosition;
    float mouseSensivity = 0.1f;
};

void MyWin::display()
{

}


// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
        ; // do something
    }
    if (key == GLFW_KEY_HOME  && (action == GLFW_PRESS)) {
        ; // do something
    }
}



void MyWin::ScrollCB(double xp, double yp)
{
    camera.changeProjection(yp);
}

void MyWin::MousePosCB(double xp, double yp)
{
    camera.rotateCamera(-(lastMouseYPosition - yp) * mouseSensivity, -(lastMouseXPosition - xp) * mouseSensivity);
    lastMouseXPosition = xp;
    lastMouseYPosition = yp;
    camera.updateView();
}

// ==========================================================================
void MyWin::MainLoop(int seed, unsigned int dimensions) {
    glfwSetInputMode(win(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ViewportOne(0, 0, wd, ht);
    myShape shapes(dimensions, seed);
    mySphere sphere(4);
    BackgroundRectangle background(glm::vec3(7.5f, 7.5f, 7.5f));
    camera.rotateCamera(-70, 180);
    camera.updateView();
    camera2.rotateCamera(89.0f, 0.0f);
    camera2.updateView();
    float sphereRay = 0.2f;
    float speed = 0.2f;
    int  minimapWd = 200;
    int  minimapHt = 200;
    bool showMap = false;
    unsigned int indexOfFinalTriangle = shapes.getLastTriangleIndex();
    const float startToEndDistance = glm::length(shapes.getLasTrianglePosition() - camera.getCameraTarget());
    sphere.setSpherePosition(camera.getCameraTarget());
    do {
    //Viewport(0, 0, wd, ht);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        camera.updateView();
        shapes.draw(0, 0, 1, camera.getView(), camera.getProjection(wd, ht));
        sphere.draw(camera.getView(), camera.getProjection(wd, ht), 1);
        background.draw(80, camera.getView(), camera.getProjection(minimapWd, minimapHt), glm::length(shapes.getLasTrianglePosition() - camera.getCameraTarget()) / startToEndDistance);
        AGLErrors("main-afterdraw");
        WaitForFixedFPS(1.0/60);
      //  glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();
        //glfwWaitEvents();
        if(showMap)
        {
        glScissor(0, 0, minimapWd, minimapHt);
        glEnable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        Viewport(0, 0, minimapWd, minimapHt);
        camera2.updateView();
        shapes.draw(0, 0, 1, camera2.getView(), camera2.getProjection(minimapWd, minimapHt));
        sphere.draw(camera2.getView(), camera2.getProjection(minimapWd, minimapHt), 1);
        background.draw(80, camera2.getView(), camera2.getProjection(minimapWd, minimapHt), glm::length(shapes.getLasTrianglePosition() - camera.getCameraTarget()) / startToEndDistance );
         }
        glfwSwapBuffers(win()); // =============================   Swap buffers


        //std::cout << glm::to_string(camera.getCameraTarget()) << std::endl;



        if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS)
        {
            camera.rotateCamera(-speed,0);
        }
        if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS)
        {
            camera.rotateCamera(speed,0);
        }
        if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS)
        {
            camera.rotateCamera(0,speed);
        }
        if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS)
        {
            camera.rotateCamera(0,-speed);
        }
        if (glfwGetKey(win(), GLFW_KEY_W ) == GLFW_PRESS)
        {
            camera.forwardBackwardMove(-speed);
            if(shapes.isFinalCollision(camera.getCameraTarget(), sphereRay, indexOfFinalTriangle))
            {
                return;
            }
            if(shapes.isCollision(camera.getCameraTarget(), sphereRay))
            {
            camera.forwardBackwardMove(speed);
            }
            sphere.setSpherePosition(camera.getCameraTarget());


        }
        if (glfwGetKey(win(), GLFW_KEY_S ) == GLFW_PRESS)
        {
            camera.forwardBackwardMove(speed);
            if(shapes.isFinalCollision(camera.getCameraTarget(), sphereRay, indexOfFinalTriangle))
            {
                return;
            }
            if(shapes.isCollision(camera.getCameraTarget(), sphereRay))
            {
                camera.forwardBackwardMove(-speed);
            }
            sphere.setSpherePosition(camera.getCameraTarget());
        }
        if (glfwGetKey(win(), GLFW_KEY_A ) == GLFW_PRESS)
        {
            speed=0.8;
        } if (glfwGetKey(win(), GLFW_KEY_D ) == GLFW_PRESS)
        {
            speed = 0.4;
        }if (glfwGetKey(win(), GLFW_KEY_M ) == GLFW_PRESS)
        {
            if(!showMap)
            {
                showMap = true;
            }
            else
            {
                showMap = false;
            }
        }


    } while( glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
             glfwWindowShouldClose(win()) == 0 );
}




int main(int argc, char *argv[]) {
    unsigned int dimensions = 10;
    int seed = 0;
    if(argc > 1)
    {
        std::istringstream ss(argv[1]);
        if(!(ss >> seed))
        {
            std::cout << "Invalid command-line parameter\n";
        }
    }
    if(argc > 2)
    {
        std::istringstream ss(argv[2]);
        if(!(ss >> dimensions))
        {
            std::cout << "Invalid command-line parameter\n";
        }
    }
    MyWin win;
    win.Init(800, 800, "AGL3 example", 0, 33);
    win.MainLoop(seed, dimensions);
    return 0;
}
