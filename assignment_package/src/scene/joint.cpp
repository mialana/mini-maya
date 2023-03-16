#include "joint.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/perpendicular.hpp>

Joint::Joint(OpenGLContext* context, QString n)
    : Drawable(context), name(n)
{
    parent = nullptr;
    translation = glm::vec3(0,0,0);
    rotation = glm::quat(1,0,0,0);
    bindMatrix = glm::mat4(0);
}

Joint::Joint(OpenGLContext* context, QString n, Joint* p, glm::vec3 t, glm::quat r)
    : Drawable(context), translation(t), rotation(r), name(n), parent(p), bindMatrix(glm::mat4(0))
{}

Joint::Joint(const Joint& j2)
    : Drawable(j2.mp_context), translation(j2.translation), rotation(j2.rotation),
      name(j2.name), parent(j2.parent), bindMatrix(j2.bindMatrix)
{
    for (const auto& c : j2.children) {
        this->children.push_back(mkU<Joint>(*c));
    }
}

Joint::~Joint() {
    Drawable::destroy();
}

void Joint::addChild(uPtr<Joint> c) {
    QTreeWidgetItem::addChild(c.get());

    children.push_back(std::move(c));
}

Joint& Joint::operator=(const Joint& j2) {
    Drawable::operator=(j2);

    translation = j2.translation;
    rotation = j2.rotation;
    name = j2.name;
    parent = j2.parent;
    bindMatrix = j2.bindMatrix;

    for (const auto& c : j2.children) {
        this->children.push_back(mkU<Joint>(*c));
    }

    return *this;
}

glm::mat4 Joint::getLocalTransformation() {
    return glm::translate(translation) * glm::toMat4(rotation);
}

glm::mat4 Joint::getOverallTransformation() {
    if (parent != nullptr) {
        return parent->getOverallTransformation() * getLocalTransformation();
    } else {
        return getLocalTransformation();
    }
}

void Joint::create() {
    std::vector<GLuint> indices = std::vector<GLuint>();
    std::vector<glm::vec4> positions = std::vector<glm::vec4>();
    std::vector<glm::vec4> normals = std::vector<glm::vec4>();
    std::vector<glm::vec4> colors = std::vector<glm::vec4>();

    glm::vec3 orientationAxis = glm::normalize(this->children[0]->translation);

    glm::vec4 worldPosition = getOverallTransformation() * glm::vec4(0.f, 0.f, 0.f, 1.f);

    glm::vec3 nonparallelAxis = (orientationAxis == glm::vec3(0.f, 0.f, 1.f) || orientationAxis == glm::vec3(0.f, 0.f, -1.f))
                            ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(-orientationAxis.y, orientationAxis.x, orientationAxis.z);

    glm::vec3 orthogonalAxis1 = glm::cross(orientationAxis, nonparallelAxis);
    glm::vec3 orthogonalAxis2 = glm::cross(orientationAxis, orthogonalAxis1);

    glm::vec4 circleVert1 = worldPosition + glm::vec4((glm::vec3(orthogonalAxis1) * 0.5f), 1.f);
    glm::vec4 circleVert2 = worldPosition + glm::vec4((glm::vec3(orthogonalAxis2) * 0.5f), 1.f);
    glm::vec4 circleVert3 = worldPosition + glm::vec4((glm::vec3(orientationAxis) * 0.5f), 1.f);

    for (int i = 0; i < 12; i++) {
        const glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(-worldPosition));
        const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), glm::radians(i * 30.0f), orientationAxis);
        const glm::mat4 reverseTranslationMatrix = glm::translate(glm::mat4(), glm::vec3(worldPosition));

        glm::vec4 v = reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(circleVert1);

        positions.push_back(v);
    }

    for (int i = 0; i < 12; i++) {
        const glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(-worldPosition));
        const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), glm::radians(i * 30.0f), orthogonalAxis1);
        const glm::mat4 reverseTranslationMatrix = glm::translate(glm::mat4(), glm::vec3(worldPosition));

        glm::vec4 v = reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(circleVert2);

        positions.push_back(v);
    }

    for (int i = 0; i < 12; i++) {
        const glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(-worldPosition));
        const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), glm::radians(i * 30.0f), orthogonalAxis2);
        const glm::mat4 reverseTranslationMatrix = glm::translate(glm::mat4(), glm::vec3(worldPosition));

        glm::vec4 v = reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(circleVert3);

        positions.push_back(v);
    }

    int numPositions = positions.size();
    for (int i = 0; i < numPositions; i = i + 12) {
        for (int j = i; j < i + 12; j++) {
            indices.push_back(j);
            indices.push_back(((j+1) % 12) + j);
        }
    }


    for (int i = 0; i < numPositions; i++) {
        normals.push_back(glm::vec4(0, 0, 0, 1));
        colors.push_back(glm::vec4(0, 0, 0, 1));
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
