#include "editeurTexte.h"
#include <QDebug>

EditeurTexte::EditeurTexte(QString n, QFile* file): note(n), xmlFile(file) {
  menuBar = new QMenuBar;
  textEdit = new QTextEdit;
  textEdit->setText(note);

  QMenu* fileMenu = new QMenu;
  fileMenu = menuBar->addMenu("File");
  QAction* loadAction = new QAction("Open");
  loadAction->setShortcuts(QKeySequence::Open);
  connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));
  fileMenu->addAction(loadAction);
  QAction* saveAction = new QAction("Save");
  saveAction->setShortcuts(QKeySequence::Save);
  connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
  fileMenu->addAction(saveAction);
  QAction* saveUnderAction = new QAction("Save as");
  saveUnderAction->setShortcuts(QKeySequence::SaveAs);
  connect(saveUnderAction, SIGNAL(triggered()), this, SLOT(saveUnder()));
  fileMenu->addAction(saveUnderAction);
  QAction* saveXmlAction = new QAction("Save XML");
  //saveXmlAction->setShortcuts(QKeySequence::SaveAs);
  connect(saveXmlAction, SIGNAL(triggered()), this, SLOT(saveXml()));
  fileMenu->addAction(saveXmlAction);

  QMenu* editMenu = new QMenu;
  editMenu = menuBar->addMenu("Edit");
  QAction* selectAction = new QAction("Select all");
  selectAction->setShortcuts(QKeySequence::SelectAll);
  connect(selectAction, SIGNAL(triggered()), textEdit, SLOT(selectAll()));
  editMenu->addAction(selectAction);
  QAction* copyAction = new QAction("Copy");
  copyAction->setShortcuts(QKeySequence::Copy);
  connect(copyAction, SIGNAL(triggered()), textEdit, SLOT(copy()));
  editMenu->addAction(copyAction);
  QAction* pasteAction = new QAction("Paste");
  pasteAction->setShortcuts(QKeySequence::Paste);
  connect(pasteAction, SIGNAL(triggered()), textEdit, SLOT(paste()));
  editMenu->addAction(pasteAction);
  QAction* cutAction = new QAction("Cut");
  cutAction->setShortcuts(QKeySequence::Cut);
  connect(cutAction, SIGNAL(triggered()), textEdit, SLOT(cut()));
  editMenu->addAction(cutAction);
  QAction* undoAction = new QAction("Undo");
  undoAction->setShortcuts(QKeySequence::Undo);
  connect(undoAction, SIGNAL(triggered()), textEdit, SLOT(undo()));
  editMenu->addAction(undoAction);
  QAction* redoAction = new QAction("Redo");
  redoAction->setShortcuts(QKeySequence::Redo);
  connect(redoAction, SIGNAL(triggered()), textEdit, SLOT(redo()));
  editMenu->addAction(redoAction);

  QVBoxLayout* editeurVBox = new QVBoxLayout;
  editeurVBox->addWidget(textEdit);

  setLayout(editeurVBox);
  setWindowTitle("Editeur de texte");
  setWindowIcon(QIcon(":/image/logo.png"));
}

void EditeurTexte::load() {
  QFileDialog* fileDialog = new QFileDialog;
  fileDialog->setFileMode(QFileDialog::AnyFile);
  fileDialog->setNameFilter("*.txt");
  fileDialog->show();
  QString fileName;
  if(fileDialog->exec()) {
    fileName = fileDialog->selectedFiles()[0];
  }
  if(fileName!="") {
    file = new QFile(fileName);
    if(!file->open(QIODevice::ReadOnly)) {
      QMessageBox msgBox;
      msgBox.setText("Erreur dans le chargement du fichier.");
      msgBox.exec();
    }
    else {
      QString texte = file->readAll();
      textEdit->setText(texte);
      file->close();
    }
  }
}

void EditeurTexte::saveUnder() {
  QFileDialog* fileDialog = new QFileDialog;
  QString fileName = fileDialog->getSaveFileName();
  file = new QFile(fileName + ".txt");
  if(!file->open(QIODevice::WriteOnly)) {
    QMessageBox msgBox;
    msgBox.setText("Erreur dans la sauvegarde du fichier");
    msgBox.exec();
  }
  else {
    QTextStream stream(file);
    stream << textEdit->toPlainText() << endl;
    file->close();
  }
}

void EditeurTexte::save() {
  if(!file) {
    saveUnder();
  }
  else {
    if(!file->open(QIODevice::WriteOnly)) {
      QMessageBox msgBox;
      msgBox.setText("Erreur dans la sauvegarde du fichier");
      msgBox.exec();
    }
    else {
      QTextStream stream(file);
      stream << textEdit->toPlainText() << endl;
      file->close();
    }
  }
}

void EditeurTexte::saveXml() {
  if(!xmlFile->open(QIODevice::ReadWrite)) {
    QMessageBox msgBox;
    msgBox.setText("Erreur dans la sauvegarde de la simulation.");
    msgBox.exec();
  }
  else {
    QDomDocument* dom = new QDomDocument("xml");
    dom->setContent(xmlFile);
    QDomElement root = dom->documentElement();
    QDomElement note = dom->createElement("note");
    QDomText noteText = dom->createTextNode(textEdit->toPlainText());
    root.appendChild(note);
    note.appendChild(noteText);
    xmlFile->resize(0);
    QTextStream stream(xmlFile);
    stream << dom->toString();
    xmlFile->close();
  }
}

void EditeurTexte::closeEvent(QCloseEvent *bar) {
  delete menuBar;
  bar->accept();
}
