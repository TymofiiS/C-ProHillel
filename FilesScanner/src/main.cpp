
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "FileTableModel.h"
#include "FileController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    FileTableModel model;
    model.addFile("document.pdf",    2300000ULL,   "PDF",    "12.01.25");
    model.addFile("photo.jpg",        800000ULL,   "Image",  "11.01.25");
    model.addFile("video.mp4",     500000000ULL,   "Video",  "10.01.25");
    model.addFile("spreadsheet.xlsx", 1200000ULL,  "Excel",  "09.01.25");
    model.addFile("archive.zip",    15800000ULL,   "Archive","08.01.25");

    FileController controller; // placeholder + progress state

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
