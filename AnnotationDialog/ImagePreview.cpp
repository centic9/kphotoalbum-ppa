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
#include "ImageManager/AsyncLoader.h"
#include "ImageManager/ImageLoaderThread.h"
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

void ImagePreview::reload()
{
    if ( !_info.isNull() ) {
        QImage img;
        if (_preloader.has(_info.fileName()))
            setCurrentImage(_preloader.getImage());
        else if (_lastImage.has(_info.fileName()))
            //don't pass by reference, the additional constructor is needed here
            //see setCurrentImage for the reason (where _lastImage is changed...)
            setCurrentImage(QImage(_lastImage.getImage()));
        else {
            setPixmap(QPixmap()); //erase old image
            ImageManager::AsyncLoader::instance()->stop(this);
            ImageManager::ImageRequest* request = new ImageManager::ImageRequest( _info.fileName(), QSize( width(), height() ), _info.angle(), this );
            request->setPriority( ImageManager::Viewer );
            ImageManager::AsyncLoader::instance()->load( request );
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
    _currentImage.set(_info.fileName(), image);
    setPixmap( QPixmap::fromImage( _currentImage.getImage()) );
    if (!_anticipated._fileName.isNull())
        _preloader.preloadImage(_anticipated._fileName, width(), height(), _anticipated._angle);
}

void ImagePreview::pixmapLoaded( const DB::FileName& fileName, const QSize& /*size*/, const QSize& /*fullSize*/, int, const QImage& image, const bool loadedOK)
{
    if ( loadedOK && !_info.isNull() ) {
        if (_info.fileName() == fileName)
            setCurrentImage(image);
    }
}

void ImagePreview::anticipate(DB::ImageInfo &info1) {
    //We cannot call _preloader.preloadImage right here:
    //this function is called before reload(), so if we preload here,
    //the preloader will always be loading the image after the next image.
    _anticipated.set(info1.fileName(), info1.angle());
}


ImagePreview::PreloadInfo::PreloadInfo() : _angle(0)
{
}

void ImagePreview::PreloadInfo::set(const DB::FileName& fileName, int angle)
{
    _fileName=fileName;
    _angle=angle;
}


bool ImagePreview::PreviewImage::has(const DB::FileName &fileName) const
{
    return fileName==_fileName && !_image.isNull();
}

QImage &ImagePreview::PreviewImage::getImage()
{
    return _image;
}

void ImagePreview::PreviewImage::set(const DB::FileName &fileName, const QImage &image)
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
    _fileName = DB::FileName();
    _image=QImage();
}


void ImagePreview::PreviewLoader::pixmapLoaded( const DB::FileName& fileName, const QSize& /*size*/,
                                                const QSize& /*fullSize*/, int, const QImage& image, const bool loadedOK)
{
    if ( loadedOK )
        set(fileName, image);
}


void ImagePreview::PreviewLoader::preloadImage(const DB::FileName &fileName, int width, int height, int angle)
{
    //no need to worry about concurrent access: everything happens in the event loop thread
    reset();
    ImageManager::AsyncLoader::instance()->stop(this);
    ImageManager::ImageRequest* request = new ImageManager::ImageRequest( fileName, QSize( width, height ), angle, this );
    request->setPriority( ImageManager::ViewerPreload );
    ImageManager::AsyncLoader::instance()->load( request );
}

void ImagePreview::PreviewLoader::cancelPreload()
{
    reset();
    ImageManager::AsyncLoader::instance()->stop(this);
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
