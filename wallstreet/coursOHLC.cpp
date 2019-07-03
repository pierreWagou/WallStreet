#include "coursOHLC.h"
#include "tradingException.h"

void CoursOHLC::setCours(double o, double h, double l, double c, double v) {
	if (o < 0 || h < 0 || l < 0 || c<0 || l>h) throw TradingException("cours OHLC incorrect");
		open = o;
		high = h;
		low = l;
		close = c;
		volume = v;
}

CoursOHLC::CoursOHLC(double o, double h, double l, double c, double v, const QDate& d):date(d) {
  if (o < 0 || h < 0 || l < 0 || c<0 || l>h) throw TradingException("cours OHLC incorrect");
    open = o;
		high = h;
		low = l;
		close = c;
		volume = v;
}

double CoursOHLC::getOpen() const {
	return open;
}

double CoursOHLC::getHigh() const {
	return high;
}

double CoursOHLC::getLow() const {
	return low;
}

double CoursOHLC::getClose() const {
	return close;
}

double CoursOHLC::getVolume() const {
	return volume;
}

QDate CoursOHLC::getDate() const {
	return date;
}

void CoursOHLC::setDate(const QDate& d) {
	date=d;
}

QString CoursOHLC::getForme() {
  if(fabs(open-close<high-open+low-close)) {
    if(fabs(open-close)==0) {
      return "Doji";
    }
    if(high==open && close!=low) {
      return "Marteau";
    }
    if(high!=close && open==low) {
      return "Etoile filante";
    }
    if(high==close && open!=low) {
      return "Pendu";
    }
    return "Toupie";
  }
  if(high==close && low==open) {
    return "Marubozu";
  }
  if(high==open && low==close) {
    return "Morubozu";
  }
  if(high==close && low!=open) {
    return "O Yin Bozu";
  }
  if(high==open && low!=close) {
    return "O Yang Bozu";
  }
  if(high!=close && low==open) {
    return "C Yang Bozu";
  }
  if(high!=open && low==close) {
    return "C Yin Bozu";
  }
  return "Bougie";
}
