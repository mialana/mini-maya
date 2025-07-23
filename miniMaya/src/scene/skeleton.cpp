#include "skeleton.h"
#include <iostream>

Skeleton::Skeleton(OpenGLContext* context)
    : Drawable(context), mp_selectedJoint(nullptr)
{
    root = nullptr;

    indices = std::vector<GLuint>();
    positions = std::vector<glm::vec4>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec4>();
    bindMats = std::vector<glm::mat4>();
    transformMats = std::vector<glm::mat4>();
}

Skeleton::Skeleton(OpenGLContext* context, uPtr<Joint> r)
    : Drawable(context), mp_selectedJoint(nullptr)
{
    root = std::move(r);

    indices = std::vector<GLuint>();
    positions = std::vector<glm::vec4>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec4>();
    bindMats = std::vector<glm::mat4>();
    transformMats = std::vector<glm::mat4>();
}

Skeleton::~Skeleton() {
    Drawable::destroy();

    indices = std::vector<GLuint>();
    positions = std::vector<glm::vec4>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec4>();
    bindMats = std::vector<glm::mat4>();
    transformMats = std::vector<glm::mat4>();
}

void Skeleton::computeBindMatrices(Joint* curr) {
    curr->bindMatrix = glm::inverse(curr->getOverallTransformation());

    for (const auto& c : curr->children) {
        computeBindMatrices(c.get());
    }
}

void Skeleton::drawJoints(ShaderProgram &shader, Joint* curr) {
    if (root == nullptr) {
        return;
    }
    curr->draw(shader);

    for (const auto& c : curr->children) {
        drawJoints(shader, c.get());
    }
}

void Skeleton::createHelper(Joint* curr) {
    if (curr == mp_selectedJoint) {
        curr->createJoint(true);
    } else {
        curr->createJoint(false);
    }

    glm::vec4 worldPosition = curr->getOverallTransformation() * glm::vec4(0.f, 0.f, 0.f, 1.f);

    positions.push_back(worldPosition);

    if (curr->parent != nullptr) {
        glm::vec4 parentWorldPosition = curr->parent->getOverallTransformation() * glm::vec4(0.f, 0.f, 0.f, 1.f);
        positions.push_back(parentWorldPosition);
    } else {
        positions.push_back(worldPosition);
    }

    for (const auto& c : curr->children) {
        this->createHelper(c.get());
    }
}

void Skeleton::create() {
    if (root == nullptr) {
        this->count = 0;
        return;
    }

    indices = std::vector<GLuint>();
    positions = std::vector<glm::vec4>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec4>();

    this->createHelper(root.get());

    for (int i = 0; i < int(positions.size()); i = i + 2) {
        indices.push_back(i);
        indices.push_back(i + 1);

        colors.push_back(glm::vec4(1, 1, 0, 1));
        colors.push_back(glm::vec4(1, 0, 1, 1));
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

uPtr<Joint> Skeleton::loadJsonHelper(QJsonObject jointJsonObj, Joint* parent) {
    QString foundName = jointJsonObj["name"].toString();
    QJsonArray foundTrans = jointJsonObj["pos"].toArray();
    QJsonArray foundRot = jointJsonObj["rot"].toArray();
    QJsonArray foundChildren = jointJsonObj["children"].toArray();

    glm::vec3 finalTrans = glm::vec3(foundTrans[0].toDouble(), foundTrans[1].toDouble(), foundTrans[2].toDouble());
    double angle = foundRot[0].toDouble();
    glm::vec3 axis = glm::vec3(foundRot[1].toDouble(), foundRot[2].toDouble(), foundRot[3].toDouble());
    glm::quat finalRot = glm::quat(glm::angleAxis(float(angle),axis));

    uPtr<Joint> newJtUPtr = mkU<Joint>(mp_context, foundName, parent, finalTrans, finalRot);

    Joint* newJt = newJtUPtr.get();

    for (auto c : foundChildren) {
        uPtr<Joint> newChild = loadJsonHelper(c.toObject(), newJt);
        newJt->addChild(std::move(newChild));
    }

    return newJtUPtr;
}

void Skeleton::loadJson(QJsonObject rootJsonObj) {
    root = loadJsonHelper(rootJsonObj, nullptr);

    indices = std::vector<GLuint>();
    positions = std::vector<glm::vec4>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec4>();

    this->destroy();
    this->create();

    this->computeBindMatrices(root.get());
}

void Skeleton::computeBindAndTransformMatrices(Joint* j) {
    bindMats.push_back(j->bindMatrix);
    transformMats.push_back(j->getOverallTransformation());

    for (const auto& c : j->children) {
        computeBindAndTransformMatrices(c.get());
    }
}
