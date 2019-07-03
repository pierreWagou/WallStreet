#include "transactionManager.h"
#include "tradingException.h"

TransactionManager::~TransactionManager(){
}

TransactionManager::Handler TransactionManager::handler = TransactionManager::Handler();
TransactionManager::Handler::~Handler() {
    delete instance;
}


TransactionManager& TransactionManager::getManager(const PaireDevise&  p){
    if (handler.instance == nullptr)
        handler.instance = new TransactionManager(p);
    return *handler.instance;
}


TransactionManager& TransactionManager::getManager(){
    if (handler.instance == nullptr){
      throw TradingException("Erreur manager de transaction inexistant");
    }
    return *handler.instance;
}

void TransactionManager::libererManager(){
    delete handler.instance;
    handler.instance = nullptr;
}

 void TransactionManager::creationTransaction(double q, QDate d, double openOHLC, double beforebase, double beforecontrepartie, double broker){
    Transaction* t = new Transaction(q, d,openOHLC,beforebase, beforecontrepartie, broker);
    this->list_transaction.push_back(t);

    return;
}
