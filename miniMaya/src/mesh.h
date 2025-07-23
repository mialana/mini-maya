#pragma once
#include "smartpointerhelp.h"
#include "scene/halfedge.h"
#include <vector>
#include "drawable.h"
#include <random>
#include <QFileDialog>
#include <unordered_map>
#include <unordered_set>
#include "scene/skeleton.h"

struct Randomizer {
    std::random_device rd;
    std::uniform_int_distribution<int> distribution;

    Randomizer() {
        rd();
        distribution = std::uniform_int_distribution<int>(0, 255);
    }

    glm::vec3 get_randVec3() {
        return glm::vec3(distribution(rd) / 255.f, distribution(rd) / 255.f, distribution(rd) / 255.f);
    }
};

class Mesh : public Drawable
{
private:
    void findCentroids(std::unordered_map<Face*, Vertex*>& centroids);
    void findSmoothedMidpoints(std::unordered_map<Face*, Vertex*>& centroids,
                                  std::unordered_map<HalfEdge*, glm::vec3>& midpointMap);
    void splitNewHedges(std::unordered_map<HalfEdge*, glm::vec3>& midpointMap);
    void smoothOrigVerts(std::unordered_map<Face*, Vertex*>& centroids, int n);
    void quadrangulate(std::unordered_map<Face*, Vertex*>& centroids, int n);

    static HalfEdge* findHedgeBefore(HalfEdge* he);
public:   
    bool initiated;
    bool binded;
    std::vector<uPtr<Vertex>> m_verts;
    std::vector<uPtr<Face>> m_faces;
    std::vector<uPtr<HalfEdge>> m_hedges;

    Mesh(OpenGLContext* context);
    virtual GLenum drawMode();
    virtual void create();

    void loadObj(QFile& file);
    void createSyms();

    void extracted(std::vector<glm::vec2> &weights);
    void bindSkeleton(Skeleton &skeleton);
    void splitHedge(HalfEdge*, HalfEdge* , Vertex*, Vertex*, glm::vec3);
    void triangulateFace(Face*);

    void subdivideMesh();
};

