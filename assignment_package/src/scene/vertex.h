#pragma once
#include <QListWidgetItem>
#include <glm/glm.hpp>
#include "joint.h"

class HalfEdge;

class Vertex : public QListWidgetItem
{
public:
    static int population;
    int id;

    glm::vec3 m_pos;
    HalfEdge* m_hedge;

    Joint *influencer1, *influencer2;
    float weight1, weight2;

    Vertex(glm::vec3 p);

    Vertex(const Vertex& v2);
};
