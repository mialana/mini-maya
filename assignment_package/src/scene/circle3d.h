#pragma once

#include <drawable.h>

class Circle3D : public Drawable
{
public:
    glm::vec3 axis;
    glm::vec4 center;
    glm::vec4 circleVert;

    Circle3D(OpenGLContext*, glm::vec3, glm::vec4, glm::vec4);

    GLenum drawMode() override {
        return GL_LINES;
    }

    void create() override;
};
