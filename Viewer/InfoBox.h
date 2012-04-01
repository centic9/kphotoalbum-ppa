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

#ifndef INFOBOX_H
#define INFOBOX_H
#include "InfoBoxResizer.h"
#include <QMouseEvent>
#include "ViewerWidget.h"
#include <QTextBrowser>
#include "config-kpa-nepomuk.h"
#ifdef HAVE_NEPOMUK
#   include <nepomuk/kratingwidget.h>
#endif
#include "Settings/SettingsData.h"

class QMenu;
class QToolButton;

namespace Viewer
{
class VisibleOptionsMenu;

class InfoBox :public QTextBrowser {
    Q_OBJECT

public:
    InfoBox( ViewerWidget* parent );
    void setInfo( const QString& text, const QMap<int, QPair<QString,QString> >& linkMap );
    virtual void setSource( const QUrl& which );
    void setSize();

protected slots:
    void jumpToContext();
    void linkHovered(const QString&);

protected:
    OVERRIDE void mouseMoveEvent( QMouseEvent* );
    OVERRIDE void mousePressEvent( QMouseEvent* );
    OVERRIDE void mouseReleaseEvent( QMouseEvent* );
    OVERRIDE void resizeEvent( QResizeEvent* );
    OVERRIDE void contextMenuEvent( QContextMenuEvent* event );
    void updateCursor( const QPoint& pos );
    bool atBlackoutPos( bool left, bool right, bool top, bool bottom, Settings::Position windowPos ) const;
    void showBrowser();
    void possiblyStartResize(const QPoint& pos);
    void hackLinkColorForQt44();
    virtual QVariant loadResource( int type, const QUrl& name );

private:
    QMap<int, QPair<QString,QString> > _linkMap;
    ViewerWidget* _viewer;
    QToolButton* _jumpToContext;
    bool _hoveringOverLink;
    InfoBoxResizer _infoBoxResizer;
    VisibleOptionsMenu* _menu;
#ifdef HAVE_NEPOMUK
    QList<QPixmap> _ratingPixmap;
#endif
};

}


#endif /* INFOBOX_H */

