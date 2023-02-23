#include "vertex.h"

Vertex::Vertex(glm::vec3 p, HalfEdge* he)
{
    this->m_pos = p;
    this->m_halfe = he;
}
