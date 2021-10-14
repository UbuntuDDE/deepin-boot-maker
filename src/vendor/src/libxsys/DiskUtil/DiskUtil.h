/*
 * Copyright (C) 2015 ~ 2018 Wuhan Deepin Technology Co., Ltd.
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

#pragma once
#include "../Common/Result.h"

#include <QObject>
#include <QString>

namespace XSys {

namespace DiskUtil {
enum PartionFormat {
    PF_FAT32,
    PF_NTFS,
    PF_RAW,
};

bool UmountDisk(const QString &targetDev);
bool UmountPartion(const QString& strPartionName);
XSys::Result EjectDisk(const QString &targetDev);
void detach();
bool Mount(const QString &targetDev);
bool Mount(const QString &targetDev, const QString &path);
QString MountPoint(const QString &targetDev) ;
PartionFormat GetPartitionFormat(const QString &targetDev);
QString GetPartitionDisk(const QString &targetDev);

qint64 GetPartitionFreeSpace(const QString &targetDev);
qint64 GetPartitionTotalSpace(const QString &targetDev);
QString GetPartitionLabel(const QString &targetDev);
bool FormatPartion(const QString& targetDev);
QStringList GetPartionOfDisk(const QString& strDisk);
bool SetActivePartion(const QString& strDisk, const QString& strPartion);
void SetPartionLabel(const QString& strPartion, const QString& strImage);
}

namespace Bootloader {

Result InstallBootloader(const QString &diskDev, const QString &images);
}
}
