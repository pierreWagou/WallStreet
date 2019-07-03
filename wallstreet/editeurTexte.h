#pragma once

#include <QtCharts>
#include <QtXml>

class EditeurTexte : public QWidget {
Q_OBJECT
  QMenuBar* menuBar;
  QString note;
  QFile* xmlFile;
  QFile* file;
  QTextEdit* textEdit;
  void closeEvent(QCloseEvent *bar);
public:
    EditeurTexte(QString n="", QFile* file=nullptr);
private slots:
  void load();
  void save();
  void saveUnder();
  void saveXml();
};
