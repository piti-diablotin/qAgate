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

    /**
     * Translate greek letters into unicode
     */
    static QString translateToUnicode(const QString&);

  protected :

    /**
     * Add thing to gnuplot like ranges, and tics
     *
     */
    void addCustom();

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
    virtual void plot(const std::vector<double> &x, const std::list<std::vector<double>> &y, const std::list<std::string> &labels, const std::vector<unsigned> &colors);

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
    virtual void plot(const std::list< std::pair< std::vector<double>,std::vector<double> > > &xy, const std::list<std::string> &labels, const std::vector<unsigned> &colors);

    /**
     * Save the graph
     * @param filename Save to filename
     * */
    virtual void save(const std::string& filename);

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
    virtual void dump(std::ostream& out, const std::string& plotname) const;

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
