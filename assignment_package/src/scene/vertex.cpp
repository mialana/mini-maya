#include "vertex.h"

int Vertex::population = 0;

Vertex::Vertex(glm::vec3 p)
{
    this->id = population;
    population++;

    this->m_pos = p;
    this->m_hedge = nullptr;
}
