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
#include <AGL3Window.hpp>
#include <myboard.h>
#include <myline.h>
#include <backgroundrectangle.h>
#include <camera.h>
#include <myshape.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment program
// ==========================================================================




// ==========================================================================
// Window Main Loop Inits ...................................................aaa
// ==========================================================================


class MyWin : public AGLWindow {
public:
    MyWin() {};
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void MainLoop(const int seed, const unsigned int dimensions);
    Camera camera{glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f)};
};



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

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{

}

// ==========================================================================
void MyWin::MainLoop(const int seed, const unsigned int dimensions) {
    ViewportOne(0,0,wd,ht);
    myShape shapes(10, 0);
    //BackgroundRectangle background(-1, 1, 1, -1);
  //  glfwSetScrollCallback(win, scrollCallback);
    float speed = 0.4;
    do {
        glClear( GL_COLOR_BUFFER_BIT );

        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        camera.updateView();
       // background.draw(0,0,2, camera.getView());
        shapes.draw(0, 0, 0, camera.getView());
        AGLErrors("main-afterdraw");
        WaitForFixedFPS(1.0/60);
        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();
        //glfwWaitEvents();


        if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
            camera.rotateCamera(-speed,0);
        }
        if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
            camera.rotateCamera(speed,0);
        }  if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
            camera.rotateCamera(0,speed);
        } if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
            camera.rotateCamera(0,-speed);
        } if (glfwGetKey(win(), GLFW_KEY_W ) == GLFW_PRESS) {
                camera.forwardBackwardMove(-speed*0.1);
        } if (glfwGetKey(win(), GLFW_KEY_S ) == GLFW_PRESS) {
            camera.forwardBackwardMove(speed*0.1);
        } if (glfwGetKey(win(), GLFW_KEY_A ) == GLFW_PRESS) {

        } if (glfwGetKey(win(), GLFW_KEY_D ) == GLFW_PRESS) {
        std::cout << glm::to_string(camera.getView()) << std::endl;

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
