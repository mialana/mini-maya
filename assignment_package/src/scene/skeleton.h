#pragma once

#include <QTreeWidget>
#include "joint.h"
#include "mesh.h"
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
    void bindMeshHelper(Vertex*, Joint*);
public:
    uPtr<Joint> root;

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

    void bindMesh(Mesh&);
};
