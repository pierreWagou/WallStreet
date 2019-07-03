#include "paireDevise.h"

PaireDevise::PaireDevise(const Devise& b, const Devise& c, const QString& s): base(&b), contrepartie(&c), surnom(s) {
}

const Devise& PaireDevise::getBase() const {
	return *base;
}

const Devise& PaireDevise::getContrepartie() const {
	return *contrepartie;
}

const QString& PaireDevise::getSurnom() const {
	return surnom;
}

QString PaireDevise::toString() const{
  QString str;
  str=getBase().getCode()+"/"+getContrepartie().getCode();
  if (getSurnom()!="") str+="("+getSurnom()+")";
  return str;
}
