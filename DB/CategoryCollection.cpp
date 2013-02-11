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
#include "CategoryCollection.h"
#include <QList>

using namespace DB;

void CategoryCollection::itemRenamed( const QString& oldName, const QString& newName )
{
    emit itemRenamed( static_cast<Category*>( const_cast<QObject*>( sender() ) ), oldName, newName );
}

void CategoryCollection::itemRemoved( const QString& item )
{
    emit itemRemoved( static_cast<Category*>( const_cast<QObject*>( sender() ) ), item );
}

/**
   See Category::text() for description why I might want to do this conversion.
*/
QString CategoryCollection::nameForText( const QString& text )
{
     QList<CategoryPtr> list = categories();
     for( QList<CategoryPtr>::Iterator it = list.begin(); it != list.end(); ++it ) {
        if ( (*it)->text() == text )
            return (*it)->name();
    }
    // There used to be an assert(false) here, but I hit it after removing first
    // category in the settings from a localised to the "international" name.
    return text;
}

#include "CategoryCollection.moc"
// vi:expandtab:tabstop=4 shiftwidth=4:
