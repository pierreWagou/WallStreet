#pragma once

#include <QDate>
#include <QtCharts>

class CoursOHLC {
	double open = 0;
	double high = 0;
	double low = 0;
	double close = 0;
	double volume = 0;
  QDate date;
public:
	CoursOHLC() {}
  CoursOHLC(double o, double h, double l, double c, double v, const QDate& d);
	double getOpen() const;
	double getHigh() const;
	double getLow() const;
	double getClose() const;
	double getVolume() const;
	void setCours(double o, double h, double l, double c, double v=0);
  QDate getDate() const;
  void setDate(const QDate& d);
	QString getForme();
};
