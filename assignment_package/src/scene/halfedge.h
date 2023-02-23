#pragma once
#include <glm/glm.hpp>

class Face;

class Vertex;

class HalfEdge
{
public:
    static int population;
    int id;

    HalfEdge* m_next;
    HalfEdge* m_sym;
    Face* m_face;
    Vertex* m_vertex;

    HalfEdge();

    bool operator==(const HalfEdge &he2) const;
    bool operator!=(const HalfEdge &he2) const;
};
