#include "drawable.h"
#include "la.h"

Drawable::Drawable(OpenGLContext* context)
    : count(-1)
    , bufIdx()
    , bufPos()
    , bufNor()
    , bufCol()
    , bufWts()
    , bufIds()
    , idxBound(false)
    , posBound(false)
    , norBound(false)
    , colBound(false)
    , wtsBound(false)
    , idsBound(false)
    , mp_context(context)
{}

Drawable::~Drawable()
{
    destroy();
}

void Drawable::destroy()
{
    mp_context->glDeleteBuffers(1, &bufIdx);
    mp_context->glDeleteBuffers(1, &bufPos);
    mp_context->glDeleteBuffers(1, &bufNor);
    mp_context->glDeleteBuffers(1, &bufCol);
    mp_context->glDeleteBuffers(1, &bufWts);
    mp_context->glDeleteBuffers(1, &bufIds);
}

GLenum Drawable::drawMode()
{
    // Since we want every three indices in bufIdx to be
    // read to draw our Drawable, we tell that the draw mode
    // of this Drawable is GL_TRIANGLES

    // If we wanted to draw a wireframe, we would return GL_LINES

    return GL_TRIANGLES;
}

int Drawable::elemCount()
{
    return count;
}

void Drawable::generateIdx()
{
    idxBound = true;
    // Create a VBO on our GPU and store its handle in bufIdx
    mp_context->glGenBuffers(1, &bufIdx);
}

void Drawable::generatePos()
{
    posBound = true;
    // Create a VBO on our GPU and store its handle in bufPos
    mp_context->glGenBuffers(1, &bufPos);
}

void Drawable::generateNor()
{
    norBound = true;
    // Create a VBO on our GPU and store its handle in bufNor
    mp_context->glGenBuffers(1, &bufNor);
}

void Drawable::generateCol()
{
    colBound = true;
    // Create a VBO on our GPU and store its handle in bufCol
    mp_context->glGenBuffers(1, &bufCol);
}

void Drawable::generateWts()
{
    wtsBound = true;
    // Create a VBO on our GPU and store its handle in bufWt
    mp_context->glGenBuffers(1, &bufWts);
}

void Drawable::generateIds()
{
    idsBound = true;
    // Create a VBO on our GPU and store its handle in bufWt
    mp_context->glGenBuffers(1, &bufIds);
}

bool Drawable::bindIdx()
{
    if (idxBound) {
        mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    }
    return idxBound;
}

bool Drawable::bindPos()
{
    if (posBound) {
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    }
    return posBound;
}

bool Drawable::bindNor()
{
    if (norBound) {
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    }
    return norBound;
}

bool Drawable::bindCol()
{
    if (colBound) {
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    }
    return colBound;
}

bool Drawable::bindWts()
{
    if (wtsBound) {
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWts);
    }
    return wtsBound;
}

bool Drawable::bindIds()
{
    if (idsBound) {
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufIds);
    }
    return idsBound;
}
