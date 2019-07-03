#include "devise.h"
#include "tradingException.h"

Devise::Devise(const QString& c, const QString& m, const QString& z): monnaie(m), zone(z) {
	if (c.size() != 3) throw TradingException("code devise incorrect");
	for (unsigned int i = 0; i < 3; i++) {
		if (c[i] < 'A' || c[i] > 'Z') throw TradingException("code devise incorrect");
	}
  code=c;
}

QString Devise::getCode() const {
	return code;
}

const QString& Devise::getMonnaie() const {
	return monnaie;
}

const QString& Devise::getZoneGeographique() const {
	return zone;
}
