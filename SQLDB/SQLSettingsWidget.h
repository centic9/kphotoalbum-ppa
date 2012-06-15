/*
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

#ifndef SQLSETTINGSWIDGET_H
#define SQLSETTINGSWIDGET_H

#include <QWidget>

namespace SQLDB { class DatabaseAddress; }
class QLabel;
class QComboBox;
class QStackedWidget;
class QSpinBox;
class KUrlRequester;
class KLineEdit;

namespace SQLDB
{
    class SQLSettingsWidget: public QWidget
    {
        Q_OBJECT

    public:
        explicit SQLSettingsWidget(QWidget* parent=0, Qt::WindowFlags fl=0);

        QStringList availableDrivers() const;
        bool hasSettings() const;
        DatabaseAddress getSettings() const;
        void setSettings(const DatabaseAddress& address);

    public slots:
        void reloadDriverList();
        void selectDriver(const QString& driver);

    signals:
        void driverSelectionChanged(const QString& newDriver);
        void passwordChanged(const QString& newPassword);

    protected:
        QLabel* _errorLabel;
        QLabel* _driverLabel;
        QComboBox* _driverCombo;
        QStackedWidget* _widgetStack;
        QLabel* _fileLabel;
        KUrlRequester* _fileLine;
        QLabel* _hostLabel;
        KLineEdit* _hostLine;
        QLabel* _portLabel;
        QSpinBox* _portSpin;
        QLabel* _dbNameLabel;
        KLineEdit* _dbNameLine;
        QLabel* _usernameLabel;
        KLineEdit* _usernameLine;
        QLabel* _passwordLabel;
        KLineEdit* _passwordLine;

    protected slots:
        virtual void languageChange();
        void showOptionsOfSelectedDriver();

    private:
        enum StackPage { ErrorPage, FileSettingsPage, ServerSettingsPage };
        enum ErrorType { NoDrivers, InvalidDriver };
        ErrorType _lastErrorType;

        void setError(ErrorType errorType);
    };
}

#endif /* SQLSETTINGSWIDGET_H */