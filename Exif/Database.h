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
#ifndef EXIFDATABASE_H
#define EXIFDATABASE_H

#include <QSqlDatabase>
#include <qstring.h>
#include <QList>
#include <qpair.h>
#include <DB/FileName.h>

namespace Exiv2 { class ExifData; }

typedef QPair<int,int> Rational;
typedef QList<Rational> RationalList;

namespace Exif
{

// ============================================================================
// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
// ============================================================================
//
// It is the resposibility of the methods in here to bail out in case database
// support is not available ( !isAvailable() ). This is to simplify client code.
class Database {

public:
    typedef QPair<QString, QString> Camera;
    typedef QList<Camera> CameraList;

    static Database* instance();
    static void deleteInstance();
    static bool isAvailable();

    bool isOpen() const;
    bool isUsable() const;
    bool add( const DB::FileName& fileName );
    void remove( const DB::FileName& fileName );
    DB::FileNameSet filesMatchingQuery( const QString& query );
    CameraList cameras() const;
    void recreate();

protected:
    static QString exifDBFile();
    void openDatabase();
    void populateDatabase();
    static QString connectionName();
    void insert( const DB::FileName& filename, Exiv2::ExifData );

private:
    bool _isOpen;
    bool _doUTF8Conversion;
    Database();
    ~Database();
    void init();
    static Database* _instance;
    QSqlDatabase _db;
};

}

#endif /* EXIFDATABASE_H */

// vi:expandtab:tabstop=4 shiftwidth=4:
