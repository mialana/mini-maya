#include "display.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

VertexDisplay::VertexDisplay(OpenGLContext* context)
    : Drawable(context), displayedVert(nullptr)
{}

void VertexDisplay::updateVert(Vertex* v) {
    displayedVert = v;
}

void VertexDisplay::create() {
    if (displayedVert == nullptr) {
        this->count = 0;
        return;
    }
    glm::vec4 pos = glm::vec4(displayedVert->m_pos, 1);
    glm::vec4 col = glm::vec4(1, 1, 1, 1);

    this->count = 1;

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint), 0, GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), glm::value_ptr(pos), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), glm::value_ptr(col), GL_STATIC_DRAW);

}



FaceDisplay::FaceDisplay(OpenGLContext* context)
    : Drawable(context), displayedFace(nullptr)
{}

void FaceDisplay::updateFace(Face* f) {
    displayedFace = f;
}

void FaceDisplay::create() {
    if (displayedFace == nullptr) {
        this->count = 0;
        return;
    }

    std::vector<GLuint> indices;
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> colors;

    int fVertNum = 0;
    HalfEdge* he = displayedFace->m_hedge;
    do {
        positions.push_back(glm::vec4(he->m_vert->m_pos, 1));

        colors.push_back(glm::vec4((1.f - displayedFace->m_color.x),
                                   (1.f - displayedFace->m_color.y),
                                   (1.f - displayedFace->m_color.z),
                                   1));

        fVertNum++;
        he = he->next;
    } while (he != displayedFace->m_hedge);

    for (int i = 0; i < fVertNum; i++) {
        indices.push_back(i);
        indices.push_back((i+1) % fVertNum);
    }

    this->count = indices.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);

}



HedgeDisplay::HedgeDisplay(OpenGLContext* context)
    : Drawable(context), displayedHedge(nullptr)
{}

void HedgeDisplay::updateHedge(HalfEdge* he) {
    displayedHedge = he;
}

void HedgeDisplay::create() {
    if (displayedHedge == nullptr || dynamic_cast<HalfEdge*>(displayedHedge->sym) == nullptr) {
        this->count = 0;
        return;
    }

    std::vector<GLuint> indices;
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> colors;

    positions.push_back(glm::vec4(displayedHedge->m_vert->m_pos, 1));

    positions.push_back(glm::vec4(displayedHedge->sym->m_vert->m_pos, 1));

    colors.push_back(glm::vec4(1, 1, 0, 1));
    colors.push_back(glm::vec4(1, 0, 0, 1));

    indices.push_back(0);
    indices.push_back(1);

    this->count = indices.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);


}
