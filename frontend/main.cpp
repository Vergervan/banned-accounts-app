#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QMetaType>
#include <QDebug>
#include <QObject>
#include "authorizationmanager.h"

struct AccountInfo : public QObject{
    Q_OBJECT
    private:
        Q_PROPERTY(QString username MEMBER m_username NOTIFY usernameChanged)
    signals:
        void usernameChanged();
    public:
        explicit AccountInfo(QObject *parent = 0):
                QObject(parent)
            {}

            AccountInfo(const AccountInfo &) {}
            AccountInfo& operator=(const AccountInfo &){}
        QString m_username;
};
Q_DECLARE_METATYPE(AccountInfo)

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication* app = new QGuiApplication(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    AuthorizationManager authManager;
    engine.rootContext()->setContextProperty("authManager", &authManager);
    QObject::connect(app, &QGuiApplication::aboutToQuit, &authManager, &AuthorizationManager::sendDisconnect);

    engine.load(url);

    QObject* windowObj = engine.rootObjects().at(0);
    if(windowObj)
    {
        QObject::connect(windowObj, SIGNAL(qmlAuth(QString,QString,bool)), &authManager, SLOT(auth(QString,QString,bool)));
        QObject::connect(&authManager, SIGNAL(sendAuthResult(QVariant,QVariant)), windowObj, SLOT(getAuthResult(QVariant,QVariant)));
        QObject::connect(&authManager, SIGNAL(sendAccountData(QVariant,QVariant,QVariant)), windowObj, SLOT(addAccount(QVariant,QVariant,QVariant)));
        QObject::connect(&authManager, SIGNAL(sendUserExit()), windowObj, SLOT(userExit()));
        QObject::connect(windowObj, SIGNAL(qmlSend(int,QString)), &authManager, SLOT(sendMessage(int,QString)));
    }

    return app->exec();
}
