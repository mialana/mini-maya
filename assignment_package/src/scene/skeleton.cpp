#include "skeleton.h"

Skeleton::Skeleton(OpenGLContext* context)
    : Drawable(context)
{
    root = mkU<Joint>(context, QString("root"));
}

Skeleton::Skeleton(OpenGLContext* context, Joint* r)
    : Drawable(context)
{
    root = mkU<Joint>(*r);
}

Skeleton::~Skeleton() {
    Drawable::destroy();
    QTreeWidget::destroy();
}

void Skeleton::computeBindMatrices(Joint* curr) {
    curr->bindMatrix = glm::inverse(curr->getOverallTransformation());

    for (const auto& c : curr->children) {
        computeBindMatrices(c.get());
    }
}

void Skeleton::create() {

}
