#ifndef READER_H
#define READER_H

#include <QObject>
#include <QPointF>

class Reader
{
public:
    Reader();
    void readFromFile(const QString &filename);
    QVector<QPointF> getPoints() const;
private:
    QVector<QPointF> _pointVector;

};

#endif // READER_H
