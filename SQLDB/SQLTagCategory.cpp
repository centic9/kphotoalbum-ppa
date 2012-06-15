/*
  Copyright (C) 2005-2010 Jesper K. Pedersen <blackie@kde.org>
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

#include "SQLTagCategory.h"
#include "QueryHelper.h"
#include <QList>

SQLDB::SQLTagCategory::SQLTagCategory(QueryHelper* queryHelper,
                                      int categoryId):
    _qh(queryHelper),
    _categoryId(categoryId)
{
}

QString SQLDB::SQLTagCategory::name() const
{
    if (_name.isNull())
        _name = _qh->categoryName(_categoryId);
    return _name;
}

QString SQLDB::SQLTagCategory::iconName() const
{
    return _qh->categoryIcon(_categoryId);
}

void SQLDB::SQLTagCategory::setIconName(const QString& name)
{
    _qh->changeCategoryIcon(_categoryId, name);
    emit changed();
}

DB::Category::ViewType SQLDB::SQLTagCategory::viewType() const
{
    return _qh->categoryViewType(_categoryId);
}

void SQLDB::SQLTagCategory::setViewType(ViewType type)
{
    _qh->changeCategoryViewType(_categoryId, type);
    emit changed();
}

bool SQLDB::SQLTagCategory::doShow() const
{
    return _qh->categoryVisible(_categoryId);
}

void SQLDB::SQLTagCategory::setDoShow(bool b)
{
    _qh->changeCategoryVisible(_categoryId, b);
    emit changed();
}

QStringList SQLDB::SQLTagCategory::items() const
{
    return _qh->tagNamesOfCategory(_categoryId);
}

void SQLDB::SQLTagCategory::setItems(const QStringList& items)
{
    _qh->executeStatement("DELETE FROM tag "
                          "WHERE category_id=? AND name NOT IN (?)",
                          QueryHelper::Bindings() <<
                          _categoryId << QVariant(toVariantList(items)));
    addOrReorderItems(items);
}

void SQLDB::SQLTagCategory::addOrReorderItems(const QStringList& items)
{
    uint position = items.count();
    for (QStringList::const_iterator it = items.begin();
        it != items.end(); ++it ) {
        if (_qh->executeQuery("SELECT COUNT(*) FROM tag "
                              "WHERE category_id=? AND name=?",
                              QueryHelper::Bindings() << _categoryId << *it
                              ).firstItem().toUInt() == 0) {
            _qh->executeStatement("INSERT INTO tag (name, category_id, position) "
                                  "VALUES (?, ?, ?)",
                                  QueryHelper::Bindings() << *it <<
                                  _categoryId << position);
        }
        else {
            _qh->executeStatement("UPDATE tag SET position=? "
                                  "WHERE name=? AND category_id=?",
                                  QueryHelper::Bindings() <<
                                  position << *it << _categoryId);
        }
        --position;
    }
}

void SQLDB::SQLTagCategory::addItem(const QString& item)
{
    _qh->insertTagFirst(_categoryId, item);
}

void SQLDB::SQLTagCategory::removeItem(const QString& item)
{
    _qh->removeTag(_categoryId, item);
    emit itemRemoved(item);
}

void SQLDB::SQLTagCategory::renameItem(const QString& oldValue,
                                       const QString& newValue)
{
    _qh->executeStatement("UPDATE tag SET name=? "
                          "WHERE name=? AND category_id=?",
                          QueryHelper::Bindings() <<
                          newValue << oldValue << _categoryId);
    emit itemRenamed(oldValue, newValue);
}

int SQLDB::SQLTagCategory::thumbnailSize() const
{
    return _qh->executeQuery("SELECT thumbsize FROM category WHERE id=?",
                             QueryHelper::Bindings() <<
                             _categoryId).firstItem().toInt();
}

void SQLDB::SQLTagCategory::setThumbnailSize(int size)
{
    _qh->executeStatement("UPDATE category SET thumbsize=? WHERE id=?",
                          QueryHelper::Bindings() << size << _categoryId);
}

QMap<QString, uint>
SQLDB::SQLTagCategory::classify(const DB::ImageSearchInfo& scope,
                                DB::MediaType typemask) const
{
    QList<DB::RawId>* scopePointer;
    QList<DB::RawId> includedFiles;
    if (scope.isNull())
        scopePointer = 0;
    else {
        includedFiles = _qh->searchMediaItems(scope, typemask);
        scopePointer = &includedFiles;
    }

    return _qh->classify(name(), typemask, scopePointer);
}

#include "SQLTagCategory.moc"