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
}

void Skeleton::computeBindMatrices(Joint* curr) {
    curr->bindMatrix = glm::inverse(curr->getOverallTransformation());

    for (const auto& c : curr->children) {
        computeBindMatrices(c.get());
    }
}

//void Skeleton::draw(ShaderProgram &prog_flat) {
//    prog_flat.draw(c1);
//    prog_flat.draw(c2);
//    prog_flat.draw(c3);
//}

void Skeleton::create() {

}
