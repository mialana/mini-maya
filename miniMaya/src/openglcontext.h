#pragma once

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLWidget>
#include <QWidget>

class OpenGLContext : public QOpenGLWidget, public QOpenGLFunctions_3_2_Core
{
    Q_OBJECT

protected:
    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /*** If true, save a test image and exit */
    /***/ bool autotesting;

public:
    OpenGLContext(QWidget* parent);
    ~OpenGLContext();

    void debugContextVersion();
    void printGLErrorLog();
    void printLinkInfoLog(int prog);
    void printShaderInfoLog(int shader);

private Q_SLOTS:

    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /***/ void saveImageAndQuit();

    virtual void timerUpdate() {}
};
