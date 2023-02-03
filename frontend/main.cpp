#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QMetaType>
#include <QDebug>
#include <QObject>
#include <QQuickWindow>
#include <QFileInfo>
#include <comdef.h>
#include "authorizationmanager.h"
#include "configurator.h"

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#include <TlHelp32.h>
#endif

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

bool checkProcNumber(std::string procfilename);

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    if(!checkProcNumber(QFileInfo(QCoreApplication::applicationFilePath()).fileName().toStdString()))
        return -1;
    AuthorizationManager authManager;
    Configurator conf;
    QObject::connect(&authManager, &AuthorizationManager::sendConfigRememberUser, &conf, &Configurator::rememberUser);
    auto confInfo = conf.getConfigInfo();
#ifndef QT_DEBUG
    conf.setAutostartApplication(true);
#endif

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url, &authManager, &confInfo](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
        if(confInfo.rememberUser)
            authManager.quickAuth(confInfo.username, confInfo.passhash);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("authManager", &authManager);
    QObject::connect(&app, &QGuiApplication::aboutToQuit, &authManager, &AuthorizationManager::sendDisconnect);

    engine.load(url);

    QObject* windowObj = engine.rootObjects().at(0);
    if(windowObj)
    {
        QWindow* win = qobject_cast<QWindow*>(windowObj);
        //Конвертация property компонента в окно
        QWindow* workspace = qvariant_cast<QWindow*>(win->property("window"));
        QObject::connect(workspace, &QWindow::windowStateChanged, [workspace](Qt::WindowState state) {
            if(state == Qt::WindowMinimized){
                workspace->hide();
            }
        });
        QObject::connect(windowObj, SIGNAL(qmlAuth(QString,QString,bool)), &authManager, SLOT(auth(QString,QString,bool)));
        QObject::connect(&authManager, SIGNAL(sendAuthResult(QVariant,QVariant)), windowObj, SLOT(getAuthResult(QVariant,QVariant)));
        QObject::connect(&authManager, SIGNAL(sendAccountData(QVariant,QVariant,QVariant)), windowObj, SLOT(addAccount(QVariant,QVariant,QVariant)));
        QObject::connect(&authManager, SIGNAL(sendUserExit()), windowObj, SLOT(userExit()));
        QObject::connect(&authManager, SIGNAL(sendMessageDialog(QVariant,QVariant)), windowObj, SLOT(showMessageBox(QVariant,QVariant)));
        QObject::connect(windowObj, SIGNAL(qmlSend(int,QString)), &authManager, SLOT(sendMessage(int,QString)));
        QObject::connect(windowObj, SIGNAL(qmlSaveCurrentUser()), &authManager, SLOT(saveCurrentUser()));
    }

    return app.exec();
}

bool checkProcNumber(std::string procfilename)
{
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 p32;
    p32.dwSize = sizeof(p32);
    BOOL hres = Process32First(hsnap, &p32);
    int cnt = 0;
    while (hres) {
        _bstr_t t(p32.szExeFile); //for wchar to char
        const char* str = t;
        if(strcmp(str, procfilename.c_str()) == 0) ++cnt;
        hres = Process32Next(hsnap, &p32);
    }
    CloseHandle(hsnap);
    if(cnt > 1) return false;
    return true;
}
