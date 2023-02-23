#include "mesh.h"

Mesh::Mesh(OpenGLContext* mp_context) : Drawable(mp_context)
{}

GLenum Mesh::drawMode() {
    return GL_TRIANGLES;
}

void Mesh::create() {

    std::vector<GLuint> indices;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;

    int currTotal = 0;
    for (const uPtr<Face> &f : m_faces) {
        int fVertNum = 0;
        HalfEdge* curr = f->m_halfe;

        // declare vectors that will deterine normal
        const glm::vec3 v1 = curr->m_next->m_vertex->m_pos - curr->m_vertex->m_pos;
        const glm::vec3 v2 = curr->m_next->m_next->m_vertex->m_pos - curr->m_next->m_vertex->m_pos;
        glm::vec3 n = glm::normalize(glm::cross(v1, v2));
        do {
            positions.push_back(curr->m_vertex->m_pos);
            normals.push_back(n);
            colors.push_back(curr->m_face->m_color);
            fVertNum++;
            curr = curr->m_next;
        } while (curr != f->m_halfe);

        for (int i = 0; i < fVertNum; i++) {
            indices.push_back(currTotal);
            indices.push_back(currTotal + i + 1);
            indices.push_back(currTotal + i + 2);
        }
        currTotal += fVertNum;
    }

    this->count = indices.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
}
