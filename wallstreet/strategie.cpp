#include "strategie.h"

void StrategieForme::execute(QDate debutSimulation, QDate finSimulation,double montantBase, double montantContrepartie, double broker){
  mB_t=montantBase;
  mC_t=montantContrepartie;
  TransactionManager& manager = TransactionManager::getManager(evolutionCours->getPaireDevises());

  EvolutionCours::iterator it = evolutionCours->begin();
  while (it->getDate() != debutSimulation) {
    it++;
  }
  EvolutionCours::iterator it2;
  for(it2 = it; it2->getDate() != finSimulation; it2++){
    if(it2->getForme() == "O Yang Bozu"){
      double quantite = (0.05*mC_t)/(it2->getOpen());
      manager.creationTransaction(quantite, it2->getDate(),it2->getOpen(),mB_t,mC_t,broker);
    }
    else if(it2->getForme() == "C Yang Bozu"){
      double quantite = (0.1*mC_t)/(it2->getOpen());
      manager.creationTransaction(quantite, it2->getDate(),it2->getOpen(),mB_t,mC_t,broker);
    }
    else if(it2->getForme() == "C Yin Bozu"){
      double quantite = (0.1*mB_t*(-1));
      manager.creationTransaction(quantite, it2->getDate(),it2->getOpen(),mB_t,mC_t,broker);
    }
    else if(it2->getForme() == "O Yin Bozu"){
      double quantite = (0.1*mB_t*(-1));
      manager.creationTransaction(quantite, it2->getDate(),it2->getOpen(),mB_t,mC_t,broker);
    }
    else if(it2->getForme() == "Marubozu"){
      double quantite = (0.15*mC_t)/(it2->getOpen());
      manager.creationTransaction(quantite, it2->getDate(),it2->getOpen(),mB_t,mC_t,broker);
    }
    else if(it2->getForme() == "Morubozu"){
      double quantite = (0.15*mB_t*(-1));
      manager.creationTransaction(quantite, it2->getDate(),it2->getOpen(),mB_t,mC_t,broker);
    }
    else if(it2->getForme() == "Pendu"){
      double quantite = (0.2*mB_t*(-1));
      manager.creationTransaction(quantite, it2->getDate(),it2->getOpen(),mB_t,mC_t,broker);
    }
    else if(it2->getForme() == "Marteau"){
      double quantite = (0.2*mC_t)/(it2->getOpen());
      manager.creationTransaction(quantite, it2->getDate(),it2->getOpen(),mB_t,mC_t,broker);
    }
    else if(it2->getForme() == "Toupie"){
      double quantite = (-1)*(0.1*mC_t)/(it2->getOpen());
      manager.creationTransaction(quantite, it2->getDate(),it2->getOpen(),mB_t,mC_t,broker);
    }
    else if(it2)
    if(!manager.getListTransaction().isEmpty()){
      mB_t = manager.getListTransaction().last()->getMontantBase();
      mC_t = manager.getListTransaction().last()->getMontantContrepartie();
    }
  }
  it2 = it;
}

void StrategieRSI::execute(QDate debutSimulation, QDate finSimulation,double montantBase, double montantContrepartie, double broker){
  mB_t=montantBase;
  mC_t=montantContrepartie;

  TransactionManager& manager = TransactionManager::getManager();

  EvolutionCours::iterator it = evolutionCours->begin();
    while (it->getDate() != debutSimulation) {
      it++;
    }
  EvolutionCours::iterator veille=it;
  it++;
  EvolutionCours::iterator it2;

  for(it2 = it; it2->getDate() != finSimulation; it2++){
    double RSI = evolutionCours->getRSI(veille->getDate(), it2->getDate());
    if(RSI>70){
      double quantite=0.15*mC_t/it2->getOpen();
      manager.creationTransaction(quantite, it2->getDate(), it2->getOpen(), mB_t, mC_t, broker);
    }
    else if(RSI < 30 && RSI > 0) {
      double quantite=-1*0.15*mB_t;
      manager.creationTransaction(quantite, it2->getDate(), it2->getOpen(), mB_t, mC_t, broker);
    }
    if(!manager.getListTransaction().isEmpty()){
      mB_t = manager.getListTransaction().last()->getMontantBase();
      mC_t = manager.getListTransaction().last()->getMontantContrepartie();
    }
    veille++;
  }
}

void StrategieSMA::execute(QDate debutSimulation, QDate finSimulation, double montantBase, double montantContrepartie, double broker){

  mB_t=montantBase;
  mC_t=montantContrepartie;

  TransactionManager& manager = TransactionManager::getManager();

  EvolutionCours::iterator it = evolutionCours->begin();
  while (it->getDate() != debutSimulation) {
    it++;
  }
  EvolutionCours::iterator avant_veille = it;
  it+=2;
  EvolutionCours::iterator it2;
  for(it2=it;it2->getDate()<finSimulation;it2++) {
    double SMA = evolutionCours->getSMA(avant_veille->getDate(), it2->getDate());
    if(it2->getOpen() > SMA){
      if((avant_veille+1)->getOpen() < SMA){
        double quantite=0.1*mC_t/it2->getOpen();
        manager.creationTransaction(quantite, it2->getDate(), it2->getOpen(), mB_t, mC_t, broker);
      }
    }
    else if (it2->getOpen()<SMA) {
      if((avant_veille+1)->getOpen() > SMA){
        double quantite=-1*0.1*mB_t;
        manager.creationTransaction(quantite, it2->getDate(), it2->getOpen(), mB_t, mC_t, broker);
      }
    }
    if(!manager.getListTransaction().isEmpty()){
      mB_t = manager.getListTransaction().last()->getMontantBase();
      mC_t = manager.getListTransaction().last()->getMontantContrepartie();
    }
    avant_veille++;
  }
}
