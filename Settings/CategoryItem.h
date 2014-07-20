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
#ifndef SETTINGS_CATEGORYITEM_H
#define SETTINGS_CATEGORYITEM_H

#include <QListWidgetItem>
#include <DB/Category.h>
namespace DB { class MemberMap; }

namespace Settings
{
class CategoryItem :public QListWidgetItem
{
public:
    CategoryItem( const QString& category, const QString& text, const QString& icon,
                  DB::Category::ViewType type, int thumbnailSize, QListWidget* parent,
                  bool positionable = false );

    void setLabel( const QString& label );
    void setPositionable( bool positionable );
    void submit( DB::MemberMap* memberMap );
    void removeFromDatabase();

    override QString text() const;
    bool positionable() const;
    override QString icon() const;
    int thumbnailSize() const;
    DB::Category::ViewType viewType() const;

    override void setIcon( const QString& icon );
    void setThumbnailSize( int size );
    void setViewType( DB::Category::ViewType type );

protected:
    void renameCategory(DB::MemberMap* memberMap);

private:
    QString _categoryOrig, _iconOrig;
    bool _positionable;
    bool _positionableOrig;
    QString _category, _icon;
    DB::Category::ViewType _type, _typeOrig;
    int _thumbnailSize, _thumbnailSizeOrig;
};

}


#endif /* SETTINGS_CATEGORYITEM_H */

// vi:expandtab:tabstop=4 shiftwidth=4:
