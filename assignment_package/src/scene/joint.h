#pragma once

#include <QTreeWidget>
#include "drawable.h"
#include "shaderprogram.h"
#include "smartpointerhelp.h"
#include "scene/circle3d.h"

class Joint : public QTreeWidgetItem, public Drawable
{
private:
    glm::vec3 translation;
    glm::quat rotation;

    Circle3D c1;
    Circle3D c2;
    Circle3D c3;
public:
    QString name;
    Joint* parent;
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

    void draw(ShaderProgram &prog_flat);
};
