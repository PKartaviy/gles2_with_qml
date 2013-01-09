#include <QApplication>
#include <QQuickView>
#include "painted_item.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    qmlRegisterType<PaintedOpenGL>("Shapes", 1, 0, "MyTriangle");
    QQuickView view;
    view.setSource(QUrl("../qml/test.qml"));
    view.show();

    return a.exec();
}
