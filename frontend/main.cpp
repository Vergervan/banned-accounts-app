#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QDebug>
#include <QObject>
#include "authorizationmanager.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    AuthorizationManager authManager;
    engine.rootContext()->setContextProperty("authManager", &authManager);

    engine.load(url);

    QObject* windowObj = engine.rootObjects().at(0);
    if(windowObj)
    {
        QObject::connect(windowObj, SIGNAL(qmlAuth(QString, QString, bool)), &authManager, SLOT(auth(QString, QString, bool)));
        QObject::connect(&authManager, SIGNAL(sendAuthResult(QVariant)), windowObj, SLOT(getAuthResult(QVariant)));
    }

    return app.exec();
}
