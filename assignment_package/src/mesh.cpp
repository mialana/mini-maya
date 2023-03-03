#include "mesh.h"
#include "glm/gtx/string_cast.hpp"
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
            const glm::vec3 v2 = curr->sym->m_vert->m_pos - curr->m_vert->m_pos;
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
            HalfEdge* curr = findHedgeBefore(map_val);

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

void Mesh::loadObj(QFile& file) {
    this->m_verts.clear();
    this->m_faces.clear();
    this->m_hedges.clear();

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


HalfEdge* Mesh::findHedgeBefore(HalfEdge* he) {
    HalfEdge* curr = he;
    do {
        curr = curr->next;
    } while (curr->next != he);

    return curr;
}

void Mesh::splitHedge(HalfEdge* he1, HalfEdge* he2, Vertex* v1, Vertex* v2, glm::vec3 pos) {
    uPtr<Vertex> v3Uptr = mkU<Vertex>(pos);

    uPtr<HalfEdge> he1bUptr = mkU<HalfEdge>(he1->next, nullptr, nullptr, nullptr);
    uPtr<HalfEdge> he2bUptr = mkU<HalfEdge>(he2->next, nullptr, nullptr, nullptr);

    Vertex* v3 = v3Uptr.get();
    HalfEdge* he1b = he1bUptr.get();
    HalfEdge* he2b = he2bUptr.get();

    this->m_verts.push_back(std::move(v3Uptr));
    this->m_hedges.push_back(std::move(he1bUptr));
    this->m_hedges.push_back(std::move(he2bUptr));

    he1b->m_vert = v1;
    he1b->m_face = he1->m_face;

    he2b->m_vert = v2;
    he2b->m_face = he2->m_face;

    he1->next = he1b;
    he2->next = he2b;

    he1->m_vert = v3;
    he2->m_vert = v3;

    he1->symWith(he2b);
    he2->symWith(he1b);
}

void Mesh::triangulateFace(Face* f1) {
    HalfEdge* curr = f1->m_hedge;
    HalfEdge* last = Mesh::findHedgeBefore(curr);

    Vertex* anchor = last->m_vert;

    HalfEdge* he1 = curr->next;
    HalfEdge* he2 = he1->next;

    while (he2 != last) {
        uPtr<Face> nfUptr = mkU<Face>(Randomizer().get_randVec3());
        uPtr<HalfEdge> he_iUptr = mkU<HalfEdge>();
        uPtr<HalfEdge> he_oUptr = mkU<HalfEdge>();

        Face* newFace = nfUptr.get();
        HalfEdge* he_inner = he_iUptr.get();
        HalfEdge* he_outer = he_oUptr.get();

        this->m_faces.push_back(std::move(nfUptr));
        this->m_hedges.push_back(std::move(he_iUptr));
        this->m_hedges.push_back(std::move(he_oUptr));

        he_inner->next = curr;
        he_inner->m_vert = anchor;
        he_inner->m_face = newFace;

        he1->next = he_inner;

        he1->m_face = newFace;
        curr->setFace(newFace);

        he_outer->next = he2;
        he_outer->m_vert = he1->m_vert;
        he_outer->m_face = f1;

        he_inner->symWith(he_outer);

        he_outer->setFace(f1);

        last->next = he_outer;

        curr = he_outer;
        he1 = he2;
        he2 = he2->next;
    }

}

void Mesh::subdivideMesh() {
    int origVertsNum = this->m_verts.size();
    int origFacesNum = this->m_faces.size();

    std::unordered_map<Face*, Vertex*> centroids;
    this->findCentroids(centroids);

    this->computeSmoothedMidpoints(centroids);

    smoothOrigVerts(centroids, origVertsNum);

    for (int i = 0; i < origFacesNum; i++) {
        Face* f = this->m_faces[i].get();
        quadrangulate(f, centroids);
    }
}

void Mesh::findCentroids(std::unordered_map<Face*, Vertex*>& centroids) {
    int numFaces = this->m_faces.size();
    for (int i = 0; i < numFaces; i++) {
        Face* f = this->m_faces[i].get();

        uPtr<Vertex> cUptr = mkU<Vertex>(glm::vec3(0.0f));
        Vertex* centroid = cUptr.get();
        this->m_verts.push_back(std::move(cUptr));

        HalfEdge* start = f->m_hedge;
        HalfEdge* curr = f->m_hedge;
        int vertCount = 0;
        glm::vec3 vertSum = glm::vec3(0.f, 0.f, 0.f);
        do {
            vertSum += curr->m_vert->m_pos;
            vertCount++;
            curr = curr->next;
        } while (curr != start);

        vertSum /= vertCount;

        centroid->m_pos = vertSum;

        centroids.insert(std::pair<Face*, Vertex*>(f, centroid));
    }
}

void Mesh::computeSmoothedMidpoints(std::unordered_map<Face*, Vertex*>& centroids) {
    std::unordered_set<HalfEdge*> hedgesSplit;
    int numHedges = this->m_hedges.size();
    for (int i = 0; i < numHedges; i++) {
        HalfEdge* curr = this->m_hedges[i].get();
        if (!hedgesSplit.count(curr) && !hedgesSplit.count(curr->sym)) {
            HalfEdge* he2 = curr->sym;

            Vertex* v1 = curr->m_vert;
            Vertex* v2 = he2->m_vert;

            glm::vec3 f1Pos = centroids.at(curr->m_face)->m_pos;
            glm::vec3 f2Pos = centroids.at(curr->sym->m_face)->m_pos;

            glm::vec3 e = (v1->m_pos + v2->m_pos + f1Pos + f2Pos) / 4.f;

            this->splitHedge(curr, he2, v1, v2, e);

            hedgesSplit.insert(curr);
            hedgesSplit.insert(curr->sym);
        }
    }
}

void Mesh::smoothOrigVerts(std::unordered_map<Face*, Vertex*>& centroids, int origVertsNum) {
    for (int i = 0; i < origVertsNum; i++) {
        Vertex* v = this->m_verts[i].get();
        glm::vec3 sumMid = glm::vec3(0.f);
        glm::vec3 sumCent = glm::vec3(0.f);
        float midCt = 0;

        HalfEdge* start = v->m_hedge;
        HalfEdge* curr = start;
        do {
            if (curr != nullptr) {
                Face* currFace = curr->m_face;
                sumCent += centroids.at(currFace)->m_pos;

                curr = curr->next;
                sumMid += curr->m_vert->m_pos;
                midCt++;
                curr = curr->sym;
            }
        } while (curr != start);

        v->m_pos = ((midCt - 2.f) * v->m_pos / midCt) +
                (sumMid / glm::pow(midCt, 2.f)) +
                (sumCent / glm::pow(midCt, 2.f));
    }
}

void Mesh::quadrangulate(Face *f, std::unordered_map<Face*, Vertex*>& centroids) {
    HalfEdge* origCurr = f->m_hedge;
    HalfEdge* curr = f->m_hedge;

    HalfEdge* prevToCentroid = nullptr;
    HalfEdge* firstFromCentroid = nullptr;

    do {
        uPtr<HalfEdge> tcUptr = mkU<HalfEdge>(nullptr, nullptr, centroids[f], nullptr);
        uPtr<HalfEdge> fcUptr = mkU<HalfEdge>(curr, nullptr, curr->sym->m_vert, nullptr);

        HalfEdge* toCentroid = tcUptr.get();
        HalfEdge* fromCentroid = fcUptr.get();

        this->m_hedges.push_back(std::move(tcUptr));
        this->m_hedges.push_back(std::move(fcUptr));

        toCentroid->next = fromCentroid;

        if (prevToCentroid != nullptr) {
            fromCentroid->symWith(prevToCentroid);
        }

        if (firstFromCentroid == nullptr) {
            firstFromCentroid = fromCentroid;

            toCentroid->setFace(f);
            fromCentroid->setFace(f);
        } else {
            uPtr<Face> newFace = mkU<Face>(Randomizer().get_randVec3());
            toCentroid->m_face = newFace.get();
            fromCentroid->m_face = newFace.get();
            curr->next->m_face = newFace.get();
            curr->setFace(newFace.get());

            this->m_faces.push_back(std::move(newFace));
        }

        HalfEdge* savedNext = curr->next;
        curr = curr->next->next;
        savedNext->next = toCentroid;
        prevToCentroid = toCentroid;
    } while (curr != origCurr);

    prevToCentroid->symWith(firstFromCentroid);
}
