#include "graph.h"

// C++ headers
#include <iostream>



graph::graph(QWidget *parent) : QGraphicsView(parent)
{
	scene = new QGraphicsScene(this);

	this->setScene(scene);
}



void graph::plots_bg(string xtit, string ytit, vector<double> x, vector<double> y, string title)
{
	// this is needed to set the scene size
	scene->setSceneRect(0, 0, xaxil*1.1, yaxil*1.4);

	
	scene->clear();
	if(x.size()<1) return;
	
	// title centered up top
	QGraphicsSimpleTextItem *Title = new QGraphicsSimpleTextItem(QString(title.c_str()));
	scene->addItem(Title);
	Title->setFont(QFont("Times-Roman", 18));
	Title->moveBy(xaxil / 2 - (double) title.length(), 10); // this should more or less center it
	
	
	// x axis
	QGraphicsLineItem *xaxis  = new QGraphicsLineItem(xorig,  yorig, xorig+xaxil,  yorig);
	xaxis->setPen( QPen(Qt::black, 3));
	scene->addItem(xaxis);
	
	// y axis
	QGraphicsLineItem *yaxis  = new QGraphicsLineItem(xorig, yorig, xorig,  yorig-yaxil);
	yaxis->setPen( QPen(Qt::black, 3));
	scene->addItem(yaxis);
	
	// axis labels
	QGraphicsSimpleTextItem *xlab = new QGraphicsSimpleTextItem(QString(xtit.c_str()));
	QGraphicsSimpleTextItem *ylab = new QGraphicsSimpleTextItem(QString(ytit.c_str()));
	scene->addItem(xlab);
	scene->addItem(ylab);
	xlab->moveBy(xorig+xaxil-xtit.length()*8, yorig + 25);
	ylab->moveBy(xorig-30, yorig-yaxil+50);
	ylab->setRotation(-90);

	// x axis ticks
	for(int a=0; a<nticksx+1; a++)
	{
		QGraphicsLineItem *xtick = new QGraphicsLineItem(xorig + inside + a*DX/nticksx, yorig + 2, xorig + inside + a*DX/nticksx,  yorig-yaxil+inside);
		xtick->setPen( QPen(Qt::blue, 1, Qt::DashDotLine));
		scene->addItem(xtick);
	}

	for(int a=0; a<nticksy+1; a++)
	{
		QGraphicsLineItem *ytick = new QGraphicsLineItem(xorig - 2, yorig - inside - a*DY/nticksy, xorig + xaxil - inside,  yorig - inside - a*DY/nticksy);
		ytick->setPen( QPen(Qt::blue, 1, Qt::DashDotLine));
		scene->addItem(ytick);
	}

	// axis limit
	setDataAxisLimits(x, y);

	// x axis labels
	for(int a=0; a<nticksx; a++)
	{
		char lab[20];
		sprintf(lab, "%4.3f", xmin+ a*dx/nticksx);
		QGraphicsSimpleTextItem *alab = new QGraphicsSimpleTextItem(QString(lab));
		alab->setFont(QFont("Helvetica", 12));
		alab->moveBy(xorig + inside + a*DX/nticksx - 12, yorig + 8);
		scene->addItem(alab);
	}
	
	// y axis labels
	for(int a=0; a<nticksy; a++)
	{
		char lab[20];

		// format depends on number
		if     (fabs(ymin + a*dy/nticksy) < 0.01)
			sprintf(lab, "%5.4f", ymin+ a*dy/nticksy);
		else if(fabs(ymin + a*dy/nticksy) < 1)
			sprintf(lab, "%5.3f", ymin+ a*dy/nticksy);
		else if(fabs(ymin + a*dy/nticksy) < 10)
			sprintf(lab, "%5.2f", ymin+ a*dy/nticksy);
		else if(fabs(ymin + a*dy/nticksy) < 100)
			sprintf(lab, "%5.1f", ymin+ a*dy/nticksy);
		else if(fabs(ymin + a*dy/nticksy) > 1000)
			sprintf(lab, "%2.1e", ymin+ a*dy/nticksy);
		else
			sprintf(lab, "%5.0f", ymin+ a*dy/nticksy);
		
		QGraphicsSimpleTextItem *alab = new QGraphicsSimpleTextItem(QString(lab));
		alab->setFont(QFont("Helvetica", 12));
		// moved by its point size * number of digit (3)
		alab->moveBy(xorig - 36, yorig - inside - a*DY/nticksy - 6);
		scene->addItem(alab);
	}
	
	
}

void graph::plot_graph(vector<double> x, vector<double> y, vector<int> pid)
{
	if(x.size()<2) return;
	
	QGraphicsRectItem    *rect;
	
	for(unsigned int i=0; i<x.size(); i++)
	{
		if(pcolors.find(pid[i]) == pcolors.end())
			cout << " Attention: color not found for: " << pid[i] << endl;
		else
		{
			rect = scene->addRect(xorig + inside, yorig - inside, 4, 4, pcolors[pid[i]]);
			rect->moveBy(DX*(x[i]-xmin)/dx, - DY*(y[i]-ymin)/dy);
		}
	}
}


void graph::setDataAxisLimits(vector<double> x, vector<double> y)
{
	// need to reset limits every time
	xmin = 100000;
	ymin = 100000;
	
	xmax = -100000;
	ymax = -100000;

	// calculating minima and maxima
	for(unsigned int i=0; i<x.size(); i++) if(x[i] < xmin) xmin = x[i];
	for(unsigned int i=0; i<y.size(); i++) if(y[i] < ymin) ymin = y[i];
	
	for(unsigned int i=0; i<x.size(); i++) if(x[i] > xmax) xmax = x[i];
	for(unsigned int i=0; i<y.size(); i++) if(y[i] > ymax) ymax = y[i];
	
	if(ymin == ymax)
	{
		if(ymin < 0) ymin *= 1.0001;
		else ymin *= 0.9999;
		
		if(ymax > 0) ymax *= 1.0001;
		else ymax *= 0.9999;
	}
	if(ymin == 0 && ymax == 0)
	{
		ymin = -1;
		ymax = 1;
	}
	
	dx = (xmax - xmin);
	dy = (ymax - ymin);

}




