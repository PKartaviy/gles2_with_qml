#include <QApplication>
#include <QQuickView>
#include <QQmlEngine>
#include "painted_item.h"
#include <assert.h>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    qmlRegisterType<PaintedOpenGL>("Shapes", 1, 0, "MyTriangle");
    QQuickView view;
    view.setSource(QUrl("../qml/test.qml"));
    a.connect(view.engine(), SIGNAL(quit()), &a, SLOT(quit()) );
    view.show();

    return a.exec();
}
