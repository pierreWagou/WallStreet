#pragma once
#include <QWidget>
#include <QtCharts>
#include <QtXml>
#include "transaction.h"
#include "transactionManager.h"
#include "bougie.h"
#include "evolutionCours.h"

class TransactionViewer : public QWidget {
  Q_OBJECT
  QString mode;
  EvolutionCours evolutionCours;
  EvolutionCours::iterator iterator;
  double broker;
  double base;
  double contrepartie;
  double initMontant;
  QDate debut;
  QDate current;
  QDate fin;
  QFile* xmlFile;
  QLineEdit* dateEdit;
  QLineEdit* prixEdit;
  QLineEdit* baseEdit;
  QLineEdit* contrepartieEdit;
  QLineEdit* achatEdit;
  QLineEdit* venteEdit;
  QPushButton* previousButton;
  QPushButton* nextButton;
  QCandlestickSeries* seriesCours;
  QBarSeries* seriesVolume;
  QBarSet* setVolume;
  QBarCategoryAxis* axisCours;
  QBarCategoryAxis* axisVolume;
  QValueAxis* axisYCours;
  QValueAxis* axisYVolume;
  QTableView* transactionTableView;
  QStandardItemModel* transactionModel;
  void addTransaction();
  void updateCharts();
public:
  TransactionViewer(QString mode, EvolutionCours* e, QDate& d, QDate& cu, QDate& f, double bk, double b=0, double c=1000000, QFile* file=nullptr);
  ~TransactionViewer() = default;
private slots:
  void acheter();
  void vendre();
  void nextDay();
  void previousDay();
  void save();
};
