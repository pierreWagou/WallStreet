#pragma once

#include <QtCharts>
#include <QtXml>

class Home : public QWidget {
Q_OBJECT
public:
  Home();
private slots:
  void load();
  void newSimulation();
};
