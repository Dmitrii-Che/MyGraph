#ifndef MYGRAPH_H
#define MYGRAPH_H

#include "ui_mygraph.h"
#include <QTableWidget>

#include "reader.h"

struct TGraph {
    QVector<QPointF> pVector;
    QColor color;
};

class MyGraph : public QMainWindow, private Ui::MyGraph {
        Q_OBJECT

    public:
        explicit MyGraph(QWidget* parent = nullptr);
        ~MyGraph();
        void drawGraph();
        void openFile();
        void openFile(const QString & fileName);
        void clearGraph();
        void setXLegendText(const QString & str);
        void setYLegendText(const QString & str);
        void setColor(const QColor & clr);

    private:

        template <typename T>
        void zeroCorrection(T &value)
        {
            if(value == 0) {
                value = 1;
            }
        }

        void makingMenu();
        void makingConnection();
        void makingSkaleStep();

        void findMaxY();
        void findMinY();
        void findMaxX();
        void findMinX();
        void findAllMaxMin();
        void findScreenKoefs();

        void loadFromSSD();
        void drawing(QPainter &p);

        /**/
        void paintEvent(QPaintEvent* ) override;
        /**/

    private:
        QAction *_pCchecableAutoApply = nullptr;

        QString _fileName;
        Reader _reader;
        QVector<TGraph> _tgVectorToDraw;
        qreal _MaxX = 0,
        _MinX = 0,
        _MaxY = 0,
        _MinY = 0,
        _screenKoefX = 0,
        _screenKoefY = 0,
        _scaleXStepValue = 0,
        _scaleYStepValue = 0,
        _rangeX = 1,
        _rangeY = 1;

        TGraph _TG2;

        int _w = 0,
        _h = 0,
        _wt = 0,
        _ht = 0;

        const int _border = 20;
        const int _arrowCoef = 5;

protected:
        void resizeEvent(QResizeEvent *event) override;
};

#endif // MYGRAPH_H
