#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "use_case_histogram_impl.h"
#include "use_case_reader_impl.h"
#include <QQmlContext>
#include <QQmlEngine>
#include <QFontDatabase>
#include <QIcon>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/histogram/images/histogram_logo.png"));

    histogram::HistogramUseCaseImpl histogram_use_case;
    data_reader::ReaderUseCaseImpl reader_use_case;

    auto hist_model = std::make_shared<histogram::HistogramModel>(reader_use_case);
    auto reader = std::make_shared<data_reader::DataReader>(histogram_use_case);

    histogram_use_case.SetHistogram(hist_model);
    reader_use_case.SetReader(reader);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/histogram/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("chartModel", hist_model.get());
    QObject::connect(&engine, &QQmlEngine::quit, [&]() {
        engine.rootContext()->setContextProperty("chartModel", QVariant());
        hist_model->deleteLater();
    });
    engine.load(url);

    return app.exec();
}
