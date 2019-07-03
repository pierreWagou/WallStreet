#pragma once
#include <QtXml>

#include <QString>
#include "devise.h"
#include "paireDevise.h"

class DeviseManager {
	Devise** devises = nullptr;
	unsigned int nbDevises = 0;
	unsigned int nbMaxDevises = 0;
	mutable PaireDevise** paires = nullptr;
	mutable unsigned int nbPaires = 0;
	mutable unsigned int nbMaxPaires = 0;
	DeviseManager(const DeviseManager& m) = delete;
	DeviseManager& operator=(const DeviseManager& m) = delete;
	DeviseManager();
	~DeviseManager();
	struct Handler {
		DeviseManager* instance = nullptr;
		~Handler();
	};
	static Handler handler;
public:
	static DeviseManager& getManager();
	static void libererManager();
  const Devise& creationDevise(const QString& c, const QString& m, const QString& z);
  const Devise& getDevise(const QString& c)const;
  const PaireDevise& getPaireDevise(const QString & c1, const QString & c2) const;
    using iterator = Devise**;
	iterator begin();
	iterator next();
	iterator end();

};
