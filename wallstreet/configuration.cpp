#include "configuration.h"
#include "deviseManager.h"
#include <QDebug>

Configuration::Configuration(QFile* file): xmlFile(file) {
  DeviseManager& m=DeviseManager::getManager();

  initCreationDialog();

  QLabel* baseLabel =  new QLabel("Devise de Base");
  QLabel* contrepartieLabel = new QLabel("Devise de contrepartie");
  baseCombo = new QComboBox;
  contrepartieCombo = new QComboBox;

  QFile* deviseXml = new QFile("devises.xml");
  if(!deviseXml->open(QIODevice::ReadWrite)){
    QMessageBox msgBox;
    msgBox.setText("Erreur dans l'importation des devises");
    msgBox.exec();
  }
  else {
    QDomDocument* dom = new QDomDocument("xml");
    dom->setContent(deviseXml);
    if(dom->toString()=="") {
      QDomProcessingInstruction instruction = dom->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
      QDomElement root = dom->createElement("devises");
      dom->appendChild(instruction);
      dom->appendChild(root);
      deviseXml->resize(0);
      QTextStream stream(deviseXml);
      stream << dom->toString();
    }
    else {
      QDomElement element = dom->documentElement().firstChildElement();
      while(!element.isNull()) {
        QDomElement code = element.firstChildElement();
        QDomElement geo = code.nextSiblingElement();
        QDomElement nom = geo.nextSiblingElement();
        element = element.nextSiblingElement();
        m.creationDevise(code.text(), nom.text(), geo.text());
      }
    }
    deviseXml->close();
  }

  for(DeviseManager::iterator i=m.begin();i!=m.end();++i) {
    baseCombo->addItem((*i)->getCode());
    contrepartieCombo->addItem((*i)->getCode());
  }

  QPushButton* createButton = new QPushButton("Créer devise");
  QObject::connect(createButton, SIGNAL(clicked()), creationDialog, SLOT(show()));
  QPushButton* nextButton = new QPushButton("Z'est barti !");
  QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(go()));

  QVBoxLayout* labelVBox = new QVBoxLayout;
  labelVBox->addWidget(baseLabel);
  labelVBox->addWidget(contrepartieLabel);

  QVBoxLayout* comboVBox = new QVBoxLayout;
  comboVBox->addWidget(baseCombo);
  comboVBox->addWidget(contrepartieCombo);

  QHBoxLayout* deviseHBox = new QHBoxLayout;
  deviseHBox->addLayout(labelVBox);
  deviseHBox->addLayout(comboVBox);

  QHBoxLayout* buttonHBox = new QHBoxLayout;
  buttonHBox->addWidget(createButton);
  buttonHBox->addWidget(nextButton);

  QVBoxLayout* fenetre = new QVBoxLayout;
  fenetre->addLayout(deviseHBox);
  fenetre->addLayout(buttonHBox);

  setLayout(fenetre);
  setWindowTitle("Selection des devises");
  setWindowIcon(QIcon(":/image/logo.png"));
}

void Configuration::initCreationDialog() {
  creationDialog = new QDialog;

  QLabel* nomLabel = new QLabel("Nom de la Devise");
  QLabel* geoLabel = new QLabel("Zone Géopgraphique");
  QLabel* codeLabel = new QLabel("Code de la Devise");

  nomEdit = new QLineEdit();
  geoEdit = new QLineEdit();
  codeEdit = new QLineEdit();

  QPushButton* createButton = new QPushButton("Créer");
  QObject::connect(createButton, SIGNAL(clicked()), this, SLOT(create()));

  QVBoxLayout* labelVBox = new QVBoxLayout;
  labelVBox->addWidget(nomLabel);
  labelVBox->addWidget(geoLabel);
  labelVBox->addWidget(codeLabel);

  QVBoxLayout* editVBox = new QVBoxLayout;
  editVBox->addWidget(nomEdit);
  editVBox->addWidget(geoEdit);
  editVBox->addWidget(codeEdit);

  QHBoxLayout* deviseHBox = new QHBoxLayout;
  deviseHBox->addLayout(labelVBox);
  deviseHBox->addLayout(editVBox);

  QVBoxLayout* fenetre = new QVBoxLayout;
  fenetre->addLayout(deviseHBox);
  fenetre->addWidget(createButton);

  creationDialog->setLayout(fenetre);
  creationDialog->setWindowTitle("Création d'une devise");
}

void Configuration::create() {
  DeviseManager& m=DeviseManager::getManager();
  QString nom = nomEdit->text();
  QString geo = geoEdit->text();
  QString code = codeEdit->text();
  bool error = false;
  if(code.size()!=3) {
    error = true;
  }
  for(int i=0;i<code.size();i++) {
    if(code[i] < 'A' || code[i] > 'Z') {
      error = true;
    }
  }
  if(error) {
    QMessageBox msgBox;
    msgBox.setText("Un code de devise doit être composé de exactement 3 lettres majuscules.");
    msgBox.exec();
  }
  else {
    m.creationDevise(code, nom, geo);
    baseCombo->addItem(code);
    contrepartieCombo->addItem(code);
    nomEdit->setText("");
    geoEdit->setText("");
    codeEdit->setText("");
    creationDialog->close();
    QFile* deviseXml = new QFile("devises.xml");
    if(!deviseXml->open(QIODevice::ReadWrite)) {
      QMessageBox msgBox;
      msgBox.setText("Erreur dans la sauvegarde du fichier");
      msgBox.exec();
    }
    QDomDocument *dom = new QDomDocument("xml");
    dom->setContent(deviseXml);

    QDomElement root = dom->documentElement();
    QDomElement devise = dom->createElement("devise");
    QDomElement codeElement = dom->createElement("code");
    QDomElement geoElement = dom->createElement("geo");
    QDomElement nomElement = dom->createElement("nom");
    QDomText codeText = dom->createTextNode(code);
    QDomText geoText = dom->createTextNode(geo);
    QDomText nomText = dom->createTextNode(nom);

    codeElement.appendChild(codeText);
    geoElement.appendChild(geoText);
    nomElement.appendChild(nomText);
    devise.appendChild(codeElement);
    devise.appendChild(geoElement);
    devise.appendChild(nomElement);
    root.appendChild(devise);

    deviseXml->resize(0);
    QTextStream stream(deviseXml);
    stream << dom->toString();
    deviseXml->close();
  }
}

void Configuration::go() {
  DeviseManager& m=DeviseManager::getManager();
  QString base = baseCombo->currentText();
  QString contrepartie = contrepartieCombo->currentText();
  if(contrepartie==base) {
    QMessageBox msgBox;
    msgBox.setText("Les 2 devises choisies sont identiques. Veuillez choisir 2 devises différentes.");
    msgBox.exec();
  }
  else {
    EvolutionCours* evolution = new EvolutionCours(m.getPaireDevise(base, contrepartie));
    EvolutionViewer* evolutionViewer = new EvolutionViewer(evolution, xmlFile);
    evolutionViewer->show();
    close();
  }
}
