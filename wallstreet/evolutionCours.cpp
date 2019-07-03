#include "evolutionCours.h"
#include <QDebug>

EvolutionCours::EvolutionCours(const PaireDevise& p): paire(&p) {
	nbCours = 0;
	nbMaxCours = 0;
}

void EvolutionCours::addCours(double o, double h, double l, double c, double v, const QDate& d) {
	if (nbMaxCours == nbCours) {
		CoursOHLC* newtab = new CoursOHLC[nbMaxCours + 100];
		for (unsigned int i = 0; i < nbCours; i++) newtab[i] = cours[i];
		CoursOHLC* old = cours;
		cours = newtab;
		nbMaxCours += 100;
		delete[] old;
	}
	cours[nbCours].setCours(o, h, l, c, v);
  cours[nbCours].setDate(d);
	nbCours++;
}

EvolutionCours::~EvolutionCours() {
	delete[] cours;
}

EvolutionCours::EvolutionCours(const EvolutionCours* e) : paire(e->paire), cours(new CoursOHLC[e->nbCours]), nbCours(e->nbCours), nbMaxCours(e->nbCours) {
	for (unsigned int i = 0; i < nbCours; i++)
		cours[i] = e->cours[i];
}

EvolutionCours& EvolutionCours::operator=(const EvolutionCours& e) {
	if (this != &e) {
		paire = e.paire;
		nbCours = 0;
		for (unsigned int i = 0; i < e.nbCours; i++)
      addCours(e.cours[i].getOpen(), e.cours[i].getHigh(),e.cours[i].getLow(), e.cours[i].getClose(), e.cours[i].getVolume(), e.cours[i].getDate());
	}
	return *this;
}

unsigned int EvolutionCours::getNbCours() const {
    return nbCours;
}

const PaireDevise& EvolutionCours::getPaireDevises() const {
	return *paire;
}

double EvolutionCours::getRSI(const QDate debut, const QDate fin) const {
	int compteurHausse=0;
	int compteurBaisse=0;
	double moyenneHausse=0;
	double moyenneBaisse=0;
	for(unsigned int i=0;i<nbCours;i++) {
		if(cours[i].getDate()==debut) {
			for(unsigned int j=0;cours[i+j].getDate()<fin;j++) {
				if(cours[i+j].getOpen()>cours[i+j].getClose()) {
					moyenneBaisse += cours[i+j].getOpen() - cours[i+j].getClose();
					compteurBaisse++;
				}
				else {
					moyenneHausse += cours[i+j].getClose() - cours[i+j].getOpen();
					compteurHausse++;
				}
			}
		}
	}
	moyenneBaisse = moyenneBaisse / compteurBaisse;
	moyenneHausse = moyenneHausse / compteurHausse;
	double rsi = 100 - (100/(1+moyenneHausse/moyenneBaisse));
	return rsi;
}

double EvolutionCours::getSMA(const QDate debut, const QDate fin) const {
	double sma = 0;
	unsigned int j=1;
	for(unsigned int i=0;i<nbCours;i++) {
		if(cours[i].getDate()==debut) {
			for(j=0;cours[i+j].getDate()<fin;j++) {
				sma = sma + cours[i+j].getClose();
			}
		}
	}
	sma = sma / j;
	return sma;
}

double EvolutionCours::getEMA(const QDate debut, const QDate fin) const {
	double ema = 0;
	unsigned int i=0, j=1;
	for(unsigned int i=0;i<nbCours;i++) {
		if(cours[i].getDate()==debut) {
			for(j=0;cours[i+j].getDate()<fin;j++) {
				ema = ema + cours[i+j].getClose();
			}
		}
	}
	ema = ema / j;
	double weight = 2 / (j+1);
	for(j=1;j<nbCours;j++) {
		ema = (cours[i+j].getClose()-ema)*weight + ema;
	}
	return ema;
}

double EvolutionCours::getMACD(const QDate debut1, const QDate fin1, const QDate debut2, const QDate fin2) const {
	double ema1 = getEMA(debut1, fin1);
	double ema2 = getEMA(debut2, fin2);
	double macd = ema1 - ema2;
	return macd;
}

EvolutionCours::iterator EvolutionCours::begin() {
	return iterator(cours);
}

EvolutionCours::iterator EvolutionCours::end() {
	return iterator(cours + nbCours);
}
