/*
  Copyright (C) 2007-2010 Tuomas Suutari <thsuut@utu.fi>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program (see the file COPYING); if not, write to the
  Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
  MA 02110-1301 USA.
*/

#include "DatabaseInitialization.h"
#include "DriverManager.h"
#include "Schema/KPhotoAlbumSchema.h"
#include "QueryHelper.h"
#include "QueryErrors.h"
#include <klocale.h>

namespace
{
static void insertInitialData(SQLDB::Connection& /*conn*/)
    {
#if 0 // When importing a database, we do not want default items.
      // The only thing we can do so far is importing, so lets keep this out for now
      //  4 Sep. 2008 14:44 -- Jesper K. Pedersen
        struct
        {
            const char* name;
            const char* icon;
            bool visible;
            DB::Category::ViewType viewtype;
            int thumbnailSize;
        } entry[] = {
            { "Tokens", "preferences-other",
              true, DB::Category::IconView, 32 },
            { "Events", "editimage",
              true, DB::Category::IconView, 32 },
            { "Places", "network-workgroup",
              true, DB::Category::ListView, 32 },
            { "People", "system-users",
              true, DB::Category::ThumbedListView, 96 },
            { 0, 0, false, DB::Category::ListView, 0 }
        };

        SQLDB::QueryHelper qh(conn);

        for (int i = 0; entry[i].name != 0; ++i)
            qh.insertCategory(QString::fromLatin1(entry[i].name),
                              QString::fromLatin1(entry[i].icon),
                              entry[i].visible,
                              entry[i].viewtype, entry[i].thumbnailSize);
#endif
    }

    static SQLDB::Schema::Identifier
    getDatabaseIdentifier(SQLDB::Connection& conn)
    {
        SQLDB::Schema::string name;
        int versionMajor(0);
        int versionMinor(0);
        int year(0);
        int month(0);
        int day(0);

        try {
            QString x =
                QString::fromLatin1("SELECT value FROM database_metadata "
                                    "WHERE property=");
            name =
                conn.executeQuery(
                    (x + QString::fromLatin1("'name'")).toAscii().constData()
                    ).firstItem().
                toString().toLatin1().constData();
            versionMajor =
                conn.executeQuery(
                    (x + QString::fromLatin1("'version major'"))
                    .toAscii().constData()
                    ).firstItem().toInt();
            versionMinor =
                conn.executeQuery(
                    (x + QString::fromLatin1("'version minor'"))
                    .toAscii().constData()
                    ).firstItem().toInt();
            year = conn.executeQuery(
                (x + QString::fromLatin1("'date year')")).toAscii().constData()
                ).firstItem().toInt();
            month = conn.executeQuery(
                (x + QString::fromLatin1("'date month'")).toAscii().constData()
                ).firstItem().toInt();
            day = conn.executeQuery(
                (x + QString::fromLatin1("'date day'")).toAscii().constData()
                ).firstItem().toInt();
        }
        catch (...) {
        }
        SQLDB::Schema::Identifier id(name, versionMajor, versionMinor);
        if (year != 0 && month != 0 && day != 0)
            id.setDate(year, month, day);
        return id;
    }
}

SQLDB::ConnectionSPtr
SQLDB::initializeKPhotoAlbumDatabase(const DatabaseAddress& address)
{
    DatabaseManager::APtr dbMgr
        (DriverManager::instance().
         getDatabaseManager(address.driverName(),
                            address.connectionParameters()));

    bool databaseCreated = false;

    const QString& dbName = address.databaseName();

    if (!dbMgr->databaseExists(dbName)) {
        dbMgr->createDatabase(dbName, Schema::getKPhotoAlbumSchema());
        databaseCreated = true;
    }

    ConnectionSPtr dbConn(dbMgr->connectToDatabase(dbName));

    if (databaseCreated) {
        insertInitialData(*dbConn);
    }
    else {
        // Test schema version
        if (!Schema::getKPhotoAlbumSchema().identifier().
            isCompatibleWith(getDatabaseIdentifier(*dbConn)))
            throw DatabaseSchemaError(i18n("Database schema is incompatible."));
    }

    return dbConn;
}