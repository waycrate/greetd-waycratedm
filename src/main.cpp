#ifndef DEBUG_MODE
#include <SessionLockQt/command.h>
#include <SessionLockQt/shell.h>
#include <SessionLockQt/window.h>
#endif

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QWindow>
#include <cstdlib>

int
main(int argc, char *argv[])
{
#ifndef DEBUG_MODE
    ExtSessionLockV1Qt::Shell::useExtSessionLock();
#endif

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    auto screens = QGuiApplication::screens();

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/WayCrateLock/qml/main.qml"_qs);
    QObject::connect(
      &engine,
      &QQmlApplicationEngine::objectCreated,
      &app,
      [url](QObject *obj, const QUrl &objUrl) {
          if (!obj && url == objUrl)
              QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

    for (auto screen : screens) {
        engine.load(url);
        if (QWindow *window = qobject_cast<QWindow *>(engine.rootObjects().last())) {
#ifndef DEBUG_MODE
            ExtSessionLockV1Qt::Window::registerWindowFromQtScreen(window, screen);
#endif
            window->show();
        } else {
            qDebug() << "Cannot get window";
            return 0;
        }
    }
    QObject::connect(&app, &QGuiApplication::screenAdded, &app, [&engine, url](auto screen) {
        engine.load(url);
        if (QWindow *window = qobject_cast<QWindow *>(engine.rootObjects().last())) {
#ifndef DEBUG_MODE
            ExtSessionLockV1Qt::Window::registerWindowFromQtScreen(window, screen);
#endif
            window->show();
        } else {
            qDebug() << "Cannot get window";
            exit(0);
        }
#ifndef DEBUG_MODE
        ExtSessionLockV1Qt::Command::instance()->LockScreen();
#endif
    });
#ifndef DEBUG_MODE
    ExtSessionLockV1Qt::Command::instance()->LockScreen();
#endif

    return app.exec();
}
