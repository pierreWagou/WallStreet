#pragma once

#include <QWidget>
#include <QtCharts>
#include <QtXml>
#include "evolutionCours.h"
#include "evolutionViewer.h"

class Configuration : public QWidget {
  Q_OBJECT
private:
  QDialog* creationDialog;
  QComboBox* baseCombo;
  QComboBox* contrepartieCombo;
  QLineEdit* nomEdit;
  QLineEdit* geoEdit;
  QLineEdit* codeEdit;
  QFile* xmlFile;
  void initCreationDialog();
public:
  Configuration(QFile* file=nullptr);
private slots:
  void create();
  void go();
};
