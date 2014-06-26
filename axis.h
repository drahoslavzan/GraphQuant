//===================================================================
// File:        axis.h
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


#ifndef _AXIS_H_
#define _AXIS_H_


#include <QtGui>

#include <qxtlabel.h>


class AxisLabel : public QxtLabel
{
	Q_OBJECT

private:
	void wheelEvent(QWheelEvent *e);
public:
	AxisLabel(QWidget *parent = 0, Qt::WindowFlags flags = 0); 
	AxisLabel(const QString & text, QWidget *parent = 0, Qt::WindowFlags flags = 0); 
signals:
	void wheel(AxisLabel *l, int v);
};


#endif /* _AXIS_H_ */
