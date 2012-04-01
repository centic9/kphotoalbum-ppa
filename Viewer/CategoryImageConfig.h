/* Copyright (C) 2003-2006 Jesper K. Pedersen <blackie@kde.org>

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

#ifndef CATEGORYIMAGECONFIG_H
#define CATEGORYIMAGECONFIG_H

#include <kdialog.h>
#include <qimage.h>
#include <QLabel>
#include "DB/ImageInfoPtr.h"

class QComboBox;
class QLabel;

namespace DB
{
    class ImageInfo;
}

class CategoryImageConfig :public KDialog {
    Q_OBJECT

public:
    static CategoryImageConfig* instance();
    void setCurrentImage( const QImage& image, const DB::ImageInfoPtr& info );
    void show();

protected slots:
    void groupChanged();
    void memberChanged();
    void slotSet();

protected:
    QString currentGroup();

private:
    static CategoryImageConfig* _instance;
    CategoryImageConfig();
    QComboBox* _group;
    QStringList _categoryNames;
    QComboBox* _member;
    QLabel* _current;
    QImage _image;
    QLabel* _imageLabel;
    DB::ImageInfoPtr _info;
};


#endif /* CATEGORYIMAGECONFIG_H */

