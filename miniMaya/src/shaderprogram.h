#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "openglcontext.h"
#include "la.h"
#include "drawable.h"

#include <glm/glm.hpp>

class ShaderProgram
{
public:
    GLuint vertShader;  // A handle for the vertex shader stored in this shader program
    GLuint fragShader;  // A handle for the fragment shader stored in this shader program
    GLuint prog;        // A handle for the linked shader program stored in this class

    int attrPos;  // A handle for the "in" vec4 representing vertex position in the vertex shader
    int attrNor;  // A handle for the "in" vec4 representing vertex normal in the vertex shader
    int attrCol;  // A handle for the "in" vec4 representing vertex color in the vertex shader
    int attrWts;
    int attrIds;

    int unifModel;  // A handle for the "uniform" mat4 representing model matrix in the vertex shader
    int unifModelInvTr;  // A handle for the "uniform" mat4 representing inverse transpose of the model matrix in the vertex shader
    int unifViewProj;  // A handle for the "uniform" mat4 representing combined projection and view matrices in the vertex shader
    int unifCamPos;  // A handle for the "uniform" vec4 representing color of geometry in the vertex shader
    int unifOverallTransforms;
    int unifBindMats;

    int unifBinded;

public:
    ShaderProgram(OpenGLContext* context);
    // Sets up the requisite GL data and shaders from the given .glsl files
    void create(const char* vertfile, const char* fragfile);
    // Tells our OpenGL context to use this shader to draw things
    void useMe();
    // Pass the given model matrix to this shader on the GPU
    void setModelMatrix(const glm::mat4& model);
    // Pass the given Projection * View matrix to this shader on the GPU
    void setViewProjMatrix(const glm::mat4& vp);
    void setBindMats(const std::vector<glm::mat4>& mvp);
    void setOverallTransforms(const std::vector<glm::mat4>& mvp);
    // Pass the given color to this shader on the GPU
    void setCamPos(glm::vec3 pos);
    void setBinded(bool b);
    // Draw the given object to our screen using this ShaderProgram's shaders
    void draw(Drawable& d);
    // Utility function used in create()
    char* textFileRead(const char*);
    // Utility function that prints any shader compilation errors to the console
    void printShaderInfoLog(int shader);
    // Utility function that prints any shader linking errors to the console
    void printLinkInfoLog(int prog);

    QString qTextFileRead(const char*);

private:
    OpenGLContext*
        context;  // Since Qt's OpenGL support is done through classes like QOpenGLFunctions_3_2_Core,
    // we need to pass our OpenGL context to the Drawable in order to call GL functions
    // from within this class.
};

#endif  // SHADERPROGRAM_H
