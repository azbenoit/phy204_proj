// #include <QGuiApplication>
// #include <QQmlApplicationEngine>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/timer/timer.hpp>

int main(int argc, char *argv[])
{
// #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//     QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
// #endif
//     QGuiApplication app(argc, argv);

//     QQmlApplicationEngine engine;
//     const QUrl url(QStringLiteral("qrc:/main.qml"));
//     QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                      &app, [url](QObject *obj, const QUrl &objUrl) {
//         if (!obj && url == objUrl)
//             QCoreApplication::exit(-1);
//     }, Qt::QueuedConnection);
//     engine.load(url);

//     return app.exec();
     using namespace boost::lambda;
    typedef std::istream_iterator<int> in;
    // boost::timer::auto_cpu_timer t;
    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
// std::cout << myTimer.elapsed();
    return 0;
}
