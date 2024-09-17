#include "mainwindow.h"
#include <QApplication>
#include <QFile>

QString readTextFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QTextStream stream(&file);
    return stream.readAll();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load and apply the stylesheet
    QString styleSheet = readTextFile("../../style/material-blue.qss");
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();

    return a.exec();
}
