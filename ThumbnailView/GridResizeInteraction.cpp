/* Copyright (C) 2003-2009 Jesper K. Pedersen <blackie@kde.org>

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
#include "GridResizeInteraction.h"
#include <KGlobal>
#include "ThumbnailWidget.h"
#include "Settings/SettingsData.h"
#include <KSharedConfig>

ThumbnailView::GridResizeInteraction::GridResizeInteraction( ThumbnailWidget* view )
    : _view( view )
{
}

void ThumbnailView::GridResizeInteraction::mousePressEvent( QMouseEvent* event )
{
    _resizing = true;
    _mousePressPos = event->pos();
    _view->setContentsPos( 0, 0 );
    _origSize = QSize( _view->cellWidth(), _view->cellHeight() );
}


void ThumbnailView::GridResizeInteraction::mouseMoveEvent( QMouseEvent* event )
{
    QPoint dist = event->pos() - _mousePressPos;

    Settings::SettingsData::instance()->setThumbSize( qMax( 32, _origSize.width() + dist.x()/5 ) );
    _view->updateCellSize();
}


void ThumbnailView::GridResizeInteraction::mouseReleaseEvent( QMouseEvent* )
{
    _resizing = false;
    _view->repaintScreen();
    KGlobal::config()->sync();
}

bool ThumbnailView::GridResizeInteraction::isResizingGrid()
{
    return _resizing;
}


