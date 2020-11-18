#include "camera.h"

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 up) : cameraPosition(cameraPosition), cameraTarget(cameraTarget), up(up), worldUp(up)
{
    view = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, up);
    yaw = 0;
    pitch = 0;
}

void Camera::forwardBackwardMove(float speed)
{
    cameraPosition += glm::normalize(cameraTarget) * speed;
}

void Camera::rotateCamera(float pitch_t, float yaw_t)
{
    this->yaw += yaw_t;
    this->pitch += pitch_t;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 front;
    front.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    front.y = sinf(glm::radians(pitch));
    front.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));

    cameraTarget = glm::normalize(front);

    right = glm::normalize(glm::cross(cameraTarget, worldUp));
    up = glm::normalize(glm::cross(right, cameraTarget));
}

void Camera::updateView()
{
    view = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, up);
}

void Camera::changeProjection(float yOffset)
{
    fov -= yOffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}



glm::mat4 Camera::getView()
{
    return view;
}
