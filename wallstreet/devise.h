#pragma once

#include <QString>

class Devise {
  QString code;
  QString monnaie, zone;
  Devise(const QString& c, const QString& m, const QString& z = "");
	~Devise() = default;
  friend class DeviseManager;
public:
  QString getCode() const;
  const QString& getMonnaie() const;
  const QString& getZoneGeographique() const;
};
