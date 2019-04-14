#define _NOEXCEPT noexcept
#include "qplot.h"
#include "base/exception.hpp"
#include <QFileDialog>

const QColor QPlot::qcolor[] = { Qt::black, Qt::red, Qt::green, Qt::blue, Qt::magenta, Qt::cyan, Qt::darkRed, Qt::darkGreen, Qt::darkYellow };

//
QPlot::QPlot(QWidget *parent) : QCustomPlot(parent), Graph(),
  _titleElement(new QCPTextElement(this, QString::fromStdString(_title), QFont("Helvetica", 12, QFont::Bold))),
  _arrowsItems()
{
  this->setAutoAddPlottableToLegend(false);
  this->plotLayout()->insertRow(0);
  this->plotLayout()->addElement(0, 0, _titleElement);
  this->setInteractions(QCP::Interaction::iRangeDrag|QCP::Interaction::iRangeZoom);
  //this->setRangeZoomAxis(this->xAxis,this->yAxis);
  connect(this->xAxis, SIGNAL(rangeChanged(QCPRange)), this->xAxis2, SLOT(setRange(QCPRange)));
  connect(this->yAxis, SIGNAL(rangeChanged(QCPRange)), this->yAxis2, SLOT(setRange(QCPRange)));
  connect(this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressed(QMouseEvent*)));
  connect(this,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(updateCoordinates(QMouseEvent*)));

  this->selectionRect()->setPen(QPen(QColor(50,50,50,200),2,Qt::SolidLine));
  this->selectionRect()->setBrush(QBrush(QColor(125,125,125,50)));
  this->setFocusPolicy(Qt::StrongFocus);
}

//
QPlot::~QPlot() {
  ;
}

void QPlot::plot(const std::vector<double> &x, const std::list<std::vector<double>> &y, const std::list<std::string> &labels, const std::vector<short> &colors){
  this->clean();
  auto label = labels.begin();
  auto yp = y.begin();
  int autocolor = 1;
  bool addedLabel = false;
  for ( unsigned p = 0 ; p < y.size() ; ++p) {
    this->addGraph();
    auto graph = this->graph(p);
    if ( p < colors.size() ) {
      graph->setPen(QPen(qcolor[colors[p] < 8 ? colors[p] : autocolor++]));
    }
    else {
      graph->setPen(QPen(qcolor[autocolor++]));
    }
    if ( autocolor >= 8 ) autocolor = 0;


    if ( p < labels.size() ) {
      if ( !label->empty() ) {
        graph->setName(translateToUnicode(QString::fromStdString(*label)));
        graph->addToLegend();
        addedLabel = true;
      }
      ++label;
    }
    graph->setData(QVector<double>::fromStdVector(x),QVector<double>::fromStdVector(*yp));
    ++yp;
  }
  this->rescaleAxes(true);
  this->xAxis2->setVisible(true);
  this->xAxis2->setTickLabels(false);
  this->yAxis2->setVisible(true);
  this->yAxis2->setTickLabels(false);
  this->xAxis->setLabel(translateToUnicode(QString::fromStdString(_xlabel)));
  this->yAxis->setLabel(translateToUnicode(QString::fromStdString(_ylabel)));
  this->legend->setVisible(addedLabel);
  _titleElement->setText(QString::fromStdString(_title));
  this->addCustom();
  this->replot();
}

void QPlot::plot(const std::vector<double> &x, const std::list<std::vector<double>> &y, const std::list<std::vector<unsigned>> &c, const std::list<std::string> &labels) {
  this->clean();
  this->show();
  auto label = labels.begin();
  (void) label;
  auto yp = y.begin();
  auto cp = c.begin();
  bool addedLabel = false;
  for ( unsigned p = 0 ; p < y.size() ; ++p) {
    for ( unsigned data = 0 ; data < x.size() ; ++data ) {
      this->addGraph();
      unsigned int color = cp->at(data);
      auto graph = this->graph(p*x.size()+data);
      graph->setLineStyle(QCPGraph::LineStyle::lsNone);
      QCPScatterStyle myScatter;
      myScatter.setShape(QCPScatterStyle::ssDisc);
      myScatter.setPen(QPen(QColor(0xFF000000|color)));
      myScatter.setBrush(QColor(0xFF000000|color));
      myScatter.setSize(2);
      graph->setScatterStyle(myScatter);

      graph->addData(x[data],yp->at(data));
    }
    ++yp;
    ++cp;
  }
  this->rescaleAxes(true);
  this->xAxis2->setVisible(true);
  this->xAxis2->setTickLabels(false);
  this->yAxis2->setVisible(true);
  this->yAxis2->setTickLabels(false);
  this->xAxis->setLabel(translateToUnicode(QString::fromStdString(_xlabel)));
  this->yAxis->setLabel(translateToUnicode(QString::fromStdString(_ylabel)));
  this->legend->setVisible(addedLabel);
  _titleElement->setText(QString::fromStdString(_title));
  this->addCustom();
  this->replot();
}

void QPlot::plot(const std::list< std::pair< std::vector<double>,std::vector<double> > > &xy, const std::list<std::string> &labels, const std::vector<short> &colors) {
  this->clean();
  this->show();
  auto label = labels.begin();
  auto xyp = xy.begin();
  int autocolor = 1;
  bool addedLabel = false;
  for ( unsigned p = 0 ; p < xy.size() ; ++p) {
    QCPCurve *newCurve = new QCPCurve(this->xAxis, this->yAxis);
    if ( p < colors.size() ) {
      newCurve->setPen(QPen(qcolor[colors[p] < 8 ? colors[p] : autocolor++]));
    }
    else {
      newCurve->setPen(QPen(qcolor[autocolor++]));
    }
    if ( autocolor >= 8 ) autocolor = 0;

    if ( p < labels.size() ) {
      if ( !label->empty() ) {
        newCurve->setName(translateToUnicode(QString::fromStdString(*label)));
        newCurve->addToLegend();
        addedLabel = true;
      }
      ++label;
    }
    newCurve->setData(QVector<double>::fromStdVector(xyp->first),QVector<double>::fromStdVector(xyp->second));
    ++xyp;
  }
  this->rescaleAxes(true);
  this->xAxis2->setVisible(true);
  this->xAxis2->setTickLabels(false);
  this->yAxis2->setVisible(true);
  this->yAxis2->setTickLabels(false);
  this->xAxis->setLabel(translateToUnicode(QString::fromStdString(_xlabel)));
  this->yAxis->setLabel(translateToUnicode(QString::fromStdString(_ylabel)));
  this->legend->setVisible(addedLabel);
  _titleElement->setText(QString::fromStdString(_title));
  this->addCustom();
  this->replot();
}

void QPlot::save(std::string filename){
  this->savePdf(QString::fromStdString(filename+".pdf"),0,0,QCP::epNoCosmetic);
}

void QPlot::clean(){
  this->clearPlottables();
  for ( auto a : _arrowsItems ) {
    this->removeItem(a);
  }
  _arrowsItems.clear();
  this->xAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
  this->yAxis->setTicker(QSharedPointer<QCPAxisTicker>(new QCPAxisTicker));
}

void QPlot::dump(std::ostream& out, std::string& plotname) const {
  (void) out;
  (void) plotname;
}

void QPlot::resizeEvent(QResizeEvent * event) {
  QCustomPlot::resizeEvent(event);
  //this->resize(event->size());
  this->replot();
}

void QPlot::updateCoordinates(QMouseEvent *event) {
  double x = this->xAxis->pixelToCoord(event->pos().x());
  double y = this->yAxis->pixelToCoord(event->pos().y());
  emit(newCoordinates(x,y));
}

void QPlot::mousePressed(QMouseEvent *event) {
  if ( event->button() == Qt::MouseButton::RightButton ) {
    this->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
  }
  else if ( event->button() == Qt::MouseButton::LeftButton ) {
    this->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
  }
}

void QPlot::autozoom(){
  this->rescaleAxes(true);
  this->replot();
}

void QPlot::save(){
  auto name = QFileDialog::getSaveFileName(this, "Save File", "", "PDF (*.pdf)",nullptr,QFileDialog::DontUseNativeDialog);
  if ( !name.isEmpty() ) {
    if ( !name.endsWith(".pdf",Qt::CaseInsensitive) )
      name.append(".pdf");
    this->savePdf(name,0,0,QCP::epNoCosmetic);
  }
}

void QPlot::addCustom() {
  if ( _xrange.set )
    this->xAxis->setRange(_xrange.min,_xrange.max);

  if ( _yrange.set )
    this->yAxis->setRange(_yrange.min,_yrange.max);

  if ( _xtics.size() > 0 ) {
    QSharedPointer<QCPAxisTickerText> xtickers(new QCPAxisTickerText);
    this->xAxis->setTicker(xtickers);
    for ( auto t : _xtics ) {
      xtickers->addTick(t.position,translateToUnicode(QString::fromStdString(t.label)));
    }
  }

  if ( _ytics.size() > 0 ) {
    QSharedPointer<QCPAxisTickerText> ytickers(new QCPAxisTickerText);
    this->yAxis->setTicker(ytickers);
    for ( auto t : _ytics ) {
      ytickers->addTick(t.position,translateToUnicode(QString::fromStdString(t.label)));
    }
  }

  if ( _arrows.size() > 0 ) {
    for ( auto a : _arrows ) {
      QCPItemLine *arrow = new QCPItemLine(this);
      _arrowsItems.push_back(arrow);
      arrow->start->setCoords(a.x1,a.y1);
      arrow->end->setCoords(a.x2,a.y2);
      arrow->setHead(a.head ? QCPLineEnding::esSpikeArrow : QCPLineEnding::esNone );
    }
  }
}

/*
void QPlot::keyPressEvent(QKeyEvent *event) {
  if ( event->key() == Qt::Key_Escape )
    this->close();
}
*/

QString QPlot::translateToUnicode(QString input) {
  const std::map<QString,QChar> translator = {
    std::pair<QString,QChar>("Alpha"  , QChar(0x91,0x03)),
    std::pair<QString,QChar>("Beta"   , QChar(0x92,0x03)),
    std::pair<QString,QChar>("Gamma"  , QChar(0x93,0x03)),
    std::pair<QString,QChar>("Delta"  , QChar(0x94,0x03)),
    std::pair<QString,QChar>("Epsilon", QChar(0x95,0x03)),
    std::pair<QString,QChar>("Zeta"   , QChar(0x96,0x03)),
    std::pair<QString,QChar>("Theta"  , QChar(0x98,0x03)),
    std::pair<QString,QChar>("Eta"    , QChar(0x97,0x03)),
    std::pair<QString,QChar>("Iota"   , QChar(0x99,0x03)),
    std::pair<QString,QChar>("Kappa"  , QChar(0x9A,0x03)),
    std::pair<QString,QChar>("Lambda" , QChar(0x9B,0x03)),
    std::pair<QString,QChar>("Mu"     , QChar(0x9C,0x03)),
    std::pair<QString,QChar>("Nu"     , QChar(0x9D,0x03)),
    std::pair<QString,QChar>("Xi"     , QChar(0x9E,0x03)),
    std::pair<QString,QChar>("Omicron", QChar(0x9F,0x03)),
    std::pair<QString,QChar>("Pi"     , QChar(0xA0,0x03)),
    std::pair<QString,QChar>("Rho"    , QChar(0xA1,0x03)),
    std::pair<QString,QChar>("Sigma"  , QChar(0xA3,0x03)),
    std::pair<QString,QChar>("Tau"    , QChar(0xA4,0x03)),
    std::pair<QString,QChar>("Upsilon", QChar(0xA5,0x03)),
    std::pair<QString,QChar>("Phi"    , QChar(0xA6,0x03)),
    std::pair<QString,QChar>("Khi"    , QChar(0xA7,0x03)),
    std::pair<QString,QChar>("Psi"    , QChar(0xA8,0x03)),
    std::pair<QString,QChar>("Omega"  , QChar(0xA9,0x03)),
    std::pair<QString,QChar>("alpha"  , QChar(0xB1,0x03)),
    std::pair<QString,QChar>("beta"   , QChar(0xB2,0x03)),
    std::pair<QString,QChar>("gamma"  , QChar(0xB3,0x03)),
    std::pair<QString,QChar>("delta"  , QChar(0xB4,0x03)),
    std::pair<QString,QChar>("epsilon", QChar(0xB5,0x03)),
    std::pair<QString,QChar>("zeta"   , QChar(0xB6,0x03)),
    std::pair<QString,QChar>("theta"  , QChar(0xB8,0x03)),
    std::pair<QString,QChar>("eta"    , QChar(0xB7,0x03)),
    std::pair<QString,QChar>("iota"   , QChar(0xB9,0x03)),
    std::pair<QString,QChar>("kappa"  , QChar(0xBA,0x03)),
    std::pair<QString,QChar>("lambda" , QChar(0xBB,0x03)),
    std::pair<QString,QChar>("mu"     , QChar(0xBC,0x03)),
    std::pair<QString,QChar>("nu"     , QChar(0xBD,0x03)),
    std::pair<QString,QChar>("xi"     , QChar(0xBE,0x03)),
    std::pair<QString,QChar>("omicron", QChar(0xBF,0x03)),
    std::pair<QString,QChar>("pi"     , QChar(0xC0,0x03)),
    std::pair<QString,QChar>("rho"    , QChar(0xC1,0x03)),
    std::pair<QString,QChar>("sigma"  , QChar(0xC3,0x03)),
    std::pair<QString,QChar>("tau"    , QChar(0xC4,0x03)),
    std::pair<QString,QChar>("upsilon", QChar(0xC5,0x03)),
    std::pair<QString,QChar>("phi"    , QChar(0xC6,0x03)),
    std::pair<QString,QChar>("khi"    , QChar(0xC7,0x03)),
    std::pair<QString,QChar>("psi"    , QChar(0xC8,0x03)),
    std::pair<QString,QChar>("omega"  , QChar(0xC9,0x03)),
  };

  QString output = input;
  for ( auto& word : translator ) {
    output.replace(QRegExp(word.first),word.second);
  }
  return output;
}
