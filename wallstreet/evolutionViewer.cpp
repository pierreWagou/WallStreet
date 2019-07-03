#include "evolutionViewer.h"
#include "bougie.h"
#include "tradingException.h"
#include "strategieSelection.h"
#include "editeurTexte.h"
#include <QDebug>

EvolutionViewer::EvolutionViewer(EvolutionCours *e, QFile* file, QWidget *parent): QWidget(parent), evolution(e), xmlFile(file) {
  setWindowTitle("Visualisation des données");
  setWindowIcon(QIcon(":/image/logo.png"));

  chartCours = new QChart;
  chartCours->setTitle("Cours OHLC");
  QChartView* chartViewCours = new QChartView(chartCours);
  chartVolume = new QChart;
  chartVolume->setTitle("Volume");
  QChartView* chartViewVolume = new QChartView(chartVolume);

  loadButton = new QPushButton("Charger CSV");
  QObject::connect(loadButton, SIGNAL(clicked()), this, SLOT(loadCSV()));
  noteButton = new QPushButton("Prendre des notes");
  QObject::connect(noteButton, SIGNAL(clicked()), this, SLOT(takeNotes()));
  QLabel* openl = new QLabel("Open");
  openl->setFixedWidth(50);
  open = new QLineEdit;
  open->setFixedWidth(100);
  QHBoxLayout* copen = new QHBoxLayout;
  copen->addWidget(openl);
  copen->addWidget(open);
  QLabel* highl = new QLabel("High");
  highl->setFixedWidth(50);
  high = new QLineEdit;
  high->setFixedWidth(100);
  QHBoxLayout* chigh = new QHBoxLayout;
  chigh->addWidget(highl);
  chigh->addWidget(high);
  QLabel* lowl = new QLabel("Low");
  lowl->setFixedWidth(50);
  low = new QLineEdit;
  low->setFixedWidth(100);
  QHBoxLayout* clow = new QHBoxLayout;
  clow->addWidget(lowl);
  clow->addWidget(low);
  QLabel* closel = new QLabel("Close");
  closel->setFixedWidth(50);
  close = new QLineEdit;
  close->setFixedWidth(100);
  QHBoxLayout* cclose = new QHBoxLayout;
  cclose->addWidget(closel);
  cclose->addWidget(close);
  QLabel* volumel = new QLabel("Volume");
  volumel->setFixedWidth(50);
  volume = new QLineEdit;
  volume->setFixedWidth(100);
  QHBoxLayout* cvolume = new QHBoxLayout;
  cvolume->addWidget(volumel);
  cvolume->addWidget(volume);
  QLabel* formel = new QLabel("Forme");
  formel->setFixedWidth(50);
  forme = new QLineEdit;
  forme->setFixedWidth(100);
  QHBoxLayout* cforme = new QHBoxLayout;
  cforme->addWidget(formel);
  cforme->addWidget(forme);
  editButton = new QPushButton("Editer");
  editButton->setEnabled(false);
  QObject::connect(editButton, SIGNAL(clicked()), this, SLOT(saveCoursOHLC()));
  launchButton = new QPushButton("Z'est barti !");
  launchButton->setEnabled(false);
  QObject::connect(launchButton, SIGNAL(clicked()), this, SLOT(launch()));
  saveButton = new QPushButton("Sauvegarder");
  saveButton->setEnabled(false);
  QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

  QVBoxLayout* coucheCours = new QVBoxLayout;
  coucheCours->addWidget(loadButton);
  coucheCours->addWidget(noteButton);
  coucheCours->addLayout(copen);
  coucheCours->addLayout(chigh);
  coucheCours->addLayout(clow);
  coucheCours->addLayout(cclose);
  coucheCours->addLayout(cvolume);
  coucheCours->addLayout(cforme);
  coucheCours->addWidget(editButton);
  coucheCours->addWidget(launchButton);
  coucheCours->addWidget(saveButton);

  QVBoxLayout* chartBox = new QVBoxLayout;
  chartBox->addWidget(chartViewCours);
  chartBox->addWidget(chartViewVolume);

  QHBoxLayout* fenetre = new QHBoxLayout;
  fenetre->addLayout(chartBox);
  fenetre->addLayout(coucheCours);
  setLayout(fenetre);

  if(evolution->getNbCours()!=0) {
    updateCharts();
    loadButton->setEnabled(false);
  }
}

void EvolutionViewer::loadCSV() {
  QFileDialog* fileDialog = new QFileDialog;
  fileDialog->setFileMode(QFileDialog::ExistingFile);
  QString fileName = fileDialog->getOpenFileName(this, tr("Open File"), QStandardPaths::displayName(QStandardPaths::DesktopLocation), tr("Fichier CSV (*.csv)"));
  if(fileName!="") {
    loadButton->setEnabled(false);
    QFile* file = new QFile(fileName);
    if(!file->open(QIODevice::ReadOnly)) {
      QMessageBox msgBox;
      msgBox.setText("Erreur dans le chargement du fichier.");
      msgBox.exec();
    }
    QString line;
    QList<QDate> dateData;
    QDate tempDate;
    QList<double> openData, highData, lowData, closeData, volumeData;
    line = file->readLine();
    while(!file->atEnd()) {
      line = file->readLine();
      tempDate = QDate::fromString(line.split(',').at(0), "yyyy-MM-dd");
      dateData.append(tempDate);
      openData.append(line.split(',').at(1).toDouble());
      highData.append(line.split(',').at(2).toDouble());
      lowData.append(line.split(',').at(3).toDouble());
      closeData.append(line.split(',').at(4).toDouble());
      volumeData.append(line.split(',').at(6).toDouble());
    }
    for(int i=0;i<dateData.count();i++) {
      evolution->addCours(openData[i], highData[i], lowData[i], closeData[i], volumeData[i], dateData[i]);
    }
    updateCharts();
    launchButton->setEnabled(true);
    saveButton->setEnabled(true);
  }
}

void EvolutionViewer::takeNotes() {
  EditeurTexte* editeurTexte = new EditeurTexte("", xmlFile);
  editeurTexte->setAttribute(Qt::WA_DeleteOnClose);
  editeurTexte->show();

}

void EvolutionViewer::viewCoursOHLC(Bougie* c) {
  open->setText(QString::number(c->getCoursOHLC().getOpen()));
  high->setText(QString::number(c->getCoursOHLC().getHigh()));
  low->setText(QString::number(c->getCoursOHLC().getLow()));
  close->setText(QString::number(c->getCoursOHLC().getClose()));
  volume->setText(QString::number(c->getCoursOHLC().getVolume()));
  forme->setText(c->getForme());
  lastBougieClicked = c;
  editButton->setEnabled(true);
}

void EvolutionViewer::saveCoursOHLC() {
  lastBougieClicked->getCoursOHLC().setCours(open->text().toDouble(), high->text().toDouble(), low->text().toDouble(), close->text().toDouble(), volume->text().toDouble());
  lastBougieClicked->setOpen(open->text().toDouble());
  lastBougieClicked->setHigh(high->text().toDouble());
  lastBougieClicked->setLow(low->text().toDouble());
  lastBougieClicked->setClose(close->text().toDouble());
  QString base = evolution->getPaireDevises().getBase().getCode();
  QString contrepartie = evolution->getPaireDevises().getContrepartie().getCode();
  setVolume = new QBarSet(base+'/'+contrepartie);
  for(EvolutionCours::iterator i=evolution->begin();i!=evolution->end();++i) {
    setVolume->append(i->getVolume());
  }
  seriesVolume->clear();
  seriesVolume->append(setVolume);
}

void EvolutionViewer::launch() {
  save();
  StrategieSelection* strategieSelection = new StrategieSelection(evolution, xmlFile);
  strategieSelection->show();
}

void EvolutionViewer::updateCharts() {
  QBarCategoryAxis* axisCours = new QBarCategoryAxis;
  QValueAxis* axisY = new QValueAxis;
  QBarCategoryAxis* axisVolume = new QBarCategoryAxis;
  double max = 0;
  for(EvolutionCours::iterator i=evolution->begin();i!=evolution->end();++i) {
    if(i->getHigh()>=max) {
      max = i->getHigh();
    }
  }
  double min = max;

  QString base = evolution->getPaireDevises().getBase().getCode();
  QString contrepartie = evolution->getPaireDevises().getContrepartie().getCode();
  QCandlestickSeries* seriesCours = new QCandlestickSeries;
  seriesCours->setName(base+'/'+contrepartie);
  seriesCours->setIncreasingColor(QColor(Qt::green));
  seriesCours->setDecreasingColor(QColor(Qt::red));
  seriesVolume = new QBarSeries;
  setVolume = new QBarSet(base+'/'+contrepartie);

  for(EvolutionCours::iterator i=evolution->begin();i!=evolution->end();++i) {
    if(i->getLow()<=min && i->getLow()>0) {
      min = i->getLow();
    }
    Bougie *set = new Bougie(i->getOpen(), i->getHigh(), i->getLow(), i->getClose(), i);
    QObject::connect(set, SIGNAL(clickBougie(Bougie*)), this, SLOT(viewCoursOHLC(Bougie*)));
    seriesCours->append(set);
    QString categories = i->getDate().toString("yy.MM.dd");
    axisCours->append(categories);
    axisVolume->append(categories);
    setVolume->append(i->getVolume());
  }

  axisY->setMin(min);
  axisY->setMax(max);
  chartCours->addSeries(seriesCours);
  chartCours->addAxis(axisCours, Qt::AlignBottom);
  chartCours->addAxis(axisY, Qt::AlignLeft);
  seriesCours->attachAxis(axisCours);
  seriesCours->attachAxis(axisY);

  seriesVolume->append(setVolume);
  chartVolume->addSeries(seriesVolume);
  chartVolume->addAxis(axisVolume, Qt::AlignBottom);
  seriesVolume->attachAxis(axisVolume);
}

void EvolutionViewer::save() {
  if(!xmlFile->open(QIODevice::ReadWrite)) {
    QMessageBox msgBox;
    msgBox.setText("Erreur dans la sauvegarde de la simulation.");
    msgBox.exec();
  }
  else {
    QDomDocument* dom = new QDomDocument("xml");
    dom->setContent(xmlFile);

    QDomElement root = dom->documentElement();
    QDomElement paire = dom->createElement("pairedevise");
    QDomElement surnom = dom->createElement("surnom");
    QDomElement base = dom->createElement("base");
    QDomElement codeBase = dom->createElement("code");
    QDomElement nomBase = dom->createElement("nom");
    QDomElement geoBase = dom->createElement("geo");
    QDomElement contrepartie = dom->createElement("contrepartie");
    QDomElement codeContrepartie = dom->createElement("code");
    QDomElement nomContrepartie = dom->createElement("nom");
    QDomElement geoContrepartie = dom->createElement("geo");

    QDomText codeBaseText = dom->createTextNode(evolution->getPaireDevises().getBase().getCode());
    QDomText nomBaseText = dom->createTextNode(evolution->getPaireDevises().getBase().getMonnaie());
    QDomText geoBaseText = dom->createTextNode(evolution->getPaireDevises().getBase().getZoneGeographique());
    QDomText codeContrepartieText = dom->createTextNode(evolution->getPaireDevises().getContrepartie().getCode());
    QDomText nomContrepartieText = dom->createTextNode(evolution->getPaireDevises().getContrepartie().getMonnaie());
    QDomText geoContrepartieText = dom->createTextNode(evolution->getPaireDevises().getContrepartie().getZoneGeographique());
    QDomText surnomText = dom->createTextNode(evolution->getPaireDevises().getSurnom());

    root.appendChild(paire);
    paire.appendChild(base);
    paire.appendChild(contrepartie);
    base.appendChild(codeBase);
    codeBase.appendChild(codeBaseText);
    base.appendChild(nomBase);
    nomBase.appendChild(nomBaseText);
    base.appendChild(geoBase);
    geoBase.appendChild(geoBaseText);
    contrepartie.appendChild(codeContrepartie);
    codeContrepartie.appendChild(codeContrepartieText);
    contrepartie.appendChild(nomContrepartie);
    nomContrepartie.appendChild(nomContrepartieText);
    contrepartie.appendChild(geoContrepartie);
    geoContrepartie.appendChild(geoContrepartieText);

    QDomElement cours = dom->createElement("cours");
    root.appendChild(cours);
    for(EvolutionCours::iterator i=evolution->begin();i!=evolution->end();++i) {
      QDomElement ohlc = dom->createElement("ohlc");
      QDomElement open = dom->createElement("open");
      QDomElement high = dom->createElement("high");
      QDomElement low = dom->createElement("low");
      QDomElement close = dom->createElement("close");
      QDomElement volume = dom->createElement("volume");
      QDomElement date = dom->createElement("date");
      QDomText openText = dom->createTextNode(QString::number(i->getOpen()));
      QDomText highText = dom->createTextNode(QString::number(i->getHigh()));
      QDomText lowText = dom->createTextNode(QString::number(i->getLow()));
      QDomText closeText = dom->createTextNode(QString::number(i->getClose()));
      QDomText volumeText = dom->createTextNode(QString::number(i->getVolume()));
      QDomText dateText = dom->createTextNode(i->getDate().toString("yyyy-MM-dd"));
      cours.appendChild(ohlc);
      ohlc.appendChild(open);
      open.appendChild(openText);
      ohlc.appendChild(high);
      high.appendChild(highText);
      ohlc.appendChild(low);
      low.appendChild(lowText);
      ohlc.appendChild(close);
      close.appendChild(closeText);
      ohlc.appendChild(volume);
      volume.appendChild(volumeText);
      ohlc.appendChild(date);
      date.appendChild(dateText);
    }
    xmlFile->resize(0);
    QTextStream stream(xmlFile);
    stream << dom->toString();
    xmlFile->close();
  }
}
