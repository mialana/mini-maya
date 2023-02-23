#include "halfedge.h"
#include "face.h"
#include "vertex.h"

HalfEdge::HalfEdge()
{
}

bool HalfEdge::operator==(const HalfEdge &he2) const {
    return (id == he2.id);
}

bool HalfEdge::operator!=(const HalfEdge &he2) const {
    return (id != he2.id);
}
