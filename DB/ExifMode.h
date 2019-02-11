/* Copyright (C) 2003-2010 Jesper K. Pedersen <blackie@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef EXIFMODE_H
#define EXIFMODE_H

#include <QFlags>

namespace DB {

enum EXIFMODE_FLAG {
    EXIFMODE_DATE = 0x01,
    EXIFMODE_ORIENTATION = 0x02,
    EXIFMODE_DESCRIPTION = 0x04,
    EXIFMODE_FORCE = 0x08,
    EXIFMODE_USE_IMAGE_DATE_IF_INVALID_EXIF_DATE = 0x10,
    EXIFMODE_DATABASE_UPDATE = 0x20,
    EXIFMODE_INIT = EXIFMODE_DATE | EXIFMODE_ORIENTATION | EXIFMODE_DESCRIPTION | EXIFMODE_USE_IMAGE_DATE_IF_INVALID_EXIF_DATE | EXIFMODE_DATABASE_UPDATE
};

Q_DECLARE_FLAGS(ExifMode, EXIFMODE_FLAG)
}

Q_DECLARE_OPERATORS_FOR_FLAGS(DB::ExifMode)
#endif /* EXIFMODE_H */

// vi:expandtab:tabstop=4 shiftwidth=4:
