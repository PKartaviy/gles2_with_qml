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

    // Draw verticies
    GLfloat vVertices[] = {  // first triangle
                             0.0f,  0.0f, 0.0f,
                             1.0f,  0.0f, 0.0f,
                             0.0f,  1.0f, 0.0f,
                             // second triangle
                             0.0f,  0.0f, 0.0f,
                             1.0f,  0.0f, 0.0f,
                             0.0f,  0.0f, 1.0f,
                             //third triangle
                             0.0f,  0.0f, 0.0f,
                             0.0f,  1.0f, 0.0f,
                             0.0f,  0.0f, 1.0f,
                             //forth triangle
                             1.0f,  0.0f, 0.0f,
                             0.0f,  1.0f, 0.0f,
                             0.0f,  0.0f, 1.0f
                          };


    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vPositionLoc);
    glEnableVertexAttribArray ( vPositionLoc );
    glVertexAttribPointer ( vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, vVertices );

    glDrawArrays ( GL_TRIANGLES, 0, 12 );

    GLfloat zeroMatrix[] = { 0.0, 0, 0, 0,
                              0,   0.0, 0, 0,
                              0,  0,   0.0,  0,
                              0,  0,   0,   0.0};
    glUniformMatrix4fv(glGetUniformLocation(m_prog, "colorMatrix"), 1, GL_FALSE, zeroMatrix);
    glDrawArrays(GL_LINES, 0, 12);

    glDisableVertexAttribArray(vPositionLoc);

    glUseProgram(0);

    painter->endNativePainting();
}
