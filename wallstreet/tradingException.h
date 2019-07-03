#pragma once

#include <QString>

class TradingException {
public:
  TradingException(const QString& message);
  QString getInfo() const;
private:
  QString info;
};
