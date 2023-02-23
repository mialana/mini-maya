#include "face.h"

Face::Face(glm::vec3 c, HalfEdge* he) {
    this->m_color = c;
    this->m_halfe = he;
}
