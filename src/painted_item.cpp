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
                "void main()                               \n"
                "{                                         \n"
                "  gl_Position = matrix * vPosition;       \n"
                "}                                         \n";
        result = vshader->compileSourceCode(vscr);
        assert(result);

        QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
        const char *fscr =
                "precision mediump float;                     \n"
                "void main()                                  \n"
                "{                                            \n"
                "  gl_FragColor = vec4 ( 0.0, 1.0, 0.0, 1.0 );\n"
                "}";
        result = fshader->compileSourceCode(fscr);
        assert(result);

        program.addShader(vshader);
        program.addShader(fshader);
        program.link();
        m_prog = program.programId();
        glBindAttribLocation ( m_prog, 0, "vPosition" );
    }
    glUseProgram ( m_prog );

    glEnableVertexAttribArray(0);

    GLfloat tab_matrix[] = { 1.0 , 0.0 , 0.0 , 0.0 ,
                             0.0 , 1.0 , 0.0 , 0.0 ,
                             0.0 , 0.0 , 1.0 , 0.0 ,
                             0.0 , 0.0 , 0.0 , 1.0
                           };


    glUniformMatrix4fv(glGetUniformLocation(m_prog, "matrix"), 1, GL_FALSE, tab_matrix);
    GLfloat vVertices[] = {  0.0f,  1.0f, 0.0f,
                             0.5f,  0.5f, 0.0f,
                             0.5f, -1.5f, 0.0f };


    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray ( 0 );

    glDrawArrays ( GL_TRIANGLES, 0, 3 );

    glDisableVertexAttribArray(0);
    glUseProgram(0);

    painter->endNativePainting();
}
