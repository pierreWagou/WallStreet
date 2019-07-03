#include "bougie.h"

Bougie::Bougie(qreal open, qreal high, qreal low, qreal close, CoursOHLC* c, qreal timestamp, QObject *parent): QCandlestickSet(open, high, low, close, timestamp, parent), cours(c){
  QObject::connect(this,SIGNAL(clicked()),this, SLOT(viewCoursOHLC()));
}

QString Bougie::getForme() {
  CoursOHLC cours = getCoursOHLC();
  QString forme = cours.getForme();
  return forme;
}

CoursOHLC& Bougie::getCoursOHLC() {
  return *cours;
}

void Bougie::viewCoursOHLC() {
  emit clickBougie(this);
}
