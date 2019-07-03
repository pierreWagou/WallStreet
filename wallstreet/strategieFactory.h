#pragma once

#include <QDebug>
#include <QWidget>
#include <QtCharts>
#include "transaction.h"
#include "transactionManager.h"
#include "bougie.h"
#include "evolutionCours.h"
#include "strategie.h"

class StrategieFactory : public QWidget {
    QList<QString> strategieName;
  public:
      StrategieFactory();
      QList<QString> getStrategiName();
      Strategie* factory(QString type,QDate& debut, QDate& fin, double montantB,double montantC,double broker, EvolutionCours* e);
  };
