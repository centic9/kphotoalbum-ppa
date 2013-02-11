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
#ifndef NUMBEREDBACKUP_H
#define NUMBEREDBACKUP_H
#include <qstringlist.h>

namespace XMLDB {
    class NumberedBackup
    {
    public:
        void makeNumberedBackup();
    protected:
        int getMaxId() const;
        QStringList backupFiles() const;
        int idForFile( const QString& fileName, bool& OK ) const;
        void deleteOldBackupFiles();
    };
}

#endif /* NUMBEREDBACKUP_H */

// vi:expandtab:tabstop=4 shiftwidth=4:
