#pragma once

#include "paireDevise.h"
#include "transaction.h"
#include <QList>


class TransactionManager {
private:
  const PaireDevise& pairepointee;
  QList<Transaction*> list_transaction;
  TransactionManager (const TransactionManager& t) = delete;
  TransactionManager& operator=(const TransactionManager& m) = delete;
  TransactionManager (const PaireDevise& p) : pairepointee(p){}
  struct Handler {
    TransactionManager* instance = nullptr;
    ~Handler();
  };
  static Handler handler;
  ~TransactionManager();
public:
  const PaireDevise& getPairePointee(){return this->pairepointee;}
  static TransactionManager& getManager(const PaireDevise& p);
  static TransactionManager& getManager();
  static void libererManager();
  void creationTransaction(double q, QDate d, double openOHLC, double beforebase, double beforecontrepartie, double broker);
  QList<Transaction*> getListTransaction() {return list_transaction;}
  void setListTransaction(QList<Transaction*> l) {list_transaction = l;}
};
