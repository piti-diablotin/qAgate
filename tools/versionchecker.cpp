#include "versionchecker.h"
#include "base/utils.hpp"
#include <QCheckBox>
#include <QMessageBox>
#include <QRegularExpressionMatch>
#include <QSettings>
#include <QString>

VersionChecker::VersionChecker(QObject *parent) : QObject(parent) {}

void VersionChecker::checkAgate() {
  QString current = QString::fromStdString(utils::agateVersion());
  QString last = QString::fromStdString(utils::agateLatestVersion());
  if (current != last) {
    QSettings settings;
    if (settings.value("updates/agate_current").toString() != current ||
        settings.value("updates/agate_hide").toBool() == false) {
      QMessageBox box;
      box.setCheckBox(new QCheckBox(tr("Do not check again"), &box));
      box.setText(tr("New version of agate is available: %0").arg(last));
      box.setWindowTitle(tr("New Agate release !"));
      box.exec();
      settings.setValue("updates/agate_hide", box.checkBox()->isChecked());
      settings.setValue("updates/agate_current", current);
    }
  }
}

void VersionChecker::checkqAgate() {
  const QString url =
      "https://github.com/piti-diablotin/qAgate/releases/latest";
  _request.setUrl(url);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
  _request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                        QNetworkRequest::ManualRedirectPolicy);
#endif
  QObject::connect(
      &_manager, &QNetworkAccessManager::finished,
      [this](QNetworkReply *reply) {
        QRegularExpression re("\\d+\\.\\d+\\.\\d+");
        QRegularExpressionMatch match = re.match(reply->readAll());
        if (match.hasMatch()) {
          if (match.captured() != QAGATE_VERSION) {
            QSettings settings;
            if (settings.value("updates/qagate_current").toString() !=
                    QAGATE_VERSION ||
                settings.value("updates/qagate_hide").toBool() == false) {
              QMessageBox box;
              box.setCheckBox(new QCheckBox(tr("Do not check again"), &box));
              box.setText(tr("New version of qAgate is available: %0")
                              .arg(match.captured()));
              box.setWindowTitle(tr("New qAgate release !"));
              box.exec();
              settings.setValue("updates/qagate_hide",
                                box.checkBox()->isChecked());
              settings.setValue("updates/qagate_current", QAGATE_VERSION);
            }
          }
        }
      });
  _manager.get(_request);
}
