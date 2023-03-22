#pragma once

#include <QTreeWidget>
#include "drawable.h"
#include "shaderprogram.h"
#include "smartpointerhelp.h"

class Joint : public QTreeWidgetItem, public Drawable
{
public:
    static int population;
    int id;

    QString name;
    Joint* parent;

    glm::vec3 translation;
    glm::quat rotation;

    std::vector<uPtr<Joint>> children;
    glm::mat4 bindMatrix;

    Joint(OpenGLContext*, QString);
    Joint(OpenGLContext*, QString, Joint*, glm::vec3, glm::quat);
    Joint(const Joint&);
    ~Joint();

    void addChild(uPtr<Joint>);

    Joint& operator=(const Joint&);

    glm::mat4 getLocalTransformation();

    glm::mat4 getOverallTransformation();

    GLenum drawMode() override {
        return GL_LINES;
    }

    void create() override;

    void createJoint(bool selected);

    void draw(ShaderProgram &prog_flat);
};
