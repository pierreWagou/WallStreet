#pragma once

#include <QDate>

class Transaction
{
private:
    QDate date;
    double quantite;
    double montantBase;
    double differenceBase;
    double montantContrepartie;
    double differenceContrepartie;
    double roi;
    double broker;
    friend class TransactionManager;
    Transaction(double quantite, QDate d, double openOHLC, double beforebase, double beforecontrepartie, double broker );
public:
    double getMontantBase()const{return montantBase;}
    double getMontantContrepartie()const{return montantContrepartie;}
    double getQuantite()const{return  quantite;}
    double getRoi()const{return  roi;}
    QDate getDate()const{return date;}
    void setRoi(double r){roi = r;}
    double getDifferenceBase()const{return differenceBase;}
    double getDifferenceContrepartie()const{return differenceContrepartie;}


};
