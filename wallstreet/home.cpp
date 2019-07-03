#include "home.h"
#include "configuration.h"
#include "evolutionViewer.h"
#include "deviseManager.h"
#include "editeurTexte.h"
#include "transactionViewer.h"
#include "transactionManager.h"

Home::Home() {

  QPixmap icon = QPixmap(":/image/logo.png");
  QLabel* iconLabel = new QLabel;
  iconLabel->setPixmap(icon);
  iconLabel->setAlignment(Qt::AlignCenter);

  QLabel* titre = new QLabel("WallStreet");
  QFont fontTitre;
  fontTitre.setPointSize(100);
  titre->setFont(fontTitre);
  titre->setAlignment(Qt::AlignCenter);

  QPushButton* loadButton = new QPushButton("Charger une simulation");
  connect(loadButton, SIGNAL(clicked()), this, SLOT(load()));
  QPushButton* newButton = new QPushButton("Créer une simulation");
  connect(newButton, SIGNAL(clicked()), this, SLOT(newSimulation()));

  QHBoxLayout* buttonHBox = new QHBoxLayout;
  buttonHBox->addWidget(loadButton);
  buttonHBox->addWidget(newButton);

  QVBoxLayout* fenetre = new QVBoxLayout;
  fenetre->addWidget(iconLabel);
  fenetre->addWidget(titre);
  fenetre->addLayout(buttonHBox);

  setLayout(fenetre);
  setWindowTitle("Wallstreet");
  setWindowIcon(QIcon(":/image/logo.png"));
}

void Home::load() {
  QFileDialog* fileDialog = new QFileDialog;
  fileDialog->setFileMode(QFileDialog::ExistingFile);
  QString fileName = fileDialog->getOpenFileName(this, tr("Open File"), QStandardPaths::displayName(QStandardPaths::AppDataLocation), tr("Fichier XML (*.xml)"));
  if(fileName!="") {
    QFile* xmlFile = new QFile(fileName);
    if(!xmlFile->open(QIODevice::ReadWrite)) {
      QMessageBox msgBox;
      msgBox.setText("Erreur dans le chargement de la simulation.");
      msgBox.exec();
    }
    else {
      DeviseManager& m=DeviseManager::getManager();

      QDomDocument* dom = new QDomDocument("xml");
      dom->setContent(xmlFile);
      QDomElement root = dom->documentElement();
      QDomNodeList codeList = dom->elementsByTagName("code");
      QDomNodeList geoList = dom->elementsByTagName("geo");
      QDomNodeList nomList = dom->elementsByTagName("nom");
      QDomNodeList openList = dom->elementsByTagName("open");
      QDomNodeList highList = dom->elementsByTagName("high");
      QDomNodeList closeList = dom->elementsByTagName("close");
      QDomNodeList lowList = dom->elementsByTagName("low");
      QDomNodeList volumeList = dom->elementsByTagName("volume");
      QDomNodeList dateList = dom->elementsByTagName("date");
      QDomNodeList texte = dom->elementsByTagName("note");
      QDomNodeList debutList = dom->elementsByTagName("debut");
      QDomNodeList finList = dom->elementsByTagName("fin");
      QDomNodeList soldeList = dom->elementsByTagName("solde");
      QDomNodeList brokerList = dom->elementsByTagName("broker");
      QDomNodeList dateTransactionList = dom->elementsByTagName("dateTransaction");
      QDomNodeList montantBaseList = dom->elementsByTagName("montantBase");
      QDomNodeList montantContrepartieList = dom->elementsByTagName("montantContrepartie");
      QDomNodeList roiList = dom->elementsByTagName("roi");


      m.creationDevise(codeList.at(0).toElement().text(), nomList.at(0).toElement().text(), geoList.at(0).toElement().text());
      m.creationDevise(codeList.at(1).toElement().text(), nomList.at(1).toElement().text(), geoList.at(1).toElement().text());
      EvolutionCours* evolution = new EvolutionCours(m.getPaireDevise(codeList.at(0).toElement().text(), codeList.at(1).toElement().text()));
      for(int i=0;i<openList.count();i++) {
        double open = openList.at(i).toElement().text().toDouble();
        double high = highList.at(i).toElement().text().toDouble();
        double close = closeList.at(i).toElement().text().toDouble();
        double low = lowList.at(i).toElement().text().toDouble();
        double volume = volumeList.at(i).toElement().text().toDouble();
        QDate date = QDate::fromString(dateList.at(i).toElement().text(), "yyyy-MM-dd");
        evolution->addCours(open, high, low, close, volume, date);
      }

      QDate debut = QDate::fromString(debutList.at(0).toElement().text(), "yyyy-MM-dd");
      QDate fin = QDate::fromString(finList.at(0).toElement().text(), "yyyy-MM-dd");
      double broker = brokerList.at(0).toElement().text().toDouble();
      double soldeBase = soldeList.at(0).toElement().text().toDouble();
      double soldeContrepartie = soldeList.at(1).toElement().text().toDouble();

      QDate current = debut;
      TransactionManager& t=TransactionManager::getManager(m.getPaireDevise(codeList.at(0).toElement().text(), codeList.at(1).toElement().text()));
      for(int i=0;i<dateTransactionList.count();i++) {
        QDate dateTransaction = QDate::fromString(dateTransactionList.at(i).toElement().text(), "yyyy-MM-dd");
        double montantBase = montantBaseList.at(i).toElement().text().toDouble();
        double montantContrepartie = montantContrepartieList.at(i).toElement().text().toDouble();
        t.creationTransaction(1, dateTransaction, 1, montantBase, montantContrepartie, 0.1);
        current = dateTransaction;
      }

      EvolutionViewer* evolutionViewer = new EvolutionViewer(evolution);
      evolutionViewer->show();

      TransactionViewer* transactionViewer = new TransactionViewer("load", evolution, debut, current, fin, broker, soldeBase, soldeContrepartie);
      transactionViewer->show();

      for(int i=0;i<texte.count();i++) {
        EditeurTexte* editeurTexte = new EditeurTexte(texte.at(i).toElement().text());
        editeurTexte->show();
      }

      xmlFile->close();
      close();
    }
  }
}

void Home::newSimulation() {
  QFileDialog* fileDialog = new QFileDialog;
  QString fileName = fileDialog->getSaveFileName(this, tr("Enregistrer le fichier"), "simulation", tr("Fichier XML (*.xml)"));
  if(fileName!="") {
    QFile* xmlFile = new QFile(fileName);
    if(!xmlFile->open(QIODevice::ReadWrite)) {
      QMessageBox msgBox;
      msgBox.setText("Erreur dans la création de la simulation.");
      msgBox.exec();
    }
    else {
      xmlFile->resize(0);
      QDomDocument* dom = new QDomDocument("xml");
      dom->setContent(xmlFile);
      QDomProcessingInstruction instruction = dom->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
      QDomElement root = dom->createElement("wallstreet");
      dom->appendChild(instruction);
      dom->appendChild(root);
      QTextStream stream(xmlFile);
      stream << dom->toString();
      xmlFile->close();
      Configuration* configuration = new Configuration(xmlFile);
      configuration->show();
      close();
    }
  }
}
