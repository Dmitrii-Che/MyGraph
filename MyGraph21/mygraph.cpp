#include "mygraph.h"
#include <QPainter>
#include <iostream>
#include <QDebug>
#include <QFileDialog>
//#include <math.h>

static QMap<QString, QColor> GraphColors {
    {"green", Qt::green},
    {"blue", Qt::blue},
    {"red", Qt::red},
    {"black", Qt::black},
    {"cyan", Qt::cyan},
};
MyGraph::MyGraph(QWidget* parent) :
    QMainWindow(parent)
{
    setupUi(this);
    _TG2.color =  Qt::red;
    makingMenu();
    makingConnection();
}

MyGraph::~MyGraph()
{
    delete _pCchecableAutoApply;
}
void MyGraph::openFile()
{
    QFileDialog* fdlg = new QFileDialog;
    _fileName = fdlg->getOpenFileName(this, "Graph file opening", QCoreApplication::applicationDirPath(), "*.ssd *.rsd");

    clearGraph();

    loadFromSSD();

    if (_pCchecableAutoApply->isChecked()) {
        drawGraph();
    }
}
void MyGraph::openFile(const QString &fileName)
{
    clearGraph();

    _fileName = fileName;
    loadFromSSD();
    if (_pCchecableAutoApply->isChecked()) {
        drawGraph();
    }
}
void MyGraph::clearGraph()
{
    _tgVectorToDraw.clear();
    update();
}
void MyGraph::setXLegendText(const QString &str)
{
    XLegend->setText(str);
}
void MyGraph::setYLegendText(const QString &str)
{
    YLegend->setText(str);
}
void MyGraph::setColor(const QColor &clr)
{
    _TG2.color =  clr;
}
void MyGraph::makingMenu()
{
    /*Menu***************************************/
    QMenu* menu = new QMenu("menu");
    menu->addAction("&Help", this, [] {

    }, Qt::Key_F1);

    menu->addSeparator();

    QMenu* graphMenu = new QMenu("&Graph", menu);
    menu->addMenu(graphMenu);

    graphMenu->addAction(
        "&Open File", this, [=] { openFile(); }, Qt::CTRL | Qt::Key_F);

    graphMenu->addAction(
        "&Draw", this, [=] { drawGraph(); }, Qt::CTRL | Qt::Key_D);

    graphMenu->addAction(
        "&Clear", this, [=] { clearGraph(); }, Qt::CTRL | Qt::Key_C);

    graphMenu->addSeparator();
    QMenu* colorMenu = new QMenu("&Colors", graphMenu);
    graphMenu->addMenu(colorMenu);


    /*****************Entering colors***************/
    for(auto color : GraphColors.keys()){
        colorMenu->addAction(color, this, [=] {
            setColor(GraphColors[color]);
            drawGraph();
        });
    }
    /*****************End Entering colors***********/

    graphMenu->addSeparator(); /*разделение*/

    _pCchecableAutoApply = menu->addAction("&Auto Drawing", this, [] {});
    _pCchecableAutoApply->setCheckable(true);
    _pCchecableAutoApply->setChecked(true);

    menu->addSeparator(); /*разделение*/
    menu->addAction(
        "&Exit", this, [this] { close(); }, Qt::CTRL | Qt::Key_Q);
    menuBarGraf->addMenu(menu);
    /*END Menu***************************************/
}
void MyGraph::makingConnection()
{
    connect(_pCchecableAutoApply, &QAction::changed, this, [=] {
        if (_pCchecableAutoApply->isChecked()) {
            drawGraph();
        }
    });
}
void MyGraph::findMaxY()
{
        if(!_TG2.pVector.isEmpty()) {
            _MaxY = _TG2.pVector[0].y();

                for(auto point : _TG2.pVector) {
                    qreal tempPoint = point.y();

                    if(tempPoint > _MaxY) {
                        _MaxY = tempPoint;
                    }
                }
        }
}
void MyGraph::findMinY()
{
        if(!_TG2.pVector.isEmpty()) {
            _MinY = _TG2.pVector[0].y();

                for(auto point : _TG2.pVector) {
                    qreal tempPoint = point.y();

                    if(tempPoint < _MinY) {
                        _MinY = tempPoint;
                    }
                }
        }
}
void MyGraph::findMaxX()
{
        if(!_TG2.pVector.isEmpty()) {
            _MaxX = _TG2.pVector[0].x();

                for(auto point : _TG2.pVector) {
                    qreal tempPoint = point.x();

                    if(tempPoint > _MaxX) {
                        _MaxX = tempPoint;
                    }
                }
        }
}
void MyGraph::findMinX()
{
        if(!_TG2.pVector.isEmpty()) {
            _MinX = _TG2.pVector[0].x();

                for(auto point : _TG2.pVector) {
                    qreal tempPoint = point.x();

                    if(tempPoint < _MinX) {
                        _MinX = tempPoint;
                    }
                }
        }
}
void MyGraph::findAllMaxMin()
{
    findMaxY();
    findMinY();
    findMaxX();
    findMinX();
}
void MyGraph::findScreenKoefs()
{
    findAllMaxMin();

    _rangeX = abs(_MaxX + _MinX);
    _rangeY = abs(_MaxY + _MinY);

    zeroCorrection(_rangeX);
    zeroCorrection(_rangeY);

    qreal _tempXParam = abs(_MaxX - _MinX);
    qreal _tempYParam = abs(_MaxY - _MinY);

    zeroCorrection(_tempXParam);
    zeroCorrection(_tempYParam);

    _screenKoefX = ((width() - _border * 2)) * _rangeX / _tempXParam;
    _screenKoefY = ((height() - _border * 3)) * _rangeY / _tempYParam;

    zeroCorrection(_screenKoefX);
    zeroCorrection(_screenKoefY);
}
void MyGraph::makingSkaleStep()
{
    _scaleXStepValue = _screenKoefX / _rangeX;
    _scaleYStepValue = _screenKoefY / _rangeY;

    qreal tempXcoef = ((_w - _border * 2) / _scaleXStepValue);
    qreal tempYcoef = ((_ht - _border * 2) / _scaleYStepValue);

    while(tempXcoef < 10){
    _scaleXStepValue /= 10;
    tempXcoef = ((_w - _border * 2) / _scaleXStepValue);
    }

    while (tempXcoef > 50) {
        _scaleXStepValue *= 10;
        tempXcoef = ((_w - _border * 2) / _scaleXStepValue);
    }

    setXLegendText(QString("X Step: %1").arg(_scaleXStepValue * _rangeX / _screenKoefX));

    while(tempYcoef < 10){
    _scaleYStepValue /= 10;
    tempYcoef = ((_ht - _border * 2) / _scaleYStepValue);
    }

    while(tempYcoef > 30){
    _scaleYStepValue *= 10;
    tempYcoef = ((_ht - _border * 2) / _scaleYStepValue);
    }

    setYLegendText(QString("Y Step: %1").arg(_scaleYStepValue * _rangeY / _screenKoefY));
}
void MyGraph::loadFromSSD()
{
    _reader.readFromFile(_fileName);
    _TG2.pVector =_reader.getPoints();
}
void MyGraph::drawing(QPainter &p)
{
    int xScaleLineWidth = _w - _border * 2;
    QPen penLine;
    QPen penPoint;
    QPolygon arrowY;
    QPolygon arrowX;
    arrowY << QPoint(_wt - _arrowCoef, _arrowCoef * 5 + _border) << QPoint(_wt + _arrowCoef, _arrowCoef * 5 + _border)
          << QPoint(_wt, _arrowCoef * 2 + _border) << QPoint(_wt - _arrowCoef, _arrowCoef * 5 + _border);
    arrowX << QPoint(_wt + xScaleLineWidth, _ht - _arrowCoef) << QPoint(_wt + xScaleLineWidth + _arrowCoef * 3, _ht)
          << QPoint(_wt + xScaleLineWidth, _ht + _arrowCoef) << QPoint(_wt + xScaleLineWidth, _ht - _arrowCoef);
    p.drawLine(_wt, _arrowCoef * 5 + _border, _wt, _ht); /*Y Line*/
    p.drawLine(_wt, _ht, _wt + xScaleLineWidth, _ht); /*X Line*/
    p.drawPolyline(arrowY);
    p.drawPolyline(arrowX);
    QPointF point;
    QVector<QPointF> pVectorForLines;

    for (auto TG : _tgVectorToDraw) {
        pVectorForLines.clear();
        penLine.setWidth(1);
        penLine.setColor(TG.color);

        if (!TG.pVector.isEmpty()) {
            for(auto tempPoint : TG.pVector) {
                point.setX(_wt + (tempPoint.rx() * _screenKoefX - _MinX * _screenKoefX) / _rangeX);
                point.setY(_ht - (tempPoint.ry() * _screenKoefY - _MinY * _screenKoefY) / _rangeY);
                pVectorForLines.append(point);
            }

            p.setPen(penLine);
            p.drawPolyline(pVectorForLines);/*lines*/
            /************************************/
            penPoint.setWidth(5);
            penPoint.setColor(TG.color);
            p.setPen(penPoint);
            p.drawPoints(pVectorForLines);/*points*/

/***********Scale***********************************************************************************************/
            {
            penPoint.setColor(Qt::black);
            p.setPen(penPoint);
            makingSkaleStep();

            for(qreal i = 0; i <= _w - _border * 2; i += _scaleXStepValue) {
                p.drawPoint(i + _border, _ht);
            }

            for(qreal i = 0; i <= _ht - _border * 2; i += _scaleYStepValue) {
                p.drawPoint(_wt, _ht - i);
            }
            }
        }
    }
}
void MyGraph::drawGraph()
{
    _tgVectorToDraw.clear();
    _tgVectorToDraw.append(_TG2);
    findScreenKoefs();

    update();
}
/*****************************************************************************/
void MyGraph::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    _w = width();/*X*/
    _h = height();/*Y*/
    _wt = _border; /*X0*/
    _ht = _h - _border;/*Y0*/
    drawing(p);
}
void MyGraph::resizeEvent(QResizeEvent *event)
{
    drawGraph();
}










