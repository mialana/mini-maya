#pragma once

#include <QTreeWidget>
#include "joint.h"
#include "smartpointerhelp.h"
#include "la.h"

class Skeleton : public QTreeWidget, public Drawable
{
public:
    uPtr<Joint> root;

    Skeleton(OpenGLContext*);
    Skeleton(OpenGLContext*, Joint*);
    ~Skeleton();

    void computeBindMatrices(Joint*);

    void create();
};
