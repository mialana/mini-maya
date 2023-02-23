#pragma once
#include "halfedge.h"

class Face
{
public:
    static int population;
    int id;

    glm::vec3 m_color;
    HalfEdge* m_halfe;

    Face(glm::vec3 c, HalfEdge* he);
};
