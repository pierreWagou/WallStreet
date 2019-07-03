#pragma once

#include <QWidget>
#include <QtCharts>
#include <QtXml>
#include "bougie.h"
#include "evolutionCours.h"

class EvolutionViewer : public QWidget{
  Q_OBJECT
  EvolutionCours* evolution;
  QChart *chartCours;
  QBarSet* setVolume;
  QBarSeries* seriesVolume;
  QChart *chartVolume;
  QLineEdit* open;
  QLineEdit* high;
  QLineEdit* low;
  QLineEdit* close;
  QLineEdit* volume;
  QLineEdit* forme;
  QPushButton* loadButton;
  QPushButton* noteButton;
  QPushButton* editButton;
  QPushButton *launchButton;
  QPushButton* saveButton;
  Bougie* lastBougieClicked = nullptr;
  QFile* xmlFile;
  void updateCharts();
public:
  explicit EvolutionViewer(EvolutionCours* e, QFile* file=nullptr, QWidget *parent = nullptr);
private slots:
  void loadCSV();
  void takeNotes();
  void viewCoursOHLC(Bougie* c);
  void saveCoursOHLC();
  void launch();
  void save();
};
