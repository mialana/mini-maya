#pragma once
#include "drawable.h"
#include "mesh.h"

class VertexDisplay : public Drawable
{
protected:
    Vertex* displayedVert;

public:
    VertexDisplay(OpenGLContext* context);

    void create() override;

    void updateVert(Vertex* v);

    GLenum drawMode() override
    {
        return GL_POINTS;
    }
};

class FaceDisplay : public Drawable
{
protected:
    Face* displayedFace;

public:
    FaceDisplay(OpenGLContext* context);

    void create() override;

    void updateFace(Face* v);

    GLenum drawMode() override
    {
        return GL_LINES;
    }
};

class HedgeDisplay : public Drawable
{
protected:
    HalfEdge* displayedHedge;

public:
    HedgeDisplay(OpenGLContext* context);

    void create() override;

    void updateHedge(HalfEdge* v);

    GLenum drawMode() override
    {
        return GL_LINES;
    }
};
