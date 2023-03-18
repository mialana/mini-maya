#pragma once

#include <QTreeWidget>
#include "joint.h"
#include "smartpointerhelp.h"

class Skeleton : public QTreeWidgetItem, public Drawable
{
private:
    std::vector<GLuint> indices;
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec4> colors;

    void createHelper(Joint*);
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
};
