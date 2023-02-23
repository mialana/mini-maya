#include "halfedge.h"
#include "face.h"
#include "vertex.h"

int HalfEdge::population = 0;

HalfEdge::HalfEdge() {
    this->id = population;
    population++;

    this->next = nullptr;
    this->sym = nullptr;
    this->m_vert = nullptr;
    this->m_face = nullptr;

    QListWidgetItem::setText(QString::number(id));
}

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
