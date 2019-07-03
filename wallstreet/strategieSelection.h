#pragma once

#include <QWidget>
#include <QtCharts>
#include <QtXml>
#include "evolutionCours.h"

class StrategieSelection : public QWidget {
Q_OBJECT
int test;
public:
  StrategieSelection(EvolutionCours* e, QFile* f);
 private :
  EvolutionCours *evolutionCours;
  QFile* xmlFile;
  QRadioButton *manualButton;
  QRadioButton *automaticButton;
  QRadioButton *pasButton;
  QHBoxLayout *modeHBox;
  QComboBox *strategieCombo;
  QDateEdit* dateDebut;
  QDateEdit* dateFin;
  QLineEdit* baseEdit;
  QLineEdit* brokerEdit;
  QLineEdit* contrepartieEdit;
private slots:
  void displayStrategie(bool checked);
  void simulate();
};
