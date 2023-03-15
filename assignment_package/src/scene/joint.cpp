#include "joint.h"

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
            this->children.push_back(std::move(c));
    }
}

Joint::~Joint() {
    Drawable::destroy();
    QTreeWidget::destroy();
}

Joint& Joint::operator=(const Joint& j2) {
    Drawable::operator=(j2);

    translation = j2.translation;
    rotation = j2.rotation;
    name = j2.name;
    parent = j2.parent;
    bindMatrix = j2.bindMatrix;

    for (const auto& c : j2.children) {
            this->children.push_back(std::move(c));
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

void createWireframeVertexPositions(Joint* parent, Joint* self){

    for (int i= 12; i<24; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(1, 0, 0)) * glm::vec4(0,0.5f,0,1);;
        wf_vert_pos[i] = v;
    }
    for (int i= 24; i<36; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0,0,1);;
        wf_vert_pos[i] = v;
    }

    wf_vert_pos[36] = glm::vec4(0,0,0,1);
    wf_vert_pos[37] = glm::inverse(self->GetLocalTransformation()) * glm::vec4(0,0,0,1);
}

void Joint::create() {
    std::vector<GLuint> indices = std::vector<GLuint>();
    std::vector<glm::vec4> positions = std::vector<glm::vec4>();
//    std::vector<glm::vec4> normals = std::vector<glm::vec4>();
    std::vector<glm::vec4> colors = std::vector<glm::vec4>();

    glm::vec3 orientationAxis = this->children[0]->translation;

    for (int i = 0; i < 12; i++) {
        glm::mat4 m = glm::rotate(glm::mat4(), glm::radians(i * 30.0f), orientationAxis);
        positions.push_back(m * getOverallTransformation() * glm::vec4(1, 1, 1, 1));
    }

    int n = 10;
    for (int i = 0; i < n; i++)
    {
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);
    }

    for (int i = 0; i < 12; i++) {
        colors.push_back(glm::vec4(0, 0, 0, 1));
    }

    this->count = indices.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
}
