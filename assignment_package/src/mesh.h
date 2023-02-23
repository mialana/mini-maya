#pragma once
#include "smartpointerhelp.h"
#include "scene/vertex.h"
#include "scene/face.h"
#include <vector>
#include "drawable.h"

class Mesh : public Drawable
{
public:
    std::vector<uPtr<Vertex>> m_verts;
    std::vector<uPtr<Face>> m_faces;
    std::vector<uPtr<HalfEdge>> m_halfes;

    Mesh(OpenGLContext* mp_context);
    virtual GLenum drawMode();
    virtual void create();
};

