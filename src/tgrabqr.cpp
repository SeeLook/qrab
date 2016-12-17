/***************************************************************************
 *   Copyright (C) 2016 by Tomasz Bojczuk                                  *
 *   seelook@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License	     *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#include "tgrabqr.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
// #include <QtGui/QWindow>
#include <QtGui/QPixmap>
#include <QtCore/QTemporaryDir>
#include <QtCore/QProcess>
#include <QtCore/QDebug>


TgrabQR::TgrabQR(QObject* parent) :
  QObject(parent)
{
}


QString TgrabQR::grab() {
  auto screen = QGuiApplication::primaryScreen();
//   if (const QWindow *window = QGuiApplication::windowHandle())
//     screen = window->screen();
  if (!screen) {
    qDebug() << "No screen to take screenshot";
    return QString();
  }

  return callZBAR(screen->grabWindow(0));
//   if (str.isEmpty()) {
//       m_textEdit->setText(tr("No QR code found!"));
//   } else {
//       m_textEdit->setText(str);
//       parseText(str);
//       qApp->clipboard()->setText(str, QClipboard::Clipboard);
//   }
  
}


//#################################################################################################
//###################              PRIVATE             ############################################
//#################################################################################################
QString TgrabQR::callZBAR(const QPixmap& pix) {
  if (!pix.isNull()) {
    QTemporaryDir tmp;
    QString pixTmpPath = tmp.path() + QLatin1String("/qrab_tmp.png");
    pix.save(pixTmpPath, "PNG");
    QProcess zbar;
    QStringList args;
    args << QStringLiteral("-q") << QStringLiteral("--raw") << pixTmpPath;
    zbar.start(QStringLiteral("zbarimg"), args);
    zbar.waitForFinished();
    return QString::fromUtf8(zbar.readAllStandardOutput().data());
  }
  return QString();
}