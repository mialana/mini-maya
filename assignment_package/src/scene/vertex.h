#pragma once
#include "scene/joint.h"
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

    std::pair<Joint*, Joint*> influencers;
    glm::vec2 distances;
    glm::vec2 weights;

    Vertex(glm::vec3 p);

    Vertex(const Vertex& v2);
};
