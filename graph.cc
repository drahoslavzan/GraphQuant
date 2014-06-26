//===================================================================
// File:        graph.cc
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


#include <graph.h>


using namespace std;
using namespace cv;
using namespace mu;


static double clamp(double v, double a, double b)
{
	if(v < a) return a;
	if(v > b) return b;
	return v;
}


// ====================================================================
// Graph
// ====================================================================


void Graph::resizeEvent(QResizeEvent *)
{
	canvas = Mat::zeros(height(), width(), CV_8UC3);
}

void Graph::wheelEvent(QWheelEvent *e)
{
	bool pl = false;

	if(e->delta() < 0)
	{
		pl |= setRangeX(rx[0] - 1, rx[1] + 1);
		pl |= setRangeY(ry[0] - 1, ry[1] + 1);
	}
	else
	{
		pl |= setRangeX(rx[0] + 1, rx[1] - 1);
		pl |= setRangeY(ry[0] + 1, ry[1] - 1);
	}

	if(pl)
	{
		emit axisRange(rx[0], rx[1], ry[0], ry[1]);
		replot();
	}
}

void Graph::plot(Mat img, int width, int height, const std::string &expr)
{
	try
	{
		double vl = rv[0];
		double vr = rv[1];

		parser.SetExpr(expr);

		/*
		if(expr.find_first_of("<>=!") != std::string::npos)
		{
			vl = 0;
			vr = 1;
		}
		*/

		miv = 0;
		mav = 0;

		emit(progressShow(true));

		for(int i = 0; i < width; ++i)
		{
			for(int j = 0; j < height; ++j)
			{
				x = i * ((rx[1] - rx[0]) / width) + rx[0];
				y = j * ((ry[1] - ry[0]) / height) + ry[0];

				double v = parser.Eval();

				if(v < miv) miv = v;
				else if(v > mav) mav = v;

				Vec3b &cb = img.at<Vec3b>(height - j - 1, i);

				if(isnan(v))
					cb = clrNan;
				else if(isinf(v))
					cb = (v < 0) ? clrInfN : clrInfP;
				else
				{
					double c = clamp(v, rv[0], rv[1]);

					cb[0] = (c - vl) * (clr[1][0] - clr[0][0]) / (vr - vl) + clr[0][0];
					cb[1] = (c - vl) * (clr[1][1] - clr[0][1]) / (vr - vl) + clr[0][1];
					cb[2] = (c - vl) * (clr[1][2] - clr[0][2]) / (vr - vl) + clr[0][2];
				}
			}

			int w = 100 * double((i + 1) * height) / (width * height) + .5;
			if(!(w % 10))
				emit(progress(w));
		}

		emit(progressShow(false));
		emit(progress(0));

		emit(valueRange(QString::fromUtf8("F(x,y) ∊ [%1, %2]").arg(miv).arg(mav)));
	}
	catch (Parser::exception_type &e)
	{
		emit(progressShow(false));
		emit(progress(0));

		plotAlert.setText(QString::fromUtf8(e.GetMsg().c_str()));
		plotAlert.show();
	}
}

Graph::Graph()
:
	clrNan(255, 0, 0), clrInfN(0, 255, 0), clrInfP(0, 0, 255)
{
	setFrameStyle(QFrame::Box);
	setLineWidth(1);

	// Members
	rx[0] = GRAPH_DEFAULT_RANGE_X[0];
	rx[1] = GRAPH_DEFAULT_RANGE_X[1];
	ry[0] = GRAPH_DEFAULT_RANGE_Y[0];
	ry[1] = GRAPH_DEFAULT_RANGE_Y[1];
	rv[0] = GRAPH_DEFAULT_RANGE_V[0];
	rv[1] = GRAPH_DEFAULT_RANGE_V[1];

	clr[0] = Vec3b(0, 0, 0);
	clr[1] = Vec3b(255, 255, 255);

	canvas = Mat::zeros(height(), width(), CV_8UC3);

	parser.DefineVar("x", &x);
	parser.DefineVar("y", &y);
	parser.DefineConst("pi", (double)M_PI);
	parser.DefineConst("e", (double)M_E);

	plotAlert.setWindowTitle("Parse Error");
	plotAlert.setIcon(QMessageBox::Critical);
}

bool Graph::incX(size_t i, int v)
{
	assert(i < 2);

	if(i == 0)
		return setRangeX(rx[0] - v, rx[1]);
	else
		return setRangeX(rx[0], rx[1] - v);
}

bool Graph::incY(size_t i, int v)
{
	assert(i < 2);

	if(i == 0)
		return setRangeY(ry[0] - v, ry[1]);
	else
		return setRangeY(ry[0], ry[1] - v);
}

bool Graph::incV(size_t i, int v)
{
	assert(i < 2);

	if(i == 0)
		return setRangeV(rv[0] - v, rv[1]);
	else
		return setRangeV(rv[0], rv[1] - v);
}

bool Graph::setRangeX(double left, double right)
{
	if(right - left >= 1)
	{
		rx[0] = left;
		rx[1] = right;

		return true;
	}

	return false;
}

bool Graph::setRangeY(double left, double right)
{
	if(right - left >= 1)
	{
		ry[0] = left;
		ry[1] = right;

		return true;
	}

	return false;
}

bool Graph::setRangeV(double left, double right)
{
	if(right - left >= 1)
	{
		rv[0] = left;
		rv[1] = right;

		return true;
	}

	return false;
}

const double * Graph::rangeX() const
{
	return rx;
}

const double * Graph::rangeY() const
{
	return ry;
}

const double * Graph::rangeV() const
{
	return rv;
}

double Graph::minV() const
{
	return miv;
}

double Graph::maxV() const
{
	return mav;
}

void Graph::replot()
{
	plot(lexpr);
}

void Graph::plot(const QString &expr)
{
	if(expr.isEmpty())
		return;

	int width = canvas.cols;
	int height = canvas.rows;

	plot(canvas, width, height, expr.toStdString());

	// Convert
	Mat dst;
	cvtColor(canvas, dst, CV_BGR2RGB);
	QImage img((uchar *)dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);

	setPixmap(QPixmap::fromImage(img));

	lexpr = expr;
}

void Graph::save(const QString &expr, int width, int height, const QString &file)
{
	Mat out = Mat::zeros(height, width, CV_8UC3);

	plot(out, width, height, expr.toStdString());

	imwrite(file.toStdString(), out);

	lexpr = expr;
}


// ====================================================================
// GraphAxis
// ====================================================================

GraphAxis::GraphAxis(Graph *graph, const QString &title)
:
	xaxis(new AxisLabel("x")), yaxis(new AxisLabel("y")),
	xtitle(new AxisLabel(QString::fromUtf8(" ≤  ") + title
				+ QString::fromUtf8("  ≤ "))),
	gr(graph)
{
	QGridLayout *ltCenter = new QGridLayout;
	QVBoxLayout *ltLeft = new QVBoxLayout;
	QHBoxLayout *ltBottom = new QHBoxLayout;
	QHBoxLayout *ltTop = new QHBoxLayout;

	setLayout(ltCenter);

	llx = new AxisLabel(QString::number(graph->rangeX()[0]));
	lrx = new AxisLabel(QString::number(graph->rangeX()[1]));
	lly = new AxisLabel(QString::number(graph->rangeY()[0]));
	lry = new AxisLabel(QString::number(graph->rangeY()[1]));
	lvl = new AxisLabel(QString::number(graph->rangeV()[0]));
	lvr = new AxisLabel(QString::number(graph->rangeV()[1]));

	ltTop->addWidget(lvl, 1, Qt::AlignRight);
	ltTop->addWidget(xtitle, 0, Qt::AlignCenter);
	ltTop->addWidget(lvr, 1, Qt::AlignLeft);

	lly->setRotation(Qxt::CounterClockwise);
	lry->setRotation(Qxt::CounterClockwise);
	yaxis->setRotation(Qxt::CounterClockwise);

	ltLeft->addWidget(lry, 0, Qt::AlignLeft);
	ltLeft->addWidget(yaxis, 1, Qt::AlignCenter);
	ltLeft->addWidget(lly, 0, Qt::AlignRight);

	ltBottom->addWidget(llx, 0, Qt::AlignLeft);
	ltBottom->addWidget(xaxis, 1, Qt::AlignCenter);
	ltBottom->addWidget(lrx, 0, Qt::AlignRight);

	ltCenter->addLayout(ltTop, 0, 1);
	ltCenter->addLayout(ltLeft, 1, 0);
	ltCenter->addWidget(graph, 1, 1);
	ltCenter->addLayout(ltBottom, 2, 1);

	ltCenter->setRowStretch(1, 1);

	// Connections
	connect(graph, SIGNAL(axisRange(double, double, double, double)),
			this, SLOT(axisRange(double, double, double, double)));
	connect(xaxis, SIGNAL(wheel(AxisLabel *, int)), this, SLOT(wheel(AxisLabel *, int)));
	connect(yaxis, SIGNAL(wheel(AxisLabel *, int)), this, SLOT(wheel(AxisLabel *, int)));
	connect(xtitle, SIGNAL(wheel(AxisLabel *, int)), this, SLOT(wheel(AxisLabel *, int)));
	connect(llx, SIGNAL(wheel(AxisLabel *, int)), this, SLOT(wheel(AxisLabel *, int)));
	connect(lly, SIGNAL(wheel(AxisLabel *, int)), this, SLOT(wheel(AxisLabel *, int)));
	connect(lrx, SIGNAL(wheel(AxisLabel *, int)), this, SLOT(wheel(AxisLabel *, int)));
	connect(lry, SIGNAL(wheel(AxisLabel *, int)), this, SLOT(wheel(AxisLabel *, int)));
	connect(lvl, SIGNAL(wheel(AxisLabel *, int)), this, SLOT(wheel(AxisLabel *, int)));
	connect(lvr, SIGNAL(wheel(AxisLabel *, int)), this, SLOT(wheel(AxisLabel *, int)));
}

void GraphAxis::axisRange(double lx, double rx, double ly, double ry)
{
	llx->setText(QString::number(lx));
	lrx->setText(QString::number(rx));
	lly->setText(QString::number(ly));
	lry->setText(QString::number(ry));
}

void GraphAxis::wheel(AxisLabel *l, int v)
{
	bool b = false;

	if(l == llx)
	{
		b |= gr->incX(0, -v);

		llx->setText(QString::number(gr->rangeX()[0]));
	}
	else if(l == lrx)
	{
		b |= gr->incX(1, v);

		lrx->setText(QString::number(gr->rangeX()[1]));
	}
	else if(l == lly)
	{
		b |= gr->incY(0, -v);

		lly->setText(QString::number(gr->rangeY()[0]));
	}
	else if(l == lry)
	{
		b |= gr->incY(1, v);

		lry->setText(QString::number(gr->rangeY()[1]));
	}
	else if(l == lvl)
	{
		b |= gr->incV(0, -v);

		lvl->setText(QString::number(gr->rangeV()[0]));
	}
	else if(l == lvr)
	{
		b |= gr->incV(1, v);

		lvr->setText(QString::number(gr->rangeV()[1]));
	}
	else if(l == xaxis)
	{
		b |= gr->setRangeX(gr->rangeX()[0] + v, gr->rangeX()[1] - v);

		llx->setText(QString::number(gr->rangeX()[0]));
		lrx->setText(QString::number(gr->rangeX()[1]));
	}
	else if(l == yaxis)
	{
		b |= gr->setRangeY(gr->rangeY()[0] + v, gr->rangeY()[1] - v);

		lly->setText(QString::number(gr->rangeY()[0]));
		lry->setText(QString::number(gr->rangeY()[1]));
	}
	else if(l == xtitle)
	{
		b |= gr->setRangeV(gr->rangeV()[0] + v, gr->rangeV()[1] - v);

		lvl->setText(QString::number(gr->rangeV()[0]));
		lvr->setText(QString::number(gr->rangeV()[1]));
	}
	else
		return;

	if(b)
		gr->replot();
}
