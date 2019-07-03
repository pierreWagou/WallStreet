#include "strategieFactory.h"

StrategieFactory::StrategieFactory() {
  strategieName.append("StrategieForme");
  strategieName.append("StrategieRSI");
  strategieName.append("StrategieSMA");
}

Strategie* StrategieFactory::factory(QString type,QDate& debut, QDate& fin, double montantB,double montantC,double broker, EvolutionCours* e){
    if (type == "StrategieForme"){
        return new StrategieForme(debut, fin, montantB, montantC,broker, e);
    }
    if (type == "StrategieRSI"){
        return new StrategieRSI(debut, fin, montantB, montantC, broker, e);
    }
    if (type == "StrategieSMA"){
        return  new StrategieSMA(debut, fin, montantB, montantC, broker, e);
    }
    return  nullptr;
}

QList<QString> StrategieFactory::getStrategiName() {
  return strategieName;
}
