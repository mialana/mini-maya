#pragma once
#include "halfedge.h"

class Vertex
{
public:
    static int population;
    int id;

    glm::vec3 m_pos;
    HalfEdge* m_halfe;

    Vertex(glm::vec3 p, HalfEdge* he);
};
