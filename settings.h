#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "view.h"
#include "abstracttab.h"

namespace Ui {
  class Settings;
}

class Settings : public QWidget, public AbstractTab
{
  Q_OBJECT

public:
  explicit Settings(QWidget *parent = 0);
  ~Settings();

protected:
  void updateStatus(const View& view);

private:
  Ui::Settings *ui;
  bool _autoUpdate;

public slots:
  void updateDisplaySize(int width, int height);

signals:
  void switchFilling();
  void switchLight();
  void switchPerspective();
  void switchAA();
  void switchAngles();
  void switchTimeInfo();
  void sendCommand(QString command);
private slots:
  void on_perspective_clicked();
  void on_light_clicked();
  void on_filling_clicked();
  void on_antialiasing_clicked();
  void on_angles_clicked();
  void on_ndiv_valueChanged(int arg1);
  void on_format_currentIndexChanged(const QString &arg1);
};

#endif // SETTINGS_H
