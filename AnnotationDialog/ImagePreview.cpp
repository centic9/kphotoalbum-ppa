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

#include "ImagePreview.h"
#include "ImageManager/Manager.h"
#include "ImageManager/ImageLoader.h"
#include "Utilities/Util.h"

using namespace AnnotationDialog;

ImagePreview::ImagePreview( QWidget* parent )
    : QLabel( parent )
{
    setAlignment( Qt::AlignCenter );
    setMinimumSize( 64, 64 );
}

void ImagePreview::resizeEvent( QResizeEvent* )
{
    _preloader.cancelPreload();
    _lastImage.reset();
    reload();
}

QSize ImagePreview::sizeHint() const
{
    return QSize( 128,128 );
}

void ImagePreview::rotate(int angle)
{
    if ( !_info.isNull() )
        _info.rotate( angle );
    else
        _angle += angle;
    _preloader.cancelPreload();
    _lastImage.reset();
    reload();
}

void ImagePreview::setImage( const DB::ImageInfo& info )
{
    _info = info;
    reload();
}

/**
   This method should only be used for the non-user images. Currently this includes
   two images: the search image and the configure several images at a time image.
*/
void ImagePreview::setImage( const QString& fileName )
{
    _fileName = fileName;
    _info = DB::ImageInfo();
    _angle = 0;
    reload();
}

const QString& ImagePreview::lastImageFileName()
{
    return _lastImage.getName();
}


void ImagePreview::reload()
{
    if ( !_info.isNull() ) {
        QImage img;
        if (_preloader.has(_info.fileName(DB::AbsolutePath)))
            setCurrentImage(_preloader.getImage());
        else if (_lastImage.has(_info.fileName(DB::AbsolutePath)))
            //don't pass by reference, the additional constructor is needed here
            //see setCurrentImage for the reason (where _lastImage is changed...)
            setCurrentImage(QImage(_lastImage.getImage()));
        else {
            setPixmap(QPixmap()); //erase old image
            ImageManager::Manager::instance()->stop(this);
            ImageManager::ImageRequest* request = new ImageManager::ImageRequest( _info.fileName(DB::AbsolutePath), QSize( width(), height() ), _info.angle(), this );
            request->setPriority( ImageManager::Viewer );
            ImageManager::Manager::instance()->load( request );
        }
    }
    else {
        QImage img( _fileName );
        img = rotateAndScale( img, width(), height(), _angle );
        setPixmap( QPixmap::fromImage(img) );
    }
}

int ImagePreview::angle() const
{
    Q_ASSERT( !_info.isNull() );
    return _angle;
}

void ImagePreview::setCurrentImage(const QImage &image)
{
    //cache the current image as the last image before changing it
    _lastImage.set(_currentImage);
    _currentImage.set(_info.fileName(DB::AbsolutePath), image);
    setPixmap( QPixmap::fromImage( _currentImage.getImage()) );
    if (!_anticipated._fileName.isEmpty())
        _preloader.preloadImage(_anticipated._fileName, width(), height(), _anticipated._angle);
}

void ImagePreview::pixmapLoaded( const QString& fileName, const QSize& /*size*/, const QSize& /*fullSize*/, int, const QImage& image, const bool loadedOK)
{
    if ( loadedOK && !_info.isNull() ) {
        if (_info.fileName(DB::AbsolutePath) == fileName)
            setCurrentImage(image);
    }
}

void ImagePreview::anticipate(DB::ImageInfo &info1) {
    //We cannot call _preloader.preloadImage right here:
    //this function is called before reload(), so if we preload here,
    //the preloader will always be loading the image after the next image.
    _anticipated.set(info1.fileName(DB::AbsolutePath), info1.angle());
}


ImagePreview::PreloadInfo::PreloadInfo() : _angle(0)
{
}

void ImagePreview::PreloadInfo::set(const QString& fileName, int angle)
{
    _fileName=fileName;
    _angle=angle;
}


bool ImagePreview::PreviewImage::has(const QString &fileName) const
{
    return fileName==_fileName && !_image.isNull();
}

QImage &ImagePreview::PreviewImage::getImage()
{
    return _image;
}

const QString &ImagePreview::PreviewImage::getName() const
{
    return _fileName;
}

void ImagePreview::PreviewImage::set(const QString &fileName, const QImage &image)
{
    _fileName=fileName;
    _image=image;
}

void ImagePreview::PreviewImage::set(const PreviewImage &other)
{
    _fileName=other._fileName;
    _image=other._image;
}

void ImagePreview::PreviewImage::reset()
{
    _fileName.clear();
    _image=QImage();
}


void ImagePreview::PreviewLoader::pixmapLoaded( const QString& fileName, const QSize& /*size*/,
                                                const QSize& /*fullSize*/, int, const QImage& image, const bool loadedOK)
{
    if ( loadedOK )
        set(fileName, image);
}


void ImagePreview::PreviewLoader::preloadImage(const QString &fileName, int width, int height, int angle)
{
    //no need to worry about concurrent access: everything happens in the event loop thread
    reset();
    ImageManager::Manager::instance()->stop(this);
    ImageManager::ImageRequest* request = new ImageManager::ImageRequest( fileName, QSize( width, height ), angle, this );
    request->setPriority( ImageManager::ViewerPreload );
    ImageManager::Manager::instance()->load( request );
}

void ImagePreview::PreviewLoader::cancelPreload()
{
    reset();
    ImageManager::Manager::instance()->stop(this);
}

QImage AnnotationDialog::ImagePreview::rotateAndScale(QImage img, int width, int height, int angle) const
{
    if ( angle != 0 )  {
        QMatrix matrix;
        matrix.rotate( angle );
        img = img.transformed( matrix );
    }
    img = Utilities::scaleImage(img, width, height, Qt::KeepAspectRatio );
    return img;
}


#include "ImagePreview.moc"
