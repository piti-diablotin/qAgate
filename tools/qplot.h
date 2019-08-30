#ifndef QPLOT_H
#define QPLOT_H

#include <QColor>
#include <QVector>
#include "plot/graph.hpp"
#include "qcustomplot.h"

class QPlot : public QCustomPlot, public Graph {
  Q_OBJECT

  private :
    QCPTextElement *_titleElement;
    QVector<QCPItemLine*> _arrowsItems;

  protected :
    //static const QColor qcolor[] = {{ Qt::black, Qt::red, Qt::green, Qt::blue, Qt::magenta, Qt::cyan, Qt::darkRed, Qt::darkGreen, Qt::darkYellow }};
    static const QColor qcolor[];

    /**
     * Add thing to gnuplot like ranges, and tics
     *
     */
    void addCustom();

    /**
     * Translate greek letters into unicode
     */
    static QString translateToUnicode(QString);

  public :

    /**
     * Constructor.
     */
    QPlot(QWidget *parent);

    /**
     * Destructor.
     */
    virtual ~QPlot();

    /** 
     * Plot several quantities on the screen
     * @param x The x quantity
     * @param y A vector with several y quantites to plot
     * @param labels The labels corresponding to the y quantities.
     */
    virtual void plot(const std::vector<double> &x, const std::list<std::vector<double>> &y, const std::list<std::string> &labels, const std::vector<short> &colors);

    /** 
     * Plot several quantities on the screen
     * @param x The x quantity
     * @param y A vector with several y quantites to plot
     * @param c A vector with colors for each x coordinate
     * @param labels The labels corresponding to the y quantities.
     */
    virtual void plot(const std::vector<double> &x, const std::list<std::vector<double>> &y, const std::list<std::vector<unsigned>> &c, const std::list<std::string> &labels);

    /** 
     * Plot several quantities on the screen
     * @param xy A list of (x,y) pairs to plot
     * @param labels The labels corresponding to the y quantities.
     */
    virtual void plot(const std::list< std::pair< std::vector<double>,std::vector<double> > > &xy, const std::list<std::string> &labels, const std::vector<short> &colors);

    /**
     * Save the graph
     * @param filename Save to filename
     * */
    virtual void save(std::string filename);

    /**
     * Clean everything
     */
    virtual void clean();

    /**
     * Print out the commande to plot
     * @param out ostream for output
     * @param plotname is the filename for the file that would be created when the graph is created
     * Not the script file that would be executed to creat the graph
     */
    virtual void dump(std::ostream& out, std::string& plotname) const;

    virtual void resizeEvent(QResizeEvent * event);

    //virtual void keyPressEvent(QKeyEvent *event);


    public slots :
      virtual void updateCoordinates(QMouseEvent *event);
      virtual void mousePressed(QMouseEvent *event);
      virtual void autozoom();
      virtual void save();

    signals:
      void newCoordinates(double x, double y);
      void beforePlot();

};

#endif  // QPLOT_H
