#include "transactionViewer.h"
#include "transactionManager.h"
#include "evolutionCours.h"
#include "coursOHLC.h"
#include "tradingException.h"
#include <algorithm>
#include "bougie.h"
#include <QDebug>

TransactionViewer::TransactionViewer(QString m, EvolutionCours* e, QDate& d, QDate& cu, QDate& f, double bk, double b, double c, QFile* file): mode(m), evolutionCours(e), debut(d), current(cu), fin(f), broker(bk), base(b), contrepartie(c), xmlFile(file) {

  iterator = evolutionCours.begin();
  while(iterator->getDate()<current) {
    iterator++;
  }

  initMontant = base+contrepartie / iterator->getOpen();

  QLabel* dateLabel = new QLabel("Date");
  dateEdit = new QLineEdit(iterator->getDate().toString("yyyy-MM-dd"));
  dateEdit->setFixedWidth(100);
  dateEdit->setReadOnly(true);
  QLabel* prixLabel = new QLabel("Prix");
  prixEdit = new QLineEdit(QString::number(iterator->getOpen()));
  prixEdit->setFixedWidth(100);
  prixEdit->setReadOnly(true);
  QLabel* baseLabel = new QLabel("Base");
  baseEdit = new QLineEdit(QString::number(base));
  baseEdit->setFixedWidth(100);
  baseEdit->setReadOnly(true);
  QLabel* contrepartieLabel = new QLabel("Contrepartie");
  contrepartieEdit = new QLineEdit(QString::number(contrepartie));
  contrepartieEdit->setFixedWidth(100);
  contrepartieEdit->setReadOnly(true);
  QLabel* brokerLabel = new QLabel("Broker");
  QLineEdit* brokerEdit = new QLineEdit(QString::number(broker));
  brokerEdit->setFixedWidth(100);
  brokerEdit->setReadOnly(true);

  QChart* chartCours = new QChart;
  chartCours->setTitle("Cours OHLC");
  QChartView* chartViewCours = new QChartView(chartCours);
  QChart* chartVolume = new QChart;
  chartVolume->setTitle("Volume");
  QChartView* chartViewVolume = new QChartView(chartVolume);
  axisCours = new QBarCategoryAxis;
  axisYCours = new QValueAxis;
  axisVolume = new QBarCategoryAxis;
  axisYVolume = new QValueAxis;
  QString base = e->getPaireDevises().getBase().getCode();
  QString contrepartie = e->getPaireDevises().getContrepartie().getCode();
  seriesCours = new QCandlestickSeries;
  seriesCours->setName(base+'/'+contrepartie);
  seriesCours->setIncreasingColor(QColor(Qt::green));
  seriesCours->setDecreasingColor(QColor(Qt::red));
  seriesVolume = new QBarSeries;

  updateCharts();

  chartCours->addSeries(seriesCours);
  chartCours->addAxis(axisCours, Qt::AlignBottom);
  chartCours->addAxis(axisYCours, Qt::AlignLeft);
  seriesCours->attachAxis(axisCours);
  seriesCours->attachAxis(axisYCours);
  seriesVolume->append(setVolume);
  chartVolume->addSeries(seriesVolume);
  chartVolume->addAxis(axisVolume, Qt::AlignBottom);
  chartVolume->addAxis(axisYVolume, Qt::AlignLeft);
  seriesVolume->attachAxis(axisVolume);
  seriesVolume->attachAxis(axisYVolume);

  transactionTableView = new QTableView();
  transactionTableView->setFixedWidth(400);
  transactionModel = new QStandardItemModel(0,3);
  QStringList tableHeaders;
  tableHeaders << "Date" << "Base" << "Contrepartie" << "ROI";
  transactionModel->setHorizontalHeaderLabels(tableHeaders);
  transactionTableView->setModel(transactionModel);

  addTransaction();

  achatEdit = new QLineEdit();
  achatEdit->setFixedWidth(200);
  venteEdit = new QLineEdit();
  venteEdit->setFixedWidth(200);
  QPushButton* achatButton = new QPushButton("Acheter");
  achatButton->setFixedWidth(100);
  QObject::connect(achatButton, SIGNAL(clicked()), this, SLOT(acheter()));
  QPushButton* venteButton = new QPushButton("Vendre");
  venteButton->setFixedWidth(100);
  QObject::connect(venteButton, SIGNAL(clicked()), this, SLOT(vendre()));
  previousButton = new QPushButton("Jour précédent");
  QObject::connect(previousButton, SIGNAL(clicked()), this, SLOT(previousDay()));
  nextButton = new QPushButton("Jour suivant");
  QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextDay()));
  QPushButton* saveButton = new QPushButton("Sauvegarder la simulation");
  QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

  if(mode=="auto") {
    achatButton->setEnabled(false);
    venteButton->setEnabled(false);
  }

  EvolutionCours::iterator it = iterator;
  it--;
  if(it->getDate()<debut || mode!="pas") {
    previousButton->setEnabled(false);
  }
  it = iterator;
  it++;
  if(it->getDate()>=fin || mode=="auto") {
    nextButton->setEnabled(false);
  }

  QVBoxLayout* baseVBox = new QVBoxLayout;
  baseVBox->addWidget(baseLabel);
  baseVBox->addWidget(baseEdit);
  QVBoxLayout* contrepartieVBox = new QVBoxLayout;
  contrepartieVBox->addWidget(contrepartieLabel);
  contrepartieVBox->addWidget(contrepartieEdit);
  QVBoxLayout* prixVBox = new QVBoxLayout;
  prixVBox->addWidget(prixLabel);
  prixVBox->addWidget(prixEdit);
  QVBoxLayout* dateVBox = new QVBoxLayout;
  dateVBox->addWidget(dateLabel);
  dateVBox->addWidget(dateEdit);
  QVBoxLayout* brokerVBox = new QVBoxLayout;
  brokerVBox->addWidget(brokerLabel);
  brokerVBox->addWidget(brokerEdit);
  QHBoxLayout* infoHBox = new QHBoxLayout;
  infoHBox->addLayout(dateVBox);
  infoHBox->addLayout(prixVBox);
  infoHBox->addLayout(baseVBox);
  infoHBox->addLayout(contrepartieVBox);
  infoHBox->addLayout(brokerVBox);
  QHBoxLayout* buyHBox = new QHBoxLayout;
  buyHBox->addWidget(achatEdit);
  buyHBox->addWidget(achatButton);
  QHBoxLayout* sellHBox = new QHBoxLayout;
  sellHBox->addWidget(venteEdit);
  sellHBox->addWidget(venteButton);
  QHBoxLayout* buttonHBox = new QHBoxLayout;
  buttonHBox->addWidget(previousButton);
  buttonHBox->addWidget(nextButton);
  QVBoxLayout* operationVBox = new QVBoxLayout;
  operationVBox->addWidget(transactionTableView);
  operationVBox->addLayout(buyHBox);
  operationVBox->addLayout(sellHBox);
  operationVBox->addLayout(buttonHBox);
  operationVBox->addWidget(saveButton);
  QVBoxLayout* graphVBox = new QVBoxLayout;
  graphVBox->addLayout(infoHBox);
  graphVBox->addWidget(chartViewCours);
  graphVBox->addWidget(chartViewVolume);
  QHBoxLayout* fenetre = new QHBoxLayout;
  fenetre->addLayout(graphVBox);
  fenetre->addLayout(operationVBox);

  setLayout(fenetre);
  setWindowTitle("Transaction de devises");
  setWindowIcon(QIcon(":/image/logo.png"));
}

void TransactionViewer::nextDay(){

  iterator++;

  updateCharts();

  dateEdit->setText(iterator->getDate().toString("yyyy-MM-dd"));
  prixEdit->setText(QString::number(iterator->getOpen()));

  EvolutionCours::iterator it = iterator;
  it++;
  if(it->getDate()>=fin) {
    nextButton->setEnabled(false);
  }
  if(mode=="pas") {
    previousButton->setEnabled(true);
  }

  achatEdit->setText("");
  venteEdit->setText("");
}

void TransactionViewer::previousDay(){

  TransactionManager& m = TransactionManager::getManager();
  QList<Transaction*> l = m.getListTransaction();
  if(!l.isEmpty()){
    Transaction* last = l.last();
    while(last->getDate() == iterator->getDate() && !l.isEmpty()){
      transactionModel->removeRow(l.size()-1);
      l.pop_back();
      if(!l.isEmpty()){
       last = l.last();
      }
    }
  }
  m.setListTransaction(l);

  iterator--;

  axisCours->remove(iterator->getDate().toString("yy.MM.dd"));
  axisVolume->remove(iterator->getDate().toString("yy.MM.dd"));

  dateEdit->setText(iterator->getDate().toString("yyyy-MM-dd"));
  prixEdit->setText(QString::number(iterator->getOpen()));

  EvolutionCours::iterator it = iterator;
  it--;
  if(it->getDate()<debut || mode!="pas") {
    previousButton->setEnabled(false);
  }
  nextButton->setEnabled(true);

  achatEdit->setText("");
  venteEdit->setText("");
}

void TransactionViewer::acheter(){
  double quantite = achatEdit->text().toDouble();
  if (achatEdit->text().isEmpty() || contrepartie<quantite*iterator->getOpen() || base<quantite){
    QMessageBox msgBox;
    msgBox.setText("Valeur incorrecte");
    msgBox.exec();
  }
  else {
    TransactionManager& m = TransactionManager::getManager();
    m.creationTransaction(quantite, iterator->getDate(), iterator->getOpen(), base, contrepartie, broker);
    addTransaction();
  }
  achatEdit->setText("");
  venteEdit->setText("");
}

 void TransactionViewer::vendre(){
  double quantite = - venteEdit->text().toDouble();

  if (venteEdit->text().isEmpty() || base<-1*quantite){
    QMessageBox msgBox;
    msgBox.setText("Valeur incorrecte");
    msgBox.exec();
  }
  else {
    TransactionManager& m = TransactionManager::getManager();
    m.creationTransaction(quantite, iterator->getDate(), iterator->getOpen(), base, contrepartie, broker);
    addTransaction();
  }
  achatEdit->setText("");
  venteEdit->setText("");
}

void TransactionViewer::updateCharts() {
  seriesCours->clear();
  seriesVolume->clear();
  double maxCours=0, maxVolume=0;
  for(EvolutionCours::iterator it=evolutionCours.begin();it<iterator;++it) {
    if(it->getHigh()>=maxCours) {
      maxCours = it->getHigh();
    }
    if(it->getVolume()>=maxVolume) {
      maxVolume = it->getVolume();
    }
  }
  double minCours=maxCours;
  QString base = evolutionCours.getPaireDevises().getBase().getCode();
  QString contrepartie = evolutionCours.getPaireDevises().getContrepartie().getCode();
  setVolume = new QBarSet(base+'/'+contrepartie);
  for(EvolutionCours::iterator it=evolutionCours.begin();it<iterator;it++){
    if(it->getLow()<=minCours && it->getLow()>0) {
      minCours = it->getLow();
    }
    Bougie *set = new Bougie(it->getOpen(), it->getHigh(), it->getLow(), it->getClose(), it);
    seriesCours->append(set);
    QString categories = it->getDate().toString("yy.MM.dd");
    axisCours->append(categories);
    axisVolume->append(categories);
    setVolume->append(it->getVolume());
  }
  seriesVolume->append(setVolume);
  axisYCours->setMin(minCours);
  axisYCours->setMax(maxCours);
  axisYVolume->setMax(maxVolume);
}

void TransactionViewer::addTransaction() {
  TransactionManager& m = TransactionManager::getManager();

  for(int i=0;i<m.getListTransaction().length();i++) {

    QStandardItem* itemdate = new QStandardItem;
    itemdate->setText(m.getListTransaction().at(i)->getDate().toString("yyyy-MM-dd"));
    itemdate->setEditable(false);
    transactionModel->setItem(i, 0, itemdate);
    QStandardItem* itemSB = new QStandardItem;
    itemSB->setText(QString::number(m.getListTransaction().at(i)->getMontantBase()) + "(" + QString::number(m.getListTransaction().at(i)->getDifferenceBase()) + ')');
    itemSB->setEditable(false);
    transactionModel->setItem(i, 1, itemSB);
    QStandardItem* itemSC = new QStandardItem;
    itemSC->setText(QString::number(m.getListTransaction().at(i)->getMontantContrepartie())+ "(" +QString::number(m.getListTransaction().at(i)->getDifferenceContrepartie())+ ')');
    itemSC->setEditable(false);
    transactionModel->setItem(i, 2, itemSC);

    base = m.getListTransaction().at(i)->getMontantBase() + m.getListTransaction().at(i)->getDifferenceBase();
    baseEdit->setText(QString::number(base));
    contrepartie = m.getListTransaction().at(i)->getMontantContrepartie() + m.getListTransaction().at(i)->getDifferenceContrepartie();
    contrepartieEdit->setText(QString::number(contrepartie));

    double montant = base+contrepartie/(iterator->getOpen());
    double roi = ((montant-initMontant)/initMontant)*100;
    m.getListTransaction().at(i)->setRoi(roi);
    QStandardItem* itemROI = new QStandardItem;
    itemROI->setText(QString::number(roi));
    itemROI->setEditable(false);
    transactionModel->setItem(i,3,itemROI);
  }
}

void TransactionViewer::save() {
  if(!xmlFile->open(QIODevice::ReadWrite)) {
    QMessageBox msgBox;
    msgBox.setText("Erreur dans la sauvegarde de la simulation.");
    msgBox.exec();
  }
  else {
    TransactionManager& m = TransactionManager::getManager();
    QDomDocument* dom = new QDomDocument("xml");
    dom->setContent(xmlFile);
    QDomElement dataElement = dom->elementsByTagName("data").at(0).toElement();
    for(int i=0;i<m.getListTransaction().length();i++) {
      QDomElement transactionElement = dom->createElement("Transaction");
      QDomElement dateElement = dom->createElement("dateTransaction");
      QDomElement montantBaseElement = dom->createElement("montantBase");
      QDomElement montantContrepartieElement = dom->createElement("montantContrepartie");
      QDomElement roiElement = dom->createElement("roi");

      Transaction* t = m.getListTransaction().at(i);
      QDomText dateText = dom->createTextNode(t->getDate().toString("yyyy-MM-dd"));
      QDomText montantBaseText = dom->createTextNode(QString::number(t->getMontantBase()));
      QDomText montantContrepartieText = dom->createTextNode(QString::number(t->getMontantContrepartie()));
      QDomText roiText = dom->createTextNode(QString::number(t->getRoi()));

      dateElement.appendChild(dateText);
      transactionElement.appendChild(dateElement);
      montantBaseElement.appendChild(montantBaseText);
      transactionElement.appendChild(montantBaseElement);
      montantContrepartieElement.appendChild(montantContrepartieText);
      transactionElement.appendChild(montantContrepartieElement);
      roiElement.appendChild(roiText);
      transactionElement.appendChild(roiElement);
      dataElement.appendChild(transactionElement);
    }

    xmlFile->resize(0);
    QTextStream stream(xmlFile);
    stream << dom->toString();
    xmlFile->close();
  }
}
