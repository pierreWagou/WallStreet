#pragma once

#include <QWidget>
#include <QtCharts>
#include "coursOHLC.h"

class Bougie : public QCandlestickSet {
  Q_OBJECT
  CoursOHLC* cours;
public:
  Bougie(qreal open, qreal high, qreal low, qreal close, CoursOHLC* c, qreal timestamp = 0.0, QObject *parent = nullptr);
  CoursOHLC& getCoursOHLC();
  QString getForme();
  const CoursOHLC& getCoursOHLC() const;
signals:
  void clickBougie(Bougie* c);
private slots:
  void viewCoursOHLC();
};
