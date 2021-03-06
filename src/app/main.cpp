/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include <QIcon>
#include <QProcess>

#ifdef Q_OS_WIN32
#include <QFontDatabase>
namespace Utils {
void loadFonts();
}
#endif
#include "bmwindow.h"
#include <bminterface.h>
#include <util/utils.h>

#include <DLog>
#include <DApplication>
#include <DWidgetUtil>
#include <DApplicationSettings>
#include <DGuiApplicationHelper>

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

#ifdef Q_OS_MAC
static bool switchToRoot(QApplication &app)
{
    QStringList allappargs = app.arguments();
    QProcess whoamip;
    whoamip.start("whoami");
    whoamip.waitForFinished();

    if (QString(whoamip.readAll()).remove("\r").remove("\n") != "root") {
        QString argsconc = "";
        QString argsconcSingleQuote = "";

        for (int i = 1; i < allappargs.size(); ++i) {
            argsconc += QString("\"%1\" ").arg(allappargs.at(i));
            argsconcSingleQuote += QString("'%1' ").arg(allappargs.at(i));
        }

        argsconc += "\"rootcheck=no\"";
        argsconcSingleQuote += "'rootcheck=no'";

        QProcess::startDetached("osascript", QStringList() << "-e" << QString("do shell script \"'%1' %2\" with administrator privileges").arg(app.applicationFilePath()).arg(argsconcSingleQuote));
        return true;

    }

    return false;
}

#endif

int main(int argc, char **argv)
{
    Utils::initResource();
#ifndef Q_OS_WIN
#if defined(DTK_STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif
#endif

    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-boot-maker");
    app.setApplicationVersion(DApplication::buildVersion("20191031"));
//    app.setApplicationVersion(DApplication::buildVersion(VERSION));
//    app.setTheme("light");

#ifdef Q_OS_MAC
    if (switchToRoot(app)) {
        exit(0);
    }
#endif

    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-40} %{line:-4} %{threadid:-8} ] %{message}\n";
    DLogManager::setLogFormat(m_format);
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

#ifndef Q_OS_MAC
    qputenv("DTK_USE_SEMAPHORE_SINGLEINSTANCE", "1");
    if (!DGuiApplicationHelper::instance()->setSingleInstance(app.applicationName(), DGuiApplicationHelper::UserScope)) {
        exit(0);
    }
#endif

#ifdef Q_OS_WIN
    Utils::loadFonts();
    app.setWindowIcon(QIcon(":/theme/light/image/deepin-boot-maker.svg"));
#endif
    app.loadTranslator();
    Utils::loadTranslate();
    app.setApplicationDisplayName(QObject::tr("Boot Maker"));

    qDebug() << "Boot Maker UI started.";

    DApplicationSettings savetheme;
    BMWindow w;
    Dtk::Widget::moveToCenter(&w);
    w.show();
//    w.waitAuth();

    auto ret =  app.exec();
    BMInterface::instance()->stop();
    return ret;
}

#ifdef Q_OS_WIN32
namespace Utils {
void loadFonts()
{
    QFontDatabase database;
    QStringList fontlist = database.families();

    QStringList preferList;
    preferList.append("Microsoft YaHei");
    preferList.append("????????????");
    preferList.append("SimHei");
    preferList.append("??????");

    foreach (QString font, preferList) {
        if (fontlist.contains(font)) {
            QFont newFont = QFont(font);
            qApp->setFont(newFont);
            return;
        }
    }
}
}
#endif

