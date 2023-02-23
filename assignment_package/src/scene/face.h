#pragma once
#include <glm/glm.hpp>

class HalfEdge;

class Face
{
public:
    static int population;
    int id;

    glm::vec3 m_color;
    HalfEdge* m_hedge;

    Face(glm::vec3 c);
};
