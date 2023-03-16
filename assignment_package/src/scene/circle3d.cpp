#include "circle3d.h"
#include <iostream>

Circle3D::Circle3D(OpenGLContext* context, glm::vec3 a, glm::vec4 c, glm::vec4 cv)
    : Drawable(context), axis(a), center(c), circleVert(cv)
{}

void Circle3D::create() {
    std::vector<GLuint> indices = std::vector<GLuint>();
    std::vector<glm::vec4> positions = std::vector<glm::vec4>();
    std::vector<glm::vec4> normals = std::vector<glm::vec4>();
    std::vector<glm::vec4> colors = std::vector<glm::vec4>();

    for (int i = 0; i < 12; i++) {
        const glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(-center));
        const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), glm::radians(i * 30.0f), axis);
        const glm::mat4 reverseTranslationMatrix = glm::translate(glm::mat4(), glm::vec3(center));

        glm::vec4 v = reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(circleVert);

        positions.push_back(v);
        normals.push_back(glm::vec4(0, 0, 0, 1));
        colors.push_back(glm::vec4(0, 0, 0, 1));
    }

    int numPositions = positions.size();
    for (int i = 0; i < numPositions; i++) {
        indices.push_back(i);
        indices.push_back((i+1) % numPositions);
    }

    this->count = indices.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec4), normals.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
}

