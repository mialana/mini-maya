#include "vertex.h"

int Vertex::population = 0;

Vertex::Vertex(glm::vec3 p)
{
    this->id = population;
    population++;

    this->m_pos = p;
    this->m_hedge = nullptr;

    QListWidgetItem::setText(QString::number(id));
}

Vertex::Vertex(const Vertex& v2) {
    this->id = v2.id;
    this->m_pos = v2.m_pos;
    this->m_hedge = v2.m_hedge;

    QListWidgetItem::setText(QString::number(v2.id));
}
