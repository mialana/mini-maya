#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <glm/gtx/string_cast.hpp>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
      m_progLambert(this),
      m_progFlat(this),
      m_glCamera(),
      mp_selectedVert(nullptr),
      mp_selectedFace(nullptr),
      mp_selectedHedge(nullptr),
      m_vertDisplay(this),
      m_faceDisplay(this),
      m_hedgeDisplay(this),
      m_meshCurrent(this)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_meshCurrent.destroy();
    m_geomSquare.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
    m_geomSquare.create();

    m_meshCurrent.create();

    m_vertDisplay.create();

    m_faceDisplay.create();

    m_hedgeDisplay.create();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");


    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);


//    glm::mat4 test = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -5.f));
//    std::cout << glm::to_string(test) << std::endl;
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPointSize(5);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progFlat.setCamPos(m_glCamera.eye);
    m_progFlat.setModelMatrix(glm::mat4(1.f));

    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setCamPos(m_glCamera.eye);
    m_progLambert.setModelMatrix(glm::mat4(1.f));

    m_progLambert.draw(m_meshCurrent);

    for (auto& v : m_meshCurrent.m_verts) {
        emit this->sig_sendListItem(v.get());
    }

    for (auto& f : m_meshCurrent.m_faces) {
        emit this->sig_sendListItem(f.get());
    }

    for (auto& he : m_meshCurrent.m_hedges) {
        emit this->sig_sendListItem(he.get());
    }

    glDisable(GL_DEPTH_TEST);

    m_progFlat.draw(m_vertDisplay);
    m_progFlat.draw(m_faceDisplay);
    m_progFlat.draw(m_hedgeDisplay);

    glEnable(GL_DEPTH_TEST);

}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }

    switch(e->key())
    {
    case Qt::Key_Escape : QApplication::quit(); break;
    case Qt::Key_Right : m_glCamera.RotateAboutUp(-amount); break;
    case Qt::Key_Left : m_glCamera.RotateAboutUp(amount); break;
    case Qt::Key_Up : m_glCamera.RotateAboutRight(-amount); break;
    case Qt::Key_Down : m_glCamera.RotateAboutRight(amount); break;
    case Qt::Key_1 : m_glCamera.fovy += amount; break;
    case Qt::Key_2 : m_glCamera.fovy -= amount; break;
    case Qt::Key_W : m_glCamera.TranslateAlongLook(amount); break;
    case Qt::Key_S : m_glCamera.TranslateAlongLook(-amount); break;
    case Qt::Key_D : m_glCamera.TranslateAlongRight(amount); break;
    case Qt::Key_A :m_glCamera.TranslateAlongRight(-amount); break;
    case Qt::Key_Q : m_glCamera.TranslateAlongUp(-amount); break;
    case Qt::Key_E : m_glCamera.TranslateAlongUp(amount); break;
    case Qt::Key_R : m_glCamera = Camera(this->width(), this->height()); break;
    }

    m_glCamera.RecomputeAttributes();

    if (mp_selectedHedge != nullptr) {
        switch(e->key())
        {
        case Qt::Key_N : slot_setSelectedHedge(mp_selectedHedge->next); return;
        case Qt::Key_M : slot_setSelectedHedge(mp_selectedHedge->sym); return;
        case Qt::Key_F : slot_setSelectedFace(mp_selectedHedge->m_face); return;
        case Qt::Key_V : slot_setSelectedVertex(mp_selectedHedge->m_vert); return;
        }
    }

    if (mp_selectedVert != nullptr && e->key() == Qt::Key_H) {
        if (amount == 10.0f) {
            slot_setSelectedHedge(mp_selectedFace->m_hedge);
        } else {
            slot_setSelectedHedge(mp_selectedVert->m_hedge);
        }
        return;
    }

    update();
}

void MyGL::slot_setSelectedVertex(QListWidgetItem *i) {
    mp_selectedVert = static_cast<Vertex*>(i);
    m_vertDisplay.updateVertex(mp_selectedVert);
    m_vertDisplay.destroy();
    m_vertDisplay.create();

    update();
}

void MyGL::slot_setSelectedFace(QListWidgetItem *i) {
    mp_selectedFace = static_cast<Face*>(i);
    m_faceDisplay.updateFace(mp_selectedFace);
    m_faceDisplay.destroy();
    m_faceDisplay.create();

    update();
}

void MyGL::slot_setSelectedHedge(QListWidgetItem *i) {
    mp_selectedHedge = static_cast<HalfEdge*>(i);
    m_hedgeDisplay.updateHedge(mp_selectedHedge);
    m_hedgeDisplay.destroy();
    m_hedgeDisplay.create();

    update();
}
