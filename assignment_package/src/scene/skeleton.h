#pragma once

#include <QTreeWidget>
#include "joint.h"
#include "smartpointerhelp.h"
#include <QJsonObject>
#include <QJsonArray>

class Skeleton : public QTreeWidgetItem, public Drawable
{
private:
    std::vector<GLuint> indices;
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec4> colors;

    void createHelper(Joint*);
    uPtr<Joint> loadJsonHelper(QJsonObject, Joint*);
public:
    uPtr<Joint> root;

    Joint* mp_selectedJoint;

    std::vector<glm::mat4> bindMats;
    std::vector<glm::mat4> transformMats;

    Skeleton(OpenGLContext*);
    Skeleton(OpenGLContext*, uPtr<Joint>);
    ~Skeleton();

    void computeBindMatrices(Joint*);

    GLenum drawMode() override {
        return GL_LINES;
    }

    void create() override;

    void drawJoints(ShaderProgram &prog_flat, Joint* curr);

    void loadJson(QJsonObject);

    void computeBindAndTransformMatrices(Joint*);
};
