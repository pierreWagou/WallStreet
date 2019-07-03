#include "transaction.h"

Transaction::Transaction(double q, QDate d, double openOHLC, double b, double c, double bk): date(d), broker(bk) {
  if(q < 0){
    differenceContrepartie = - (q*openOHLC - (broker*0.01)*q*openOHLC);
    differenceBase = q;
  }
  else {
    differenceBase = (q-((broker*0.01)*q));
    differenceContrepartie = -(q*openOHLC);
  }
  montantBase = b;
  montantContrepartie = c;
}
