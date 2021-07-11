/* SPDX-FileCopyrightText: 2003-2010 Jesper K. Pedersen <blackie@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTEXPORT_H
#define IMPORTEXPORT_H

#include <ImageManager/ImageClientInterface.h>
#include <Utilities/UniqFilenameMapper.h>

#include <QDialog>
#include <QEventLoop>
#include <QPointer>

class QRadioButton;
class QSpinBox;
class QCheckBox;
class KZip;
class QProgressDialog;

namespace DB
{
class FileNameList;
}

namespace ImportExport
{

enum ImageFileLocation { Inline,
                         ManualCopy,
                         AutoCopy,
                         Link,
                         Symlink };

class Export : public ImageManager::ImageClientInterface
{

public:
    static void imageExport(const DB::FileNameList &list);

    Export(const DB::FileNameList &list, const QString &zipFile,
           bool compress, int maxSize,
           ImageFileLocation, const QString &baseUrl,
           bool generateThumbnails,
           bool *ok);
    ~Export() override;

    static void showUsageDialog();

    // ImageManager::ImageClient callback.
    void pixmapLoaded(ImageManager::ImageRequest *request, const QImage &image) override;

protected:
    void generateThumbnails(const DB::FileNameList &list);
    void copyImages(const DB::FileNameList &list);

private:
    bool m_internalOk; // used in case m_ok is null
    bool *m_ok;
    int m_filesRemaining;
    int m_steps;
    QProgressDialog *m_progressDialog;
    KZip *m_zip;
    int m_maxSize;
    QString m_subdir;
    bool m_loopEntered;
    ImageFileLocation m_location;
    Utilities::UniqFilenameMapper m_filenameMapper;
    bool m_copyingFiles;
    QString m_destdir;
    const QPointer<QEventLoop> m_eventLoop;
};

class ExportConfig : public QDialog
{
    Q_OBJECT

public:
    ExportConfig();
    QCheckBox *mp_compress;
    QCheckBox *mp_generateThumbnails;
    QCheckBox *mp_enforeMaxSize;
    QSpinBox *mp_maxSize;

    ImageFileLocation imageFileLocation() const;

private:
    QRadioButton *m_include;
    QRadioButton *m_manually;
    QRadioButton *m_link;
    QRadioButton *m_symlink;
    QRadioButton *m_auto;

private slots:
    void showHelp();
};
}

#endif /* IMPORTEXPORT_H */

// vi:expandtab:tabstop=4 shiftwidth=4:
