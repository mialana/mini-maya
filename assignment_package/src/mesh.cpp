#include "mesh.h"
#include <QFileDialog>
#include <iostream>

Mesh::Mesh(OpenGLContext* context) : Drawable(context)
{}

GLenum Mesh::drawMode() {
    return GL_TRIANGLES;
}

void Mesh::create() {

    std::vector<GLuint> indices;
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec4> colors;

    int currTotal = 0;
    for (const uPtr<Face> &f : m_faces) {
        int fVertNum = 0;
        HalfEdge* curr = f->m_hedge;

        do {
            positions.push_back(glm::vec4(curr->m_vert->m_pos, 1));

            const glm::vec3 v1 = curr->next->m_vert->m_pos - curr->m_vert->m_pos;
            const glm::vec3 v2 = curr->next->next->m_vert->m_pos - curr->next->m_vert->m_pos;
            glm::vec3 n = glm::normalize(glm::cross(v1, v2));
            normals.push_back(glm::vec4(n, 0));

            colors.push_back(glm::vec4(curr->m_face->m_color, 1));
            fVertNum++;
            curr = curr->next;
        } while (curr != f->m_hedge);

        for (int i = 0; i < fVertNum - 2; i++) {
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
    mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec4), normals.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
}

void Mesh::createSyms() {
    std::map<std::pair<Vertex*, Vertex*>, HalfEdge*> hedgesMap;
    for (const auto& f : this->m_faces) {
        HalfEdge* map_val = f->m_hedge;
        do {
            HalfEdge* curr = map_val;
            do {
                curr = curr->next;
            } while (curr->next != map_val);

            Vertex* a = (map_val->m_vert->id < curr->m_vert->id) ? map_val->m_vert : curr->m_vert;
            Vertex* b = (map_val->m_vert->id < curr->m_vert->id) ? curr->m_vert : map_val->m_vert;

            std::pair<Vertex*, Vertex*> p = std::make_pair(a, b);

            if (hedgesMap[p] != nullptr) {
                map_val->symWith(hedgesMap[p]);
            } else {
                hedgesMap[p] = map_val;
            }
            map_val = map_val->next;
        } while (map_val != f->m_hedge);
    }
}

void Mesh::loadObj(QString filename) {
    this->m_verts.clear();
    this->m_faces.clear();
    this->m_hedges.clear();


    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly)){
        qWarning("Could not open the file.");
        return;
    }

    QTextStream fileText(&file);
    while (!fileText.atEnd()) {
        QString fileLine = fileText.readLine();

        if (fileLine.startsWith("v ")) {
            QStringList lineList = fileLine.split(" ");
            glm::vec3 pos = glm::vec3(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());
            uPtr<Vertex> v = mkU<Vertex>(pos);
            this->m_verts.push_back(std::move(v));
        }
    }

    fileText.seek(0);

    while (!fileText.atEnd()) {
        QString fileLine = fileText.readLine();

        Randomizer r = Randomizer();

        if (fileLine.startsWith("f ")) {
            QStringList lineList = fileLine.split(" ");

            glm::vec3 randCol = r.get_randVec3();
            uPtr<Face> f = mkU<Face>(randCol);

            HalfEdge* firstHedge;
            for (int i = 1; i < lineList.length(); i++) {
                int vertIdx = lineList[i].split("/")[0].toInt();
                Vertex* v = this->m_verts[vertIdx - 1].get();

                uPtr<HalfEdge> newHedge = mkU<HalfEdge>();
                newHedge->setVert(v);
                newHedge->setFace(f.get());

                if (i == 1) {
                    firstHedge = newHedge.get();
                } else {
                    HalfEdge* prevHedge = this->m_hedges.back().get();
                    prevHedge->next = newHedge.get();
                }
                if (i == lineList.length() - 1) {
                    newHedge->next = firstHedge;
                }
                this->m_hedges.push_back(std::move(newHedge));
            }

            this->m_faces.push_back(std::move(f));
        }
    }

    this->createSyms();
}
