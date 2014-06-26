//===================================================================
// File:        main.cc
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


#include "main.h"

#include <iostream>


using namespace std;


void MainWindow::resizeEvent(QResizeEvent *)
{
	if(!replot)
	{
		replot = true;
		QTimer::singleShot(500, this, SLOT(plot()));
	}
}

MainWindow::MainWindow(const QString &title)
:
	graph(new Graph), expr(new QLineEdit), working(new QProgressBar),
	status(new QStatusBar), replot(true)
{
	setWindowIcon(QIcon(ICON_MAIN));
	setWindowTitle(title);
	setMinimumSize(WINDOW_SZ[0], WINDOW_SZ[1]);

	// Members
	expr->setPlaceholderText("Expression");

	QVBoxLayout *ltCenter = new QVBoxLayout;
	QHBoxLayout *ltExpr = new QHBoxLayout;

	QWidget *wgCenter = new QWidget;
	wgCenter->setLayout(ltCenter);

	setCentralWidget(wgCenter);

	working->setRange(0, 100);
	working->setVisible(false);
	status->addPermanentWidget(working, 1);

	setStatusBar(status);

	ltCenter->addLayout(ltExpr);
	ltCenter->addWidget(new GraphAxis(graph, "F(x,y)"));

	QToolButton *btnSave = new QToolButton;
	btnSave->setIcon(QIcon(ICON_SAVE));

	ltExpr->addWidget(new QLabel("<b>F(x,y)</b> ="));
	ltExpr->addWidget(expr);
	ltExpr->addWidget(btnSave);

	// Connections
	connect(expr, SIGNAL(returnPressed()), this, SLOT(plot()));
	connect(btnSave, SIGNAL(clicked()), this, SLOT(save()));
	connect(graph, SIGNAL(valueRange(const QString &)),
			status, SLOT(showMessage(const QString &)));
	connect(graph, SIGNAL(progress(int)), working, SLOT(setValue(int)));
	connect(graph, SIGNAL(progressShow(bool)), working, SLOT(setVisible(bool)));
};

void MainWindow::plot()
{
	graph->plot(expr->displayText());
	replot = false;
	//qApp->processEvents();
}

void MainWindow::save()
{
	if(expr->displayText().isEmpty())
		return;

	QString fileName = QFileDialog::getSaveFileName(this,
			"Open File", "image.png", "Files (*.*)");

	if(!fileName.isEmpty())
		graph->save(expr->displayText(), 1600, 1200, fileName);
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	MainWindow win(WINDOW_TITLE);

	win.show();

	return app.exec();
}

