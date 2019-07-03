#include "strategieSelection.h"
#include "transactionViewer.h"
#include "strategieFactory.h"
#include "transactionManager.h"
#include <QDebug>

StrategieSelection::StrategieSelection(EvolutionCours* e, QFile* f): evolutionCours(e), xmlFile(f) {

  QLabel* modeLabel = new QLabel("Mode");
  modeLabel->setFixedWidth(50);
  manualButton = new QRadioButton("Mode manuel");
  manualButton->setChecked(true);
  automaticButton = new QRadioButton("Mode automatique");
  QObject::connect(automaticButton, SIGNAL(toggled(bool)), this, SLOT(displayStrategie(bool)));
  pasButton = new QRadioButton("Mode pas à pas");
  QLabel* debutLabel = new QLabel("Début");
  debutLabel->setFixedWidth(50);
  QLabel* finLabel = new QLabel("Fin");
  finLabel->setFixedWidth(25);
  EvolutionCours::iterator it = evolutionCours->begin();
  QDate dateMin = it->getDate().addDays(1);
  it = evolutionCours->end();
  it--;
  QDate dateMax = it->getDate();
  dateDebut = new QDateEdit;
  dateDebut->setMinimumDate(dateMin);
  dateDebut->setMaximumDate(dateMax);
  dateFin = new QDateEdit;
  dateFin->setMinimumDate(dateMin);
  dateFin->setMaximumDate(dateMax);
  dateFin->setDate(dateMax);
  QLabel* baseLabel = new QLabel("Montant de base");
  baseEdit = new QLineEdit;
  baseEdit->setText("0");
  QLabel* brokerLabel = new QLabel("Intermédiaire");
  brokerEdit = new QLineEdit;
  brokerEdit->setText("0.1");
  QLabel* contrepartieLabel = new QLabel("Montant de contrepartie");
  contrepartieEdit = new QLineEdit;
  contrepartieEdit->setText("1000000");
  QPushButton* simulationButton = new QPushButton("Lancer la simulation");
  QObject::connect(simulationButton, SIGNAL(clicked()), this, SLOT(simulate()));

  QVBoxLayout* radioVBox = new QVBoxLayout;
  radioVBox->addWidget(manualButton);
  radioVBox->addWidget(automaticButton);
  radioVBox->addWidget(pasButton);

  modeHBox = new QHBoxLayout;
  modeHBox->addWidget(modeLabel);
  modeHBox->addLayout(radioVBox);

  QHBoxLayout* dateHBox = new QHBoxLayout;
  dateHBox->addWidget(debutLabel);
  dateHBox->addWidget(dateDebut);
  dateHBox->addWidget(finLabel);
  dateHBox->addWidget(dateFin);

  QHBoxLayout* montantHBox = new QHBoxLayout;
  montantHBox->addWidget(baseLabel);
  montantHBox->addWidget(baseEdit);
  montantHBox->addWidget(brokerLabel);
  montantHBox->addWidget(brokerEdit);
  montantHBox->addWidget(contrepartieLabel);
  montantHBox->addWidget(contrepartieEdit);

  QVBoxLayout* fenetre = new QVBoxLayout;
  fenetre->addLayout(modeHBox);
  fenetre->addLayout(dateHBox);
  fenetre->addLayout(montantHBox);
  fenetre->addWidget(simulationButton);

  setLayout(fenetre);
  setWindowTitle("Choix de la stratégie de trading");
  setWindowIcon(QIcon(":/image/logo.png"));
}

void StrategieSelection::displayStrategie(bool checked) {
  if(checked) {
    StrategieFactory* factory = new StrategieFactory();
    strategieCombo = new QComboBox;
    for(int i=0;i<factory->getStrategiName().count();i++) {
      strategieCombo->addItem(factory->getStrategiName().at(i));
    }
    modeHBox->addWidget(strategieCombo);
  }
  else {
    modeHBox->removeWidget(strategieCombo);
    delete strategieCombo;
  }
}

void StrategieSelection::simulate() {

  double broker = brokerEdit->text().toDouble();
  double base = baseEdit->text().toDouble();
  double contrepartie = contrepartieEdit->text().toDouble();
  QDate debut = dateDebut->date();
  QDate fin = dateFin->date();

  if(debut>=fin) {
    QMessageBox msgBox;
    msgBox.setText("Dates incorrectes.");
    msgBox.exec();
  }
  else {
    if(!xmlFile->open(QIODevice::ReadWrite)) {
      QMessageBox msgBox;
      msgBox.setText("Erreur dans la sauvegarde de la simulation.");
      msgBox.exec();
    }
    else {
      QDomDocument* dom = new QDomDocument("xml");
      dom->setContent(xmlFile);

      QDomNodeList baseList = dom->elementsByTagName("base");
      QDomNodeList contrepartieList = dom->elementsByTagName("contrepartie");
      QDomElement baseElement = baseList.at(0).toElement();
      QDomElement contrepartieElement = contrepartieList.at(0).toElement();
      QDomElement root = dom->documentElement();
      QDomElement brokerElement = dom->createElement("broker");
      QDomElement soldeBaseElement = dom->createElement("solde");
      QDomElement soldeContrepartieElement = dom->createElement("solde");
      QDomElement debutElement = dom->createElement("debut");
      QDomElement finElement = dom->createElement("fin");
      QDomElement dataElement = dom->createElement("data");


      QDomText brokerText = dom->createTextNode(QString::number(broker));
      QDomText soldeBaseText = dom->createTextNode(QString::number(base));
      QDomText soldeContrepartieText = dom->createTextNode(QString::number(contrepartie));
      QDomText debutText = dom->createTextNode(debut.toString("yyyy-MM-dd"));
      QDomText finText = dom->createTextNode(fin.toString("yyyy-MM-dd"));

      soldeBaseElement.appendChild(soldeBaseText);
      baseElement.appendChild(soldeBaseElement);
      soldeContrepartieElement.appendChild(soldeContrepartieText);
      contrepartieElement.appendChild(soldeContrepartieElement);
      debutElement.appendChild(debutText);
      root.appendChild(debutElement);
      finElement.appendChild(finText);
      root.appendChild(finElement);
      brokerElement.appendChild(brokerText);
      root.appendChild(brokerElement);
      root.appendChild(dataElement);

      xmlFile->resize(0);
      QTextStream stream(xmlFile);
      stream << dom->toString();
      xmlFile->close();
    }

    TransactionManager& m = TransactionManager::getManager(evolutionCours->getPaireDevises());

    if(manualButton->isChecked()) {
      TransactionViewer* transactionViewer = new TransactionViewer("manuel", evolutionCours, debut, debut, fin, broker, base, contrepartie, xmlFile);
      transactionViewer->show();
    }
    if(pasButton->isChecked()) {
      TransactionViewer* transactionViewer = new TransactionViewer("pas", evolutionCours, debut, debut, fin, broker, base, contrepartie, xmlFile);
      transactionViewer->show();
    }
    if(automaticButton->isChecked()) {
      StrategieFactory* factory = new StrategieFactory();
      Strategie* strategie = factory->factory(strategieCombo->currentText(), debut, fin, base, contrepartie, broker, evolutionCours);
      strategie->execute(debut,fin,base, contrepartie, broker);
      TransactionViewer* transactionViewer = new TransactionViewer("auto", evolutionCours, debut, fin, fin, broker, base, contrepartie, xmlFile);
      transactionViewer->show();
    }
    close();
  }
}
