#include "vertex.h"

int Vertex::population = 0;

Vertex::Vertex(glm::vec3 p)
{
    this->id = population;
    population++;

    this->m_pos = p;
    this->m_hedge = nullptr;

    this->influencers = std::make_pair(nullptr, nullptr);
    this->distances = glm::vec2(MAXFLOAT, MAXFLOAT);
    this->weights = glm::vec2(-1.f, -1.f);

    QListWidgetItem::setText(QString::number(id));
}

Vertex::Vertex(const Vertex& v2)
{
    this->id = v2.id;
    this->m_pos = v2.m_pos;
    this->m_hedge = v2.m_hedge;

    QListWidgetItem::setText(QString::number(v2.id));
}

void Vertex::computeInfluentialJoints(Joint* currJoint)
{
    float distance = glm::distance(glm::vec4(m_pos, 1),
                                   currJoint->getOverallTransformation() * glm::vec4(0, 0, 0, 1));

    if (distance < distances[0]) {
        if (distance < distances[1]) {
            distances[0] = distances[1];
            influencers.first = influencers.second;

            distances[1] = distance;
            influencers.second = currJoint;
        } else {
            distances[0] = distance;
            influencers.first = currJoint;
        }
    }

    for (const auto& c : currJoint->children) {
        computeInfluentialJoints(c.get());
    }
}
