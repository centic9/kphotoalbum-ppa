/*
  Copyright (C) 2006-2010 Tuomas Suutari <thsuut@utu.fi>

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

#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include "Cursor.h"
#include <qstringlist.h>
#include <qvariant.h>
#include <qpair.h>
#include <qmap.h>
#include <QList>

namespace SQLDB
{
    typedef QList< QPair<QString, QString> > StringStringList;

    class QueryResult
    {
    public:
        QueryResult(std::auto_ptr<QSqlQuery> query): _cursor(query)
        {
        }

        template <class T>
        QList<T> asList() const;

        QVariant firstItem() const;
        RowData getRow(uint n=0) const;
        Cursor cursor() const { return _cursor; }

    private:
        mutable Cursor _cursor;
    };
}

#endif /* QUERYRESULT_H */