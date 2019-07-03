#include "deviseManager.h"
#include "tradingException.h"

DeviseManager::Handler DeviseManager::handler = DeviseManager::Handler();

DeviseManager::Handler::~Handler() {
	delete instance;
}

DeviseManager::DeviseManager() {
}

DeviseManager& DeviseManager::getManager() {
	if (handler.instance == nullptr)
		handler.instance = new DeviseManager;
	return *handler.instance;
}

void DeviseManager::libererManager() {
	delete handler.instance;
	handler.instance = nullptr;
}

const Devise& DeviseManager::creationDevise(const QString& c, const QString& m, const QString& z) {
	if (nbDevises == nbMaxDevises) {
		Devise** newtab = new Devise*[nbMaxDevises + 10];
		for (unsigned int i = 0; i < nbDevises; i++)
			newtab[i] = devises[i];
		Devise** old = devises;
		nbMaxDevises += 10;
		devises = newtab;
		delete[] old;
	}
	devises[nbDevises] = new Devise(c, m, z);



	return *devises[nbDevises++];
}

const Devise& DeviseManager::getDevise(const QString& c)const {
	for (unsigned int i = 0; i < nbDevises; i++)
		if (devises[i]->getCode() == c)
			return *devises[i];
	throw TradingException("devise inexistante");
}

const PaireDevise& DeviseManager::getPaireDevise(const QString & c1, const QString & c2) const {
	const Devise& d1 = getDevise(c1);
	const Devise& d2 = getDevise(c2);
	for (unsigned int i = 0; i < nbPaires; i++)
		if (paires[i]->getBase().getCode() == c1 &&
			paires[i]->getContrepartie().getCode() == c2)
			return *paires[i];
	if (nbPaires == nbMaxPaires) {
		PaireDevise** newtab = new PaireDevise*[nbMaxPaires + 10];
		for (unsigned int i = 0; i < nbPaires; i++) newtab[i] = paires[i];
		PaireDevise** old = paires;
		nbMaxPaires += 10;
		paires = newtab;
		delete[] old;
	}
	paires[nbPaires] = new PaireDevise(d1, d2);
	return *paires[nbPaires++];
}

DeviseManager::~DeviseManager() {
	for (unsigned int i = 0; i < nbPaires; i++) delete paires[i];
	for (unsigned int i = 0; i < nbDevises; i++) delete devises[i];
	delete[] paires; delete[] devises;
}

DeviseManager::iterator DeviseManager::begin() {
    return iterator(devises);
}

DeviseManager::iterator DeviseManager::next() {
	return iterator(++devises);
}

DeviseManager::iterator DeviseManager::end() {
    return iterator(devises+nbDevises);
}
