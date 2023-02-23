#include "face.h"

int Face::population = 0;

Face::Face(glm::vec3 c) {
    this->id = population;
    population++;

    this->m_color = c;
    this->m_hedge = nullptr;

    QListWidgetItem::setText(QString::number(id));
}
