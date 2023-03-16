#include "joint.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/perpendicular.hpp>

Joint::Joint(OpenGLContext* context, QString n)
    : Drawable(context), c1(context), c2(context), c3(context), name(n)
{
    parent = nullptr;
    translation = glm::vec3(0,0,0);
    rotation = glm::quat(1,0,0,0);
    bindMatrix = glm::mat4(0);
}

Joint::Joint(OpenGLContext* context, QString n, Joint* p, glm::vec3 t, glm::quat r)
    : Drawable(context), translation(t), rotation(r), c1(context), c2(context), c3(context),
      name(n), parent(p), bindMatrix(glm::mat4(0))
{}

Joint::Joint(const Joint& j2)
    : Drawable(j2.mp_context), translation(j2.translation), rotation(j2.rotation),
      c1(j2.c1), c2(j2.c2), c3(j2.c3),
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
    c1 = j2.c1;
    c2 = j2.c2;
    c3 = j2.c3;
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

void Joint::draw(ShaderProgram &prog_flat) {
    prog_flat.draw(c1);
    prog_flat.draw(c2);
    prog_flat.draw(c3);
}

void Joint::create() {
    glm::vec4 worldPosition = getOverallTransformation() * glm::vec4(0.f, 0.f, 0.f, 1.f);

    glm::vec3 orientationAxis = children.empty() ? glm::vec3(1.f, 0.f, 0.f) : glm::normalize(children[0]->translation);

    glm::vec3 nonparallelAxis = (orientationAxis == glm::vec3(0.f, 0.f, 1.f) || orientationAxis == glm::vec3(0.f, 0.f, -1.f))
                            ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(-orientationAxis.y, orientationAxis.x, orientationAxis.z);

    glm::vec3 orthogonalAxis1 = glm::cross(orientationAxis, nonparallelAxis);
    glm::vec3 orthogonalAxis2 = glm::cross(orientationAxis, orthogonalAxis1);

    glm::vec4 circleVert1 = worldPosition + glm::vec4((glm::vec3(orientationAxis) * 0.5f), 0.f);
    glm::vec4 circleVert2 = worldPosition + glm::vec4((glm::vec3(orthogonalAxis1) * 0.5f), 0.f);
    glm::vec4 circleVert3 = worldPosition + glm::vec4((glm::vec3(orthogonalAxis2) * 0.5f), 0.f);

    c1 = Circle3D(mp_context, orthogonalAxis1, worldPosition, circleVert1, glm::vec4(0, 0, 1, 1));
    c2 = Circle3D(mp_context, orthogonalAxis2, worldPosition, circleVert2, glm::vec4(0, 1, 0, 1));
    c3 = Circle3D(mp_context, orientationAxis, worldPosition, circleVert3, glm::vec4(1, 0, 0, 1));

    c1.create();
    c2.create();
    c3.create();

    this->count = 0;
}
