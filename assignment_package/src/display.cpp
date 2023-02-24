#include "display.h"
#include <iostream>

VertexDisplay::VertexDisplay(OpenGLContext* context)
    : Drawable(context), displayedVertex(nullptr)
{}

void VertexDisplay::updateVertex(Vertex* v) {
    displayedVertex = mkU<Vertex>(*v);
}

void VertexDisplay::create() {
    std::vector<GLuint> indices;
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> colors;

    if (displayedVertex != nullptr) {
        positions.push_back(glm::vec4(displayedVertex->m_pos.x, displayedVertex->m_pos.y, displayedVertex->m_pos.z, 1));
        colors.push_back(glm::vec4(1, 1, 1, 1));
        indices.push_back(0);
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



FaceDisplay::FaceDisplay(OpenGLContext* context)
    : Drawable(context), displayedFace(nullptr)
{}

void FaceDisplay::updateFace(Face* f) {
    displayedFace = f;
}

void FaceDisplay::create() {



}



HedgeDisplay::HedgeDisplay(OpenGLContext* context)
    : Drawable(context), displayedHedge(nullptr)
{}

void HedgeDisplay::updateHedge(HalfEdge* he) {
    displayedHedge = he;
}

void HedgeDisplay::create() {



}
