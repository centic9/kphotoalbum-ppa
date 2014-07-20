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

#include "CategoryImagePopup.h"
#include <klocale.h>
#include <Utilities/Set.h>
#include <qstringlist.h>
#include "Window.h"
#include "DB/CategoryCollection.h"
#include "Viewer/CategoryImageConfig.h"


void MainWindow::CategoryImagePopup::populate( const QImage& image, const DB::FileName& imageName )
{
    clear();

    _image = image;
    _imageInfo = DB::ImageDB::instance()->info( imageName );

    // add the categories
    QList<DB::CategoryPtr> categories = DB::ImageDB::instance()->categoryCollection()->categories();
    for ( QList<DB::CategoryPtr>::Iterator categoryIt = categories.begin(); categoryIt != categories.end(); ++categoryIt ) {
        if ( !(*categoryIt)->isSpecialCategory() ) {
            bool categoryMenuEnabled = false;
            const QString categoryName = (*categoryIt)->name();
            QMenu* categoryMenu = new QMenu(this);
            categoryMenu->setTitle( (*categoryIt)->text() );

            // add category members
            Utilities::StringSet members = _imageInfo->itemsOfCategory( categoryName );
            for ( Utilities::StringSet::const_iterator memberIt = members.begin();
                    memberIt != members.end(); ++memberIt ) {
                QAction* action = categoryMenu->addAction( *memberIt );
                action->setObjectName( categoryName );
                action->setData( *memberIt );
                categoryMenuEnabled = true;
            }

            categoryMenu->setEnabled(categoryMenuEnabled);
            addMenu( categoryMenu );
        }
    }

    // Add the Category Editor menu item
    QAction* action = addAction( QString::fromLatin1("viewer-show-category-editor"), this, SLOT(makeCategoryImage()) );
    action->setText( i18n("Show Category Editor") );
}

void MainWindow::CategoryImagePopup::slotExecuteService( QAction* action )
{
    QString categoryName = action->objectName();
    QString memberName = action->data().toString();
    if (categoryName.isNull())
        return;
    DB::ImageDB::instance()->categoryCollection()->categoryForName( categoryName )->
        setCategoryImage(categoryName, memberName, _image);
}

void MainWindow::CategoryImagePopup::makeCategoryImage()
{
    CategoryImageConfig::instance()->setCurrentImage( _image, _imageInfo );
    CategoryImageConfig::instance()->show();
}

MainWindow::CategoryImagePopup::CategoryImagePopup( QWidget* parent )
    :QMenu( parent )
{
    setTitle( i18n("Make Category Image") );
    connect( this, SIGNAL(triggered(QAction*)), this, SLOT(slotExecuteService(QAction*)) );
}

#include "CategoryImagePopup.moc"
// vi:expandtab:tabstop=4 shiftwidth=4:
