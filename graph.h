//===================================================================
// File:        graph.h
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        May 19 2013
// Project:     GraphQuant
//-------------------------------------------------------------------
// Copyright (C) 2013 Drahoslav Zan
//
// This file is part of GraphQuant.
//
// GraphQuant is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GraphQuant is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GraphQuant. If not, see <http://www.gnu.org/licenses/>.
//===================================================================
// vim: set nowrap sw=2 ts=2


#ifndef _GRAPH_H_
#define _GRAPH_H_


#include "axis.h"

#include <QtGui>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <muParser.h>


const double GRAPH_DEFAULT_RANGE_X[2] = { -1, 1 };
const double GRAPH_DEFAULT_RANGE_Y[2] = { -1, 1 };
const double GRAPH_DEFAULT_RANGE_V[2] = { -1, 1 };


class Graph : public QLabel
{
	Q_OBJECT

private:
	double rx[2], ry[2], rv[2];
	double x, y;
	double miv, mav;
	cv::Vec3b clrNan, clrInfN, clrInfP;
	cv::Vec3b clr[2];
	cv::Mat canvas;
	mu::Parser parser;
	QMessageBox plotAlert;
	QString lexpr;
private:
	void resizeEvent(QResizeEvent *e);
	void wheelEvent(QWheelEvent *e);
private:
	void plot(cv::Mat img, int width, int height, const std::string &expr);
public:
	Graph();

	bool incX(size_t i, int v = 1);
	bool incY(size_t i, int v = 1);
	bool incV(size_t i, int v = 1);

	bool setRangeX(double left, double right);
	bool setRangeY(double left, double right);
	bool setRangeV(double left, double right);

	const double * rangeX() const;
	const double * rangeY() const;
	const double * rangeV() const;

	double minV() const;
	double maxV() const;

	void replot();
	void plot(const QString &expr);
	void save(const QString &expr, int width, int height, const QString &file);
signals:
	void progressShow(bool b);
	void progress(int v);
	void valueRange(const QString &s);
	void axisRange(double lx, double rx, double ly, double ry);
};

class GraphAxis : public QWidget
{
	Q_OBJECT

private:
	AxisLabel *llx, *lrx, *lly, *lry;
	AxisLabel *lvl, *lvr;
	AxisLabel *xaxis, *yaxis, *xtitle;
	Graph *gr;
public:
	GraphAxis(Graph *graph, const QString &title);
public slots:
	void axisRange(double lx, double rx, double ly, double ry);
	void wheel(AxisLabel *l, int v);
};


#endif /* _GRAPH_H_ */
