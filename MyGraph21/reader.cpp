#include "reader.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QRegularExpression>

Reader::Reader()
{
    _pointVector.clear();
}

void Reader::readFromFile(const QString &filename)
{
    QFile* file = new QFile(filename);
    QRegularExpression re("^"
                          "("
                          "(\\d.\\d*e[-|+]\\d{3}\\s\\d.\\d*e[-|+]\\d{3})"
                          "|(\\d.\\d*\\s\\d.\\d*)"
                          "|(\\d*\\s\\d*)"
                          ")"
                          "((\\r\\n)|\\r|)"
                          "$");
    _pointVector.clear();
    if (!file->open(QIODevice::ReadOnly)) {
        //        qDebug()  << "File Opening Erroe";
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "File Opening Erroe", QMessageBox::Ok);
        msgBox.exec();
    } else {
        QString str;
        unsigned stringCounter = 0, errorCounter = 0;
        do {
            str = file->readLine(100);
            if (*str.begin() != '#' && *str.begin() != '\0') {
                try {
                    qreal valueX = 0.0, valueY = 0.0;
                    auto match = re.match(str);
                    if (!match.hasMatch()) {
                        throw std::runtime_error("not correct string");
                    }

                    QStringList strList = str.split(" ");
                    if (str == "\r\n") {
                        throw std::runtime_error("empty string");
                    }
                    valueX = strList[0].toDouble();
                    if (strList.size() <= 1) {
                        throw std::runtime_error("not correct string");
                    }
                    valueY = strList[1].split("\r\n")[0].toDouble();

                    _pointVector.append({valueX, valueY});
                    stringCounter++;
                } catch (const std::exception &e) {
                    //qDebug() << "string:" << stringCounter << "-> read exception: " << e.what();
                    errorCounter++;
                }
            }
        } while (str != "");

        if (errorCounter) {
            QMessageBox msgBox(QMessageBox::Warning,
                               "Warning",
                               QString(
                                   "The document is damaged.\r\nThere is: %1 errors per %2 strings")
                                   .arg(errorCounter)
                                   .arg(stringCounter),
                               QMessageBox::Ok);
            msgBox.exec();
        }
    }
}

QVector<QPointF> Reader::getPoints() const
{
    return _pointVector;
}
