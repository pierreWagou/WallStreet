#pragma once

#include "devise.h"

class PaireDevise {
	const Devise* base;
	const Devise* contrepartie;
  QString surnom;
  PaireDevise(const Devise& b, const Devise& c, const QString& s = "");
	~PaireDevise() = default;
  friend class DeviseManager;
public:
	const Devise& getBase() const;
	const Devise& getContrepartie() const;
  const QString& getSurnom() const;
  QString toString() const;
};
