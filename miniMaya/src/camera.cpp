#include "camera.h"

#include "la.h"
#include <iostream>

Camera::Camera()
    : Camera(400, 400)
{
    look = glm::vec3(0, 0, -1);
    up = glm::vec3(0, 1, 0);
    right = glm::vec3(1, 0, 0);
}

Camera::Camera(unsigned int w, unsigned int h)
    : Camera(w, h, glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))
{}

Camera::Camera(unsigned int w,
               unsigned int h,
               const glm::vec3& e,
               const glm::vec3& r,
               const glm::vec3& worldUp)
    : fovy(45)
    , width(w)
    , height(h)
    , near_clip(0.1f)
    , far_clip(1000)
    , eye(e)
    , ref(r)
    , world_up(worldUp)
{
    radius = glm::length(ref - eye);
    theta = 0;
    phi = 0;
    RecomputeAttributes();
}

Camera::Camera(const Camera& c)
    : fovy(c.fovy)
    , width(c.width)
    , height(c.height)
    , near_clip(c.near_clip)
    , far_clip(c.far_clip)
    , aspect(c.aspect)
    , eye(c.eye)
    , ref(c.ref)
    , look(c.look)
    , up(c.up)
    , right(c.right)
    , world_up(c.world_up)
    , V(c.V)
    , H(c.H)
    , theta(c.theta)
    , phi(c.phi)
    , radius(c.radius)
{}

void Camera::operator=(const Camera& c)
{
    fovy = c.fovy;
    width = c.width;
    height = c.height;
    near_clip = c.near_clip;
    far_clip = c.far_clip;
    aspect = c.aspect;
    eye = c.eye;
    ref = c.ref;
    look = c.look;
    up = c.up;
    right = c.right;
    world_up = c.world_up;
    V = c.V;
    H = c.H;
    theta = c.theta;
    phi = c.phi;
    radius = c.radius;
}

void Camera::RecomputeAttributes()
{
    look = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(look, world_up));
    up = glm::cross(right, look);

    float tan_fovy = tan(glm::radians(fovy / 2));
    float len = glm::length(ref - eye);
    aspect = width / static_cast<float>(height);
    V = up * len * tan_fovy;
    H = right * len * aspect * tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(glm::radians(fovy), width / (float)height, near_clip, far_clip)
           * glm::lookAt(eye, ref, up);
}

void Camera::RotateAboutUp(float deg)
{
    theta += glm::radians(deg);
    updateEye();
    RecomputeAttributes();
}

void Camera::RotateAboutRight(float deg)
{
    phi += glm::radians(deg);
    updateEye();
    RecomputeAttributes();
}

void Camera::TranslateAlongLook(float amt)
{
    radius += amt;
    updateEye();
    RecomputeAttributes();
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;
}

void Camera::TranslateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;
}

void Camera::updateEye()
{
    eye = glm::vec3(glm::rotate(theta, glm::vec3(0.f, 1.f, 0.f))
                    * glm::rotate(phi, glm::vec3(1.f, 0.f, 0.f))
                    * glm::translate(glm::vec3(0.f, 0.f, radius)) * glm::vec4(ref, 1));
}
