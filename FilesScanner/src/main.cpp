
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "FileTableModel.h"
#include "FileController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    FileTableModel model;
    FileController controller(&model);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("fileModel", &model);
    engine.rootContext()->setContextProperty("controller", &controller);

    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
