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
#include "CategoryItem.h"
#include "DB/ImageDB.h"
#include "DB/MemberMap.h"
#include <qdir.h>
#include <klocale.h>
#include <kmessagebox.h>
#include "MainWindow/Window.h"
#include "DB/CategoryCollection.h"

Settings::CategoryItem::CategoryItem( const QString& category, const QString& text, const QString& icon,
                                      DB::Category::ViewType type, int thumbnailSize, QListWidget* parent,
                                      bool positionable )
    :QListWidgetItem( text, parent ),
     _categoryOrig( category ), _iconOrig( icon ),
     _positionable( positionable ), _positionableOrig( positionable ),
     _category( category ), _icon( icon ), _type( type ), _typeOrig( type ),
     _thumbnailSize( thumbnailSize ), _thumbnailSizeOrig( thumbnailSize )
{
}

void Settings::CategoryItem::setLabel( const QString& label )
{
    setText( label );
    _category = label;
}

void Settings::CategoryItem::submit( DB::MemberMap* memberMap )
{
    if ( _categoryOrig.isNull() ) {
        // New Item
        DB::ImageDB::instance()->categoryCollection()->addCategory( _category, _icon, _type, _thumbnailSize, true );
    }
    else {
        DB::CategoryPtr category = DB::ImageDB::instance()->categoryCollection()->categoryForName( _categoryOrig );
        if ( _category != _categoryOrig )
            renameCategory( memberMap );

        if ( _positionable != _positionableOrig )
            category->setPositionable( _positionable );

        if ( _icon != _iconOrig )
            category->setIconName( _icon );

        if ( _type != _typeOrig )
            category->setViewType( _type );

        if ( _thumbnailSize != _thumbnailSizeOrig )
            category->setThumbnailSize( _thumbnailSize );
    }

    _categoryOrig = _category;
    _iconOrig = _icon;
    _typeOrig = _typeOrig;
    _thumbnailSizeOrig = _thumbnailSize;
    _positionableOrig = _positionable;
}

void Settings::CategoryItem::removeFromDatabase()
{
    if ( !_categoryOrig.isNull() ) {
        // the database knows about the item.
        DB::ImageDB::instance()->categoryCollection()->removeCategory( _categoryOrig );
    }
}

QString Settings::CategoryItem::text() const
{
    return _category;
}

bool Settings::CategoryItem::positionable() const
{
    return _positionable;
}

void Settings::CategoryItem::setPositionable(bool positionable)
{
    _positionable = positionable;
}

QString Settings::CategoryItem::icon() const
{
    return _icon;
}

int Settings::CategoryItem::thumbnailSize() const
{
    return _thumbnailSize;
}

DB::Category::ViewType Settings::CategoryItem::viewType() const
{
    return _type;
}

void Settings::CategoryItem::setIcon( const QString& icon )
{
    _icon = icon;
}

void Settings::CategoryItem::setThumbnailSize( int size )
{
    _thumbnailSize = size;
}

void Settings::CategoryItem::setViewType( DB::Category::ViewType type )
{
    _type = type;
}

void Settings::CategoryItem::renameCategory( DB::MemberMap* memberMap )
{
    QString txt = i18n("<p>Due to a shortcoming in KPhotoAlbum, you need to save your database after renaming categories; "
                       "otherwise all the filenames of the category thumbnails will be wrong, and thus lost.</p>"
                       "<p>So either press Cancel now (and it will not be renamed), or press Continue, and as your next "
                       "step save the database.</p>" );


    if ( KMessageBox::warningContinueCancel( ::MainWindow::Window::theMainWindow(), txt ) == KMessageBox::Cancel )
        return;

    QDir dir( QString::fromLatin1("%1/CategoryImages" ).arg( Settings::SettingsData::instance()->imageDirectory() ) );
    const QStringList files = dir.entryList( QStringList() << QString::fromLatin1("%1*" ).arg( _categoryOrig ) );
    for( QStringList::ConstIterator fileNameIt = files.begin(); fileNameIt != files.end(); ++fileNameIt ) {
        QString newName = _category + (*fileNameIt).mid( _categoryOrig.length() );
        dir.rename( *fileNameIt, newName );
    }

    Settings::SettingsData* settings = Settings::SettingsData::instance();
    DB::ImageSearchInfo info = settings->currentLock();
    const bool exclude = settings->lockExcludes();
    info.renameCategory( _categoryOrig, _category );
    settings->setCurrentLock( info, exclude );

    DB::ImageDB::instance()->categoryCollection()->rename(  _categoryOrig, _category );
    memberMap->renameCategory(  _categoryOrig, _category );
    _categoryOrig = _category;
}


// vi:expandtab:tabstop=4 shiftwidth=4:
