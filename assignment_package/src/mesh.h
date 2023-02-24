#pragma once
#include "smartpointerhelp.h"
#include "scene/halfedge.h"
#include <vector>
#include "drawable.h"
#include <random>

struct Randomizer {
    std::random_device rd;
    std::uniform_int_distribution<int> distribution;

    Randomizer() {
        rd();
        distribution = std::uniform_int_distribution<int>(0, 255);
    }

    glm::vec3 get_randVec3() {
        return glm::vec3(distribution(rd) / 255.f, distribution(rd) / 255.f, distribution(rd) / 255.f);
    }
};

class Mesh : public Drawable
{
public:
    std::vector<uPtr<Vertex>> m_verts;
    std::vector<uPtr<Face>> m_faces;
    std::vector<uPtr<HalfEdge>> m_hedges;

    Mesh(OpenGLContext* context);
    virtual GLenum drawMode();
    virtual void create();

    void loadObj(QString filename);
    void createSyms();
};

