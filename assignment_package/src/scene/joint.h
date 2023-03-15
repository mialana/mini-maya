#pragma once

#include <QTreeWidget>
#include "drawable.h"
#include "smartpointerhelp.h"
#include "la.h"

class Joint : public QTreeWidget, public Drawable
{
private:
    glm::vec3 translation;
    glm::quat rotation;
public:
    QString name;
    Joint* parent;
    std::vector<uPtr<Joint>> children;
    glm::mat4 bindMatrix;

    Joint(OpenGLContext*, QString);
    Joint(OpenGLContext*, QString, Joint*, glm::vec3, glm::quat);
    Joint(const Joint&);
    ~Joint();

    Joint& operator=(const Joint&);

    glm::mat4 getLocalTransformation();

    glm::mat4 getOverallTransformation();

    void create();
};
