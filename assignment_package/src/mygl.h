#ifndef MYGL_H
#define MYGL_H

#include "camera.h"
#include "display.h"
#include "mesh.h"
#include "openglcontext.h"
#include "scene/skeleton.h"
#include "scene/squareplane.h"
#include "shaderprogram.h"
#include "utils.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include <pxr/usd/usdGeom/mesh.h>

class MyGL : public OpenGLContext {
  Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_progSkeleton;

  GLuint vao; // A handle for our vertex array object. This will store the VBOs
              // created in our geometry classes. Don't worry too much about
              // this. Just know it is necessary in order to render geometry.

  Camera m_glCamera;

    void updateAll();
    void bindSkeletonHelper();

public:
  Mesh m_meshCurrent;
  Vertex *mp_selectedVert;
  Face *mp_selectedFace;
  HalfEdge *mp_selectedHedge;

  VertexDisplay m_vertDisplay;
  FaceDisplay m_faceDisplay;
  HedgeDisplay m_hedgeDisplay;

  Skeleton m_skeletonCurrent;

  pxr::UsdGeomMesh m_usdMesh;

  explicit MyGL(QWidget *parent = nullptr);
  ~MyGL();

  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

protected:
  void keyPressEvent(QKeyEvent *e);

public Q_SLOTS:
    void slot_splitHedge();
    void slot_triangulateFace();
    void slot_subdivideMesh();
    void slot_bindSkeleton();

  void slot_translateX(double);
  void slot_translateY(double);
  void slot_translateZ(double);

  void slot_changeRed(double);
  void slot_changeGreen(double);
  void slot_changeBlue(double);

  void slot_exportToUSD();
    void slot_jointTranslateX(double);
    void slot_jointTranslateY(double);
    void slot_jointTranslateZ(double);

    void slot_jointRotatePlusX();
    void slot_jointRotatePlusY();
    void slot_jointRotatePlusZ();

    void slot_jointRotateMinusX();
    void slot_jointRotateMinusY();
    void slot_jointRotateMinusZ();

Q_SIGNALS:
    void sig_sendRootNode(QTreeWidgetItem*);
    void sig_sendListItem(QListWidgetItem*);
    void sig_setSelectedVert(QListWidgetItem*);
    void sig_setSelectedFace(QListWidgetItem*);
    void sig_setSelectedHedge(QListWidgetItem*);

};

#endif // MYGL_H
