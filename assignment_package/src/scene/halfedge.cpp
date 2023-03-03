#include "halfedge.h"
#include "face.h"
#include "vertex.h"

int HalfEdge::population = 0;

HalfEdge::HalfEdge(HalfEdge* n, HalfEdge* s, Vertex* v, Face* f)
    : next(n), sym(s), m_vert(v), m_face(f) {
    this->id = population;
    population++;

    QListWidgetItem::setText(QString::number(id));
}

HalfEdge::HalfEdge()
    : HalfEdge(nullptr, nullptr, nullptr, nullptr)
{}

bool HalfEdge::operator==(const HalfEdge &he2) const {
    return (id == he2.id);
}

bool HalfEdge::operator!=(const HalfEdge &he2) const {
    return (id != he2.id);
}

void HalfEdge::symWith(HalfEdge* he2) {
    this->sym = he2;
    he2->sym = this;
}

void HalfEdge::setVert(Vertex* v) {
    this->m_vert = v;
    v->m_hedge = this;
}

void HalfEdge::setFace(Face* f) {
    this->m_face = f;
    f->m_hedge = this;
}
