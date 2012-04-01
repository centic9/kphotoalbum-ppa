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

#include <QListWidget>
#include <QGroupBox>
#include "Exif/ReReadDialog.h"
#include <klocale.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <QVBoxLayout>
#include <kmessagebox.h>
#include "DB/ImageDB.h"
#include "Exif/Database.h"


Exif::ReReadDialog::ReReadDialog( QWidget* parent )
    :KDialog( parent )
{
    setWindowTitle( i18n("Read EXIF info from files") );

    QWidget* top = new QWidget;
    setMainWidget( top );

    QVBoxLayout* lay1 = new QVBoxLayout( top );

    _exifDB = new QCheckBox( i18n( "Update EXIF search database" ), top );
    lay1->addWidget( _exifDB );
    if ( !Exif::Database::instance()->isUsable() ) {
        _exifDB->hide();
    }

    _date = new QCheckBox( i18n( "Update image date" ), top );
    lay1->addWidget( _date );

    _force_date = new QCheckBox( i18n( "Use modification date if EXIF not found" ), top );
    lay1->addWidget( _force_date );

    _orientation = new QCheckBox( i18n( "Update image orientation from EXIF information" ), top );
    lay1->addWidget( _orientation );

    _description = new QCheckBox( i18n( "Update image description from EXIF information" ), top );
    lay1->addWidget( _description );

    QGroupBox* box = new QGroupBox( i18n("Affected Files") );
    lay1->addWidget( box );

    QHBoxLayout* boxLayout = new QHBoxLayout( box );
    _fileList = new QListWidget;
    _fileList->setSelectionMode( QAbstractItemView::NoSelection );
    boxLayout->addWidget( _fileList );

    connect( this, SIGNAL( okClicked() ), this, SLOT( readInfo() ) );
    connect( _date, SIGNAL( toggled( bool ) ), _force_date, SLOT( setEnabled( bool ) ) );
    connect( _date, SIGNAL( toggled( bool ) ), this, SLOT( warnAboutDates( bool ) ) );
}

int Exif::ReReadDialog::exec( const QStringList& list )
{
    _exifDB->setChecked( true);
    _date->setChecked( false );
    _force_date->setChecked( true );
    _force_date->setEnabled( false );
    _orientation->setChecked( false );
    _description->setChecked( false );
    _list = list;
    _fileList->clear();
    _fileList->addItems( list );

    return KDialog::exec();
}

void Exif::ReReadDialog::readInfo()
{
    DB::ExifMode mode = DB::EXIFMODE_FORCE;

    if ( _exifDB->isChecked() )
        mode |= DB::EXIFMODE_DATABASE_UPDATE;

    if ( _date->isChecked() )
            mode |= DB::EXIFMODE_DATE;
    if ( _force_date->isChecked() )
             mode |= DB::EXIFMODE_USE_IMAGE_DATE_IF_INVALID_EXIF_DATE;
    if ( _orientation->isChecked() )
            mode |= DB::EXIFMODE_ORIENTATION;
    if ( _description->isChecked() )
            mode |= DB::EXIFMODE_DESCRIPTION;

    accept();
    DB::ImageDB::instance()->slotReread(_list, mode);
}

void Exif::ReReadDialog::warnAboutDates( bool b )
{
    if ( !b )
        return;

    int ret = KMessageBox::warningContinueCancel( this, i18n("<p>Be aware that setting the data from EXIF may "
                                                    "<b>overwrite</b> data you have previously entered "
                                                    "manually using the image configuration dialog.</p>" ),
                                         i18n( "Override image dates" ) );
    if ( ret == KMessageBox::Cancel )
        _date->setChecked( false );
}

#include "ReReadDialog.moc"
