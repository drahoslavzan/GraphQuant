//===================================================================
// File:        main.h
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        May 19 2013
// Comments:    MKP problem solver.
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


#ifndef _MAIN_H_
#define _MAIN_H_


#include <graph.h>

#include <cstdlib>

#include <QtGui>


const char * const WINDOW_TITLE = "GraphQuant";
const size_t WINDOW_SZ[2] = { 640, 480 };

//const char * const ICON_GRAPH_PLOT = ":/images/plot";
const char * const ICON_SAVE = ":/images/save";
const char * const ICON_MAIN = ":/images/icon";


class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Graph *graph;
	QLineEdit *expr;
	QProgressBar *working;
	QStatusBar *status;
	bool replot;
private:
	void resizeEvent(QResizeEvent *);
public:
	MainWindow(const QString &title);
public slots:
	void plot();
	void save();
};


#endif /* _MAIN_H_ */
