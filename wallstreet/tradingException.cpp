#include "tradingException.h"

TradingException::TradingException(const QString& message): info(message) {
}

QString TradingException::getInfo() const { 
  return info;
}
