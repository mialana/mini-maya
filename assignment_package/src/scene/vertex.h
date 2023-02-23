#pragma once
#include <QListWidgetItem>
#include <glm/glm.hpp>

class HalfEdge;

class Vertex : public QListWidgetItem
{
public:
    static int population;
    int id;

    glm::vec3 m_pos;
    HalfEdge* m_hedge;

    Vertex(glm::vec3 p);
};
