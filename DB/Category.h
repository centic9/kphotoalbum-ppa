/* SPDX-FileCopyrightText: 2003-2019 The KPhotoAlbum Development Team

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef CATEGORY_H
#define CATEGORY_H

#include "ImageDate.h"

#include <KIconLoader>
#include <QDate>
#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <QString>

class QImage;
class QPixmap;
namespace DB
{
class CategoryItem;

struct CountWithRange {
    uint count = 0;
    ImageDate range {};
    void add(const ImageDate &date)
    {
        count++;
        range.extendTo(date);
    }
};

/**
   This class stores information about categories (People/Places/Events)
*/
class Category : public QObject, public QSharedData
{
    Q_OBJECT

public:
    enum ViewType { TreeView,
                    ThumbedTreeView,
                    IconView,
                    ThumbedIconView };
    enum CategoryType { PlainCategory,
                        FolderCategory,
                        MediaTypeCategory,
                        TokensCategory };

    virtual QString name() const = 0;
    virtual void setName(const QString &name) = 0;

    virtual void setPositionable(bool) = 0;
    virtual bool positionable() const = 0;

    virtual QString iconName() const = 0;
    virtual void setIconName(const QString &name) = 0;
    virtual QPixmap icon(int size = 22, KIconLoader::States state = KIconLoader::DefaultState) const;

    virtual void setViewType(ViewType type) = 0;
    virtual ViewType viewType() const = 0;

    virtual void setThumbnailSize(int) = 0;
    virtual int thumbnailSize() const = 0;

    virtual void setDoShow(bool b) = 0;
    virtual bool doShow() const = 0;

    virtual void setType(CategoryType t) = 0;
    virtual CategoryType type() const = 0;
    virtual bool isSpecialCategory() const = 0;

    virtual void addOrReorderItems(const QStringList &items) = 0;
    virtual void setItems(const QStringList &items) = 0;
    virtual void removeItem(const QString &item) = 0;
    virtual void renameItem(const QString &oldValue, const QString &newValue) = 0;
    virtual void addItem(const QString &item) = 0;
    virtual QStringList items() const = 0;
    virtual QStringList itemsInclCategories() const;
    QExplicitlySharedDataPointer<CategoryItem> itemsCategories() const;

    QPixmap categoryImage(const QString &category, QString, int width, int height) const;
    void setCategoryImage(const QString &category, QString, const QImage &image);
    QString fileForCategoryImage(const QString &category, QString member) const;
    virtual void setBirthDate(const QString &item, const QDate &birthDate) = 0;
    virtual QDate birthDate(const QString &item) const = 0;

private:
    QString defaultIconName() const;

signals:
    void changed();
    void itemRenamed(const QString &oldName, const QString &newName);
    void itemRemoved(const QString &name);
};

}

#endif /* CATEGORY_H */

// vi:expandtab:tabstop=4 shiftwidth=4:
