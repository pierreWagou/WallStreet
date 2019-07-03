#pragma once

#include "evolutionCours.h"
#include "bougie.h"
#include "evolutionViewer.h"
#include "transaction.h"
#include "transactionManager.h"

#include <QDebug>
#include<QDate>

class Strategie {
protected:
  QDate deb;
  QDate fin;
  double mB_t;
  double mC_t;
  double broker;
  EvolutionCours* evolutionCours;
public:
  virtual ~Strategie() = default;
  Strategie(QDate d, QDate f, double b, double c, double brok, EvolutionCours* evolve):deb(d),fin(f),mB_t(b), mC_t(c), broker(brok), evolutionCours(evolve){}
  virtual void execute(QDate debutSimulation, QDate finSimulation,double montantBase, double montantContrepartie, double broker) = 0;
};

class StrategieForme : public Strategie {
public:
  StrategieForme(QDate d, QDate f, double b, double c,double brok, EvolutionCours* evolve):Strategie(d,f,b,c,brok,evolve){}
  virtual void execute(QDate debutSimulation, QDate finSimulation,double montantBase, double montantContrepartie, double broker);
  virtual QString getTest(){return  "test fille";}
};

class StrategieRSI : public Strategie {
public :
  StrategieRSI(QDate d, QDate f, double b, double c, double brok, EvolutionCours* evolve):Strategie(d, f, b, c, brok, evolve){}
  virtual void execute(QDate debutSimulation, QDate finSimulation,double montantBase, double montantContrepartie, double broker);
};

class StrategieSMA : public Strategie {
public :
  StrategieSMA(QDate d, QDate f, double b, double c, double brok, EvolutionCours* evolve):Strategie(d, f, b, c, brok, evolve){}
  virtual void execute(QDate debutSimulation, QDate finSimulation,double montantBase, double montantContrepartie, double broker);
};
