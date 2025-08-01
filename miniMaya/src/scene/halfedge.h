#pragma once
#include <QListWidgetItem>
#include "vertex.h"
#include "face.h"

class HalfEdge : public QListWidgetItem
{
public:
    static int population;
    int id;

    HalfEdge* next;
    HalfEdge* sym;
    Vertex* m_vert;
    Face* m_face;

    HalfEdge();
    HalfEdge(HalfEdge* n, HalfEdge* s, Vertex* v, Face*);

    bool operator==(const HalfEdge& he2) const;
    bool operator!=(const HalfEdge& he2) const;

    void symWith(HalfEdge* he2);
    void setVert(Vertex* v);
    void setFace(Face* f);
};
