#include "painted_item.h"
#include <QPainter>
#include <QtGui/QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QGLShader>
#include <QQuickPaintedItem>

#include <GLES2/gl2.h>

#include <assert.h>

PaintedOpenGL::PaintedOpenGL(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_prog(0)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

void PaintedOpenGL::paint(QPainter *painter)
{
    painter->beginNativePainting();
    if(m_prog == 0)
    {
        bool result;
        m_prog = 1;
        QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
        const char *vscr =
                "attribute vec4 vPosition;                 \n"
                "uniform mat4 matrix;                      \n"
                "varying vec4 lPosition;                   \n"
                "void main()                               \n"
                "{                                         \n"
                "  gl_Position = matrix * vPosition;       \n"
                "  lPosition = vPosition;                  \n"
                "}                                         \n";
        result = vshader->compileSourceCode(vscr);
        assert(result);

        QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
        const char *fscr =
                "precision mediump float;                     \n"
                "uniform mat4 colorMatrix;                    \n"
                "varying vec4 lPosition;                      \n"
                "void main()                                  \n"
                "{                                            \n"
                "  gl_FragColor = colorMatrix*lPosition;      \n"
                "  gl_FragColor[3] = 1.0;                     \n"
                "}";
        result = fshader->compileSourceCode(fscr);
        assert(result);

        program.addShader(vshader);
        program.addShader(fshader);
        program.link();
        m_prog = program.programId();

        vPositionLoc = glGetAttribLocation(m_prog, "vPosition");
    }
    glUseProgram ( m_prog );

    GLfloat tab_matrix[] = { 0.7 , 0.0 , -0.7 , 0.0 ,
                             0.4 , 0.8 , 0.4 , 0.0 ,
                             0.57 , -0.57 , 0.57 , 0.0 ,
                             0.0 , 0.0 , 0.0 , 1.0
                           };


    GLfloat colorMatrix[] = { 1.0, 0, 0, 0,
                              0,   1.0, 0, 0,
                              0,  0,   1.0,  0,
                              0,  0,   0,   1.0};

    glUniformMatrix4fv(glGetUniformLocation(m_prog, "matrix"), 1, GL_FALSE, tab_matrix);
    glUniformMatrix4fv(glGetUniformLocation(m_prog, "colorMatrix"), 1, GL_FALSE, colorMatrix);

    //Load vertices to GPU memory
    int verticesNum = 4;
    GLfloat vVertices[] = {
                             1.0f,  0.0f, 0.0f,
                             0.0f,  1.0f, 0.0f,
                             0.0f,  0.0f, 1.0f,
                             0.0f,  0.0f, 0.0f
                          };
    int indecesNum = 12;
    GLushort indeces[] = { 0, 1, 2,
                           1, 2, 3,
                           0, 1, 3,
                           0, 2, 3};

    GLuint buffersId[2];
    glGenBuffers(2, buffersId);

    glBindBuffer(GL_ARRAY_BUFFER, buffersId[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesNum*3*sizeof(GLfloat), vVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffersId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecesNum*sizeof(GLushort), indeces, GL_STATIC_DRAW);

    // Draw verticies
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray ( vPositionLoc );

    GLuint offset = 0;
    glVertexAttribPointer ( vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, (const void*) offset );
    glBindAttribLocation(m_prog, vPositionLoc, "vPosition");
    glDrawElements(GL_TRIANGLES, indecesNum, GL_UNSIGNED_SHORT, (const void*)0);

    GLfloat zeroMatrix[] = { 0.0, 0, 0, 0,
                             0,   0.0, 0, 0,
                             0,  0,   0.0,  0,
                             0,  0,   0,   0.0};
    glUniformMatrix4fv(glGetUniformLocation(m_prog, "colorMatrix"), 1, GL_FALSE, zeroMatrix);
    glDrawElements(GL_LINES, indecesNum, GL_UNSIGNED_SHORT, (const void*) 0);

    glDisableVertexAttribArray(vPositionLoc);

    glUseProgram(0);

    glDeleteBuffers(2, buffersId);
    painter->endNativePainting();
}
