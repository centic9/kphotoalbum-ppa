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

#include "DatabaseAddress.h"
#include "Utilities/Util.h"

bool SQLDB::DatabaseAddress::operator==(const DatabaseAddress& other) const
{
    if (isNull() || other.isNull())
        return isNull() == other.isNull();

    if (driverName().toLower() != other.driverName().toLower())
        return false;

    if (isFileBased() != other.isFileBased())
        return false;

    if (isFileBased())
        return Utilities::areSameFile(databaseName(), other.databaseName());

    if (databaseName() != other.databaseName())
        return false;

    if (usesLocalConnection() != other.usesLocalConnection())
        return false;

    if (usesLocalConnection())
        return true;

    return (hostName() == other.hostName() && port() == other.port());
}