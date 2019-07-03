#pragma once

#include "paireDevise.h"
#include "coursOHLC.h"

class EvolutionCours {
  const PaireDevise* paire;
	CoursOHLC* cours = nullptr;
	unsigned int nbCours = 0;
	unsigned int nbMaxCours = 0;
public:
  EvolutionCours(const PaireDevise& p);
  void addCours(double o, double h, double l, double c, double v, const QDate& d);
	~EvolutionCours();
	EvolutionCours(const EvolutionCours* e);
	EvolutionCours& operator=(const EvolutionCours& e);
  unsigned int getNbCours() const;
  double getRSI(const QDate debut, const QDate fin) const;
  double getSMA(const QDate debut, const QDate fin) const;
  double getEMA(const QDate debut, const QDate fin) const;
  double getMACD(const QDate debut1, const QDate fin1, const QDate debut2, const QDate fin2) const;
  const PaireDevise& getPaireDevises() const;
  using iterator = CoursOHLC*;
	iterator begin();
	iterator end();
};
