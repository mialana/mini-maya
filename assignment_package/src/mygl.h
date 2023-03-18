#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"
#include "mesh.h"
#include "display.h"
#include "scene/skeleton.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

    void updateAll();

public:
    Mesh m_meshCurrent;
    Vertex* mp_selectedVert;
    Face* mp_selectedFace;
    HalfEdge* mp_selectedHedge;

    VertexDisplay m_vertDisplay;
    FaceDisplay m_faceDisplay;
    HedgeDisplay m_hedgeDisplay;

    uPtr<Skeleton> testSkeleton;

    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:
    void slot_splitHedge();
    void slot_triangulateFace();
    void slot_subdivideMesh();

    void slot_translateX(double);
    void slot_translateY(double);
    void slot_translateZ(double);

    void slot_changeRed(double);
    void slot_changeGreen(double);
    void slot_changeBlue(double);

signals:
    void sig_sendListItem(QListWidgetItem*);
    void sig_setSelectedVert(QListWidgetItem*);
    void sig_setSelectedFace(QListWidgetItem*);
    void sig_setSelectedHedge(QListWidgetItem*);

};


#endif // MYGL_H
