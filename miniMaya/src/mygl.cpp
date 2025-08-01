#include "mygl.h"

#include "mesh.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QKeyEvent>
#include <iostream>

#include <glm/gtx/string_cast.hpp>
#include <pxr/base/gf/vec3f.h>
#include <pxr/base/vt/array.h>
#include <pxr/usd/usd/stage.h>

#define VALUE(string) #string
#define TO_LITERAL(string) VALUE(string)

MyGL::MyGL(QWidget* parent)
    : OpenGLContext(parent)
    , m_geomSquare(this)
    , m_progLambert(this)
    , m_progFlat(this)
    , m_progSkeleton(this)
    , m_glCamera()
    , m_meshCurrent(this)
    , mp_selectedVert(nullptr)
    , mp_selectedFace(nullptr)
    , mp_selectedHedge(nullptr)
    , m_vertDisplay(this)
    , m_faceDisplay(this)
    , m_hedgeDisplay(this)
    , m_skeletonCurrent(this)
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
    // If you were programming in a non-Qt context you might use GLEW (GL
    // Extension Wrangler)instead
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
    // Set the color with which the screen is filled at the start of each render
    // call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    // Create the instances of Cylinder and Sphere.
    m_geomSquare.create();

    QFile relativeFile(QDir().cleanPath(QString(PROJECT_SOURCE_DIR) + "/../obj_files/cube.obj"));

    if (relativeFile.open(QIODevice::ReadOnly)) {
        m_meshCurrent.loadObj(relativeFile);
    }

    m_meshCurrent.create();

    m_skeletonCurrent.create();

    m_vertDisplay.create();

    m_faceDisplay.create();

    m_hedgeDisplay.create();

    // Create and set up the diffuse shader
    m_progLambert.create((std::string(PROJECT_SOURCE_DIR) + "/glsl/lambert.vert.glsl").c_str(),
                         (std::string(PROJECT_SOURCE_DIR) + "/glsl/lambert.frag.glsl").c_str());
    // Create and set up the flat lighting shader
    m_progFlat.create((std::string(PROJECT_SOURCE_DIR) + "/glsl/flat.vert.glsl").c_str(),
                      (std::string(PROJECT_SOURCE_DIR) + "/glsl/flat.frag.glsl").c_str());

    m_progSkeleton.create((std::string(PROJECT_SOURCE_DIR) + "/glsl/skeleton.vert.glsl").c_str(),
                          (std::string(PROJECT_SOURCE_DIR) + "/glsl/skeleton.frag.glsl").c_str());

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    // This code sets the concatenated view and perspective projection matrices
    // used for our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics
    // card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    m_progSkeleton.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

// This function is called by Qt any time your GL window is supposed to update
// For example, when the function update() is called, paintGL is called
// implicitly.
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

    m_progSkeleton.setViewProjMatrix(m_glCamera.getViewProj());
    m_progSkeleton.setCamPos(m_glCamera.eye);
    m_progSkeleton.setModelMatrix(glm::mat4(1.f));

    for (auto& v : m_meshCurrent.m_verts) {
        Q_EMIT this->sig_sendListItem(v.get());
    }

    for (auto& f : m_meshCurrent.m_faces) {
        Q_EMIT this->sig_sendListItem(f.get());
    }

    for (auto& he : m_meshCurrent.m_hedges) {
        Q_EMIT this->sig_sendListItem(he.get());
    }

    m_progSkeleton.draw(m_meshCurrent);

    glDisable(GL_DEPTH_TEST);

    m_progFlat.draw(m_vertDisplay);
    m_progFlat.draw(m_faceDisplay);
    m_progFlat.draw(m_hedgeDisplay);

    m_skeletonCurrent.drawJoints(m_progFlat, m_skeletonCurrent.root.get());
    m_progFlat.setModelMatrix(glm::mat4(1.f));
    m_progFlat.draw(m_skeletonCurrent);

    glEnable(GL_DEPTH_TEST);
}

void MyGL::keyPressEvent(QKeyEvent* e)
{
    float amount = 2.0f;
    if (e->modifiers() & Qt::ShiftModifier) {
        amount = 10.0f;
    }

    switch (e->key()) {
        case Qt::Key_Escape: QApplication::quit(); break;
        case Qt::Key_Right: m_glCamera.RotateAboutUp(amount); break;
        case Qt::Key_Left: m_glCamera.RotateAboutUp(-amount); break;
        case Qt::Key_Up: m_glCamera.RotateAboutRight(-amount); break;
        case Qt::Key_Down: m_glCamera.RotateAboutRight(amount); break;
        case Qt::Key_1: m_glCamera.fovy += amount; break;
        case Qt::Key_2: m_glCamera.fovy -= amount; break;
        case Qt::Key_W: m_glCamera.TranslateAlongLook(-amount); break;
        case Qt::Key_S: m_glCamera.TranslateAlongLook(amount); break;
        case Qt::Key_D: m_glCamera.TranslateAlongRight(amount); break;
        case Qt::Key_A: m_glCamera.TranslateAlongRight(-amount); break;
        case Qt::Key_Q: m_glCamera.TranslateAlongUp(-amount); break;
        case Qt::Key_E: m_glCamera.TranslateAlongUp(amount); break;
        case Qt::Key_R: m_glCamera = Camera(this->width(), this->height()); break;
    }

    m_glCamera.RecomputeAttributes();

    if (mp_selectedHedge != nullptr) {
        switch (e->key()) {
            case Qt::Key_N: Q_EMIT sig_setSelectedHedge(mp_selectedHedge->next); return;
            case Qt::Key_M: Q_EMIT sig_setSelectedHedge(mp_selectedHedge->sym); return;
            case Qt::Key_F: Q_EMIT sig_setSelectedFace(mp_selectedHedge->m_face); return;
            case Qt::Key_V: Q_EMIT sig_setSelectedVert(mp_selectedHedge->m_vert); return;
        }
    }

    if (e->key() == Qt::Key_H) {
        if (amount == 10.0f) {
            if (mp_selectedFace != nullptr) {
                Q_EMIT sig_setSelectedHedge(mp_selectedFace->m_hedge);
            }
        } else {
            if (mp_selectedVert != nullptr) {
                Q_EMIT sig_setSelectedHedge(mp_selectedVert->m_hedge);
            }
        }
        return;
    }

    update();
}

void MyGL::bindSkeletonHelper()
{
    m_skeletonCurrent.bindMats = std::vector<glm::mat4>();
    m_skeletonCurrent.transformMats = std::vector<glm::mat4>();
    m_skeletonCurrent.computeBindAndTransformMatrices(m_skeletonCurrent.root.get());

    m_progSkeleton.setBindMats(m_skeletonCurrent.bindMats);
    m_progSkeleton.setOverallTransforms(m_skeletonCurrent.transformMats);
    m_progSkeleton.setBinded(true);
}

void MyGL::updateAll()
{
    if (m_meshCurrent.binded && m_skeletonCurrent.root != nullptr && m_meshCurrent.initiated) {
        bindSkeletonHelper();
    }

    m_meshCurrent.destroy();
    m_meshCurrent.create();

    m_skeletonCurrent.destroy();
    m_skeletonCurrent.create();

    Q_EMIT sig_setSelectedVert(mp_selectedVert);
    Q_EMIT sig_setSelectedFace(mp_selectedFace);
    Q_EMIT sig_setSelectedHedge(mp_selectedHedge);

    update();
}

void MyGL::slot_splitHedge()
{
    if (mp_selectedHedge != nullptr) {
        HalfEdge* he2 = mp_selectedHedge->sym;
        Vertex* v1 = mp_selectedHedge->m_vert;
        Vertex* v2 = he2->m_vert;
        glm::vec3 midPt = (v1->m_pos + v2->m_pos) / 2.f;

        m_meshCurrent.splitHedge(mp_selectedHedge, he2, v1, v2, midPt);

        updateAll();
    }
}

void MyGL::slot_triangulateFace()
{
    if (mp_selectedFace != nullptr) {
        m_meshCurrent.triangulateFace(mp_selectedFace);

        updateAll();
    }
}

void MyGL::slot_subdivideMesh()
{
    m_meshCurrent.subdivideMesh();

    updateAll();
}

void MyGL::slot_bindSkeleton()
{
    if (m_skeletonCurrent.root != nullptr && m_meshCurrent.initiated) {
        bindSkeletonHelper();

        m_meshCurrent.bindSkeleton(m_skeletonCurrent);

        updateAll();
    }
}

void MyGL::slot_translateX(double newX)
{
    if (mp_selectedVert != nullptr) {
        mp_selectedVert->m_pos.x = newX;

        updateAll();
    }
}

void MyGL::slot_translateY(double newY)
{
    if (mp_selectedVert != nullptr) {
        mp_selectedVert->m_pos.y = newY;

        updateAll();
    }
}

void MyGL::slot_translateZ(double newZ)
{
    if (mp_selectedVert != nullptr) {
        mp_selectedVert->m_pos.z = newZ;

        updateAll();
    }
}

void MyGL::slot_changeRed(double newRed)
{
    if (mp_selectedFace != nullptr) {
        mp_selectedFace->m_color.r = newRed;

        updateAll();
    }
}

void MyGL::slot_changeGreen(double newGreen)
{
    if (mp_selectedFace != nullptr) {
        mp_selectedFace->m_color.g = newGreen;

        updateAll();
    }
}

void MyGL::slot_changeBlue(double newBlue)
{
    if (mp_selectedFace != nullptr) {
        mp_selectedFace->m_color.b = newBlue;

        updateAll();
    }
}

void MyGL::slot_jointTranslateX(double newX)
{
    if (m_skeletonCurrent.mp_selectedJoint != nullptr) {
        m_skeletonCurrent.mp_selectedJoint->translation.x = newX;

        updateAll();
    }
}

void MyGL::slot_jointTranslateY(double newY)
{
    if (m_skeletonCurrent.mp_selectedJoint != nullptr) {
        m_skeletonCurrent.mp_selectedJoint->translation.y = newY;

        updateAll();
    }
}

void MyGL::slot_jointTranslateZ(double newZ)
{
    if (m_skeletonCurrent.mp_selectedJoint != nullptr) {
        m_skeletonCurrent.mp_selectedJoint->translation.z = newZ;

        updateAll();
    }
}

void MyGL::slot_jointRotatePlusX()
{
    if (m_skeletonCurrent.mp_selectedJoint != nullptr) {
        m_skeletonCurrent.mp_selectedJoint->rotation = glm::quat(
            glm::angleAxis(glm::radians(5.0f), glm::vec3(1.f, 0.f, 0.f))
            * m_skeletonCurrent.mp_selectedJoint->rotation);

        updateAll();
    }
}

void MyGL::slot_jointRotatePlusY()
{
    if (m_skeletonCurrent.mp_selectedJoint != nullptr) {
        m_skeletonCurrent.mp_selectedJoint->rotation = glm::quat(
            glm::angleAxis(glm::radians(5.0f), glm::vec3(0.f, 1.f, 0.f))
            * m_skeletonCurrent.mp_selectedJoint->rotation);

        updateAll();
    }
}

void MyGL::slot_jointRotatePlusZ()
{
    if (m_skeletonCurrent.mp_selectedJoint != nullptr) {
        m_skeletonCurrent.mp_selectedJoint->rotation = glm::quat(
            glm::angleAxis(glm::radians(5.0f), glm::vec3(0.f, 0.f, 1.f))
            * m_skeletonCurrent.mp_selectedJoint->rotation);

        updateAll();
    }
}

void MyGL::slot_jointRotateMinusX()
{
    if (m_skeletonCurrent.mp_selectedJoint != nullptr) {
        m_skeletonCurrent.mp_selectedJoint->rotation = glm::quat(
            glm::angleAxis(glm::radians(-5.0f), glm::vec3(1.f, 0.f, 0.f))
            * m_skeletonCurrent.mp_selectedJoint->rotation);

        updateAll();
    }
}

void MyGL::slot_jointRotateMinusY()
{
    if (m_skeletonCurrent.mp_selectedJoint != nullptr) {
        m_skeletonCurrent.mp_selectedJoint->rotation = glm::quat(
            glm::angleAxis(glm::radians(-5.0f), glm::vec3(0.f, 1.f, 0.f))
            * m_skeletonCurrent.mp_selectedJoint->rotation);

        updateAll();
    }
}

void MyGL::slot_jointRotateMinusZ()
{
    if (m_skeletonCurrent.mp_selectedJoint != nullptr) {
        m_skeletonCurrent.mp_selectedJoint->rotation = glm::quat(
            glm::angleAxis(glm::radians(-5.0f), glm::vec3(0.f, 0.f, 1.f))
            * m_skeletonCurrent.mp_selectedJoint->rotation);

        updateAll();
    }
}

void MyGL::slot_exportToUSD()
{
    // const pxr::VtIntArray &faceVertexCounts;
    //     const pxr::VtIntArray &faceVertexIndices;
    //     const pxr::VtVec3fArray &points;
    QString usda_file = QFileDialog::getSaveFileName(this,
                                                     QString("Select Destination for USD Export"),
                                                     QDir().cleanPath(
                                                         QString(PROJECT_SOURCE_DIR)
                                                         + "/../usd_outputs/output.usda"),
                                                     QString("*.usda"));

    if (usda_file == "") {
        qDebug() << "No destination selected.";
        return;
    }

    pxr::UsdStageRefPtr stage = pxr::UsdStage::CreateNew(usda_file.toStdString());
    m_usdMesh = pxr::UsdGeomMesh::Define(stage, pxr::SdfPath("/MyRoot/MyMesh"));

    pxr::UsdAttribute face_vertex_counts = m_usdMesh.CreateFaceVertexCountsAttr();
    pxr::UsdAttribute face_vertex_indices = m_usdMesh.CreateFaceVertexIndicesAttr();
    pxr::UsdAttribute points = m_usdMesh.CreatePointsAttr();

    pxr::VtArray<int> face_vertex_counts_data;
    pxr::VtArray<int> face_vertex_indices_data;
    pxr::VtArray<pxr::GfVec3f> points_data;

    for (auto& face : m_meshCurrent.m_faces) {
        HalfEdge* firstHedge = face->m_hedge;
        HalfEdge* currHedge = firstHedge;
        int count = 0;

        do {  // discover how many vertices are in this face
            count += 1;
            face_vertex_indices_data.push_back(currHedge->m_vert->id);

            currHedge = currHedge->next;
        } while (currHedge != firstHedge);
        face_vertex_counts_data.push_back(count);
    }

    // iterate through mesh verts to read position data
    for (auto& vert : m_meshCurrent.m_verts) {
        glm::vec3 pos = vert->m_pos;
        points_data.push_back(pxr::GfVec3f(pos.x, pos.y, pos.z));
    }

    face_vertex_counts.Set(face_vertex_counts_data);
    face_vertex_indices.Set(face_vertex_indices_data);
    points.Set(points_data);

    stage->SetDefaultPrim(m_usdMesh.GetPrim());

    stage->Save();
}
