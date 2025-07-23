#pragma once
#include <QListWidgetItem>
#include <glm/glm.hpp>

class HalfEdge;

class Face : public QListWidgetItem
{
public:
    static int population;
    int id;

    glm::vec3 m_color;
    HalfEdge* m_hedge;

    Face(glm::vec3 c);
};
