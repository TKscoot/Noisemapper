#include "mainwindow.h"

#include <QApplication>
#include <QDockWidget>
#include <QFile>
#include <QPushButton>
#include <QTextStream>
#include <QDir>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <iostream>

void customLogHandler(QtMsgType type, const QMessageLogContext& context,
                      const QString& msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // custom handler start
    qInstallMessageHandler(&customLogHandler);

    QFile f(":/qdarkstyle/dark/darkstyle.qss");
    auto p = QDir::currentPath();
    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
    }

    MainWindow w;
    w.show();

    return a.exec();
}

// example for custom message handler
void customLogHandler(QtMsgType type, const QMessageLogContext& context,
                      const QString& msg)
{
    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        std::cout << msg.toStdString() << std::endl;
        break;
    case QtWarningMsg:
    case QtCriticalMsg:
    case QtFatalMsg:
        std::cerr << msg.toStdString() << std::endl;
        break;
    default:
        std::cout << msg.toStdString() << std::endl;
        break;
    }
}
