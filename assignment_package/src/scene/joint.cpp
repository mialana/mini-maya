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
    glm::vec3 orientationAxis = children.empty() ? glm::vec3(1.f, 0.f, 0.f) : glm::normalize(children[0]->translation);
    glm::vec3 worldPosition = glm::vec3(getOverallTransformation() * glm::vec4(0.f, 0.f, 0.f, 1.f));
    glm::vec3 childWorldPosition = children.empty() ? glm::vec3(worldPosition.x + 1, worldPosition.yz()) : glm::vec3(children[0]->getOverallTransformation() * glm::vec4(0.f, 0.f, 0.f, 1.f));

    glm::vec3 nonparallelAxis = (orientationAxis == glm::vec3(0.f, 0.f, 1.f) || orientationAxis == glm::vec3(0.f, 0.f, -1.f))
                                ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(-orientationAxis.y, orientationAxis.x, orientationAxis.z);

    glm::vec3 orthogonalAxis1 = glm::cross(orientationAxis, nonparallelAxis);

    prog_flat.setModelMatrix(getOverallTransformation() * glm::lookAt(worldPosition, childWorldPosition, orthogonalAxis1));
    prog_flat.draw(*this);
}

void Joint::create() {

    glm::vec4 circleVert1 = glm::vec4((glm::vec3(1, 0, 0) * 0.5f), 0.f);
    glm::vec4 circleVert2 = glm::vec4((glm::vec3(0, 1, 0) * 0.5f), 0.f);
    glm::vec4 circleVert3 = glm::vec4((glm::vec3(0, 0, 1) * 0.5f), 0.f);

    c1 = Circle3D(mp_context, glm::vec3(0, 0, 1), glm::vec4(0, 0, 0, 1), circleVert1, glm::vec4(0, 0, 1, 1));
    c2 = Circle3D(mp_context, glm::vec3(1, 0, 0), glm::vec4(0, 0, 0, 1), circleVert2, glm::vec4(0, 1, 0, 1));
    c3 = Circle3D(mp_context, glm::vec3(0, 1, 0), glm::vec4(0, 0, 0, 1), circleVert3, glm::vec4(1, 0, 0, 1));

    c1.create();
    c2.create();
    c3.create();

//    this->count = 0;

    std::vector<GLuint> indices = std::vector<GLuint>();
    std::vector<glm::vec4> positions = std::vector<glm::vec4>();
    std::vector<glm::vec4> normals = std::vector<glm::vec4>();
    std::vector<glm::vec4> colors = std::vector<glm::vec4>();

    for (int i= 0; i<12; i++) {
            glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 0, 1)) * glm::vec4(0,0.5f,0,1);
            positions.push_back(v);
            normals.push_back(glm::vec4(0, 0, 0, 1));
                    colors.push_back(glm::vec4(0, 1, 0, 1));
        }
        for (int i= 12; i<24; i++) {
            glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(1, 0, 0)) * glm::vec4(0,0.5f,0,1);;
            positions.push_back(v);
            normals.push_back(glm::vec4(0, 0, 0, 1));
                    colors.push_back(glm::vec4(0, 1, 0, 1));
        }
        for (int i= 24; i<36; i++) {
            glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0,0,1);;
            positions.push_back(v);
            normals.push_back(glm::vec4(0, 0, 0, 1));
                    colors.push_back(glm::vec4(0, 1, 0, 1));
        }

        int numPositions = positions.size();
            for (int i = 0; i < numPositions; i = i + 12) {
                for (int j = i; j < i + 12; j++) {
                    indices.push_back(j);
                    indices.push_back((j+1) % (i+12) + i * (j / (i + 11)));
                }
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
