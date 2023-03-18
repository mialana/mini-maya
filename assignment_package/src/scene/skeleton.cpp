#include "skeleton.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

Skeleton::Skeleton(OpenGLContext* context)
    : Drawable(context)
{
    root = mkU<Joint>(context, QString("root"));

    indices = std::vector<GLuint>();
    positions = std::vector<glm::vec4>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec4>();

    this->computeBindMatrices(root.get());
}

Skeleton::Skeleton(OpenGLContext* context, uPtr<Joint> r)
    : Drawable(context)
{
    root = std::move(r);

    indices = std::vector<GLuint>();
    positions = std::vector<glm::vec4>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec4>();

    this->computeBindMatrices(root.get());
}

Skeleton::~Skeleton() {
    Drawable::destroy();

    indices = std::vector<GLuint>();
    positions = std::vector<glm::vec4>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec4>();
}

void Skeleton::computeBindMatrices(Joint* curr) {
    curr->bindMatrix = glm::inverse(curr->getOverallTransformation());

    for (const auto& c : curr->children) {
        computeBindMatrices(c.get());
    }
}

void Skeleton::drawJoints(ShaderProgram &prog_flat, Joint* curr) {
    curr->draw(prog_flat);

    for (const auto& c : curr->children) {
        drawJoints(prog_flat, c.get());
    }
}

void Skeleton::createHelper(Joint* curr) {
    curr->create();

    glm::vec4 worldPosition = curr->getOverallTransformation() * glm::vec4(0.f, 0.f, 0.f, 1.f);
    positions.push_back(worldPosition);
    indices.push_back(positions.size() - 1);
    normals.push_back(glm::vec4(0, 0, 0, 1));
    colors.push_back(glm::vec4(1, 0, 1, 1));

    if (curr->children.empty()) {
        indices.push_back(positions.size() - 1);
        return;
    }
    for (const auto& c : curr->children) {
        glm::vec4 childWorldPosition = c->getOverallTransformation() * glm::vec4(0.f, 0.f, 0.f, 1.f);
        std::cout << glm::to_string(childWorldPosition) << std::endl;
        positions.push_back(childWorldPosition);
        indices.push_back(positions.size() - 1);
        normals.push_back(glm::vec4(0, 0, 0, 1));
        colors.push_back(glm::vec4(1, 1, 0, 1));

        this->createHelper(c.get());
    }
}

void Skeleton::create() { 
    this->createHelper(root.get());

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

void Skeleton::loadJson(QJsonObject rootJsonObj) {
    root = loadJsonHelper(rootJsonObj, nullptr);

    indices = std::vector<GLuint>();
    positions = std::vector<glm::vec4>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec4>();

    this->destroy();
    this->create();
}

uPtr<Joint> Skeleton::loadJsonHelper(QJsonObject jointJsonObj, Joint* parent) {
    QString foundName = jointJsonObj["name"].toString();
    QJsonArray foundTrans = jointJsonObj["pos"].toArray();
    QJsonArray foundRot = jointJsonObj["rot"].toArray();
    QJsonArray foundChildren = jointJsonObj["children"].toArray();

    glm::vec3 finalTrans = glm::vec3(foundTrans[1].toDouble(), foundTrans[2].toDouble(), foundTrans[3].toDouble());
    double angle = foundRot[0].toDouble();
    glm::vec3 axis = glm::vec3(foundRot[1].toDouble(), foundRot[2].toDouble(), foundRot[3].toDouble());
    glm::quat finalRot = glm::quat(glm::angleAxis(float(angle),axis));

    uPtr<Joint> newJtUPtr = mkU<Joint>(mp_context, foundName, parent, finalTrans, finalRot);

    Joint* newJt = newJtUPtr.get();

    for (int i = 0; i < foundChildren.size(); i++) {
        uPtr<Joint> newChild = loadJsonHelper(foundChildren[i].toObject(), newJt);
        newJt->addChild(std::move(newChild));
    }

    return newJtUPtr;
}
