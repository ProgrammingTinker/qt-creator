/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
**************************************************************************/
    
#ifndef OPENEDITORSVIEW_H
#define OPENEDITORSVIEW_H

#include "ui_openeditorsview.h"

#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/inavigationwidgetfactory.h>

#include <QtCore/QList>
#include <QtGui/QWidget>
#include <QtGui/QKeySequence>
#include <QtGui/QAbstractButton>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QStyledItemDelegate>

namespace Core {
namespace Internal {

class OpenEditorsWidget : public QWidget
{
    Q_OBJECT

public:
    OpenEditorsWidget();
    ~OpenEditorsWidget();

private slots:
    void selectEditor(const QModelIndex &);
    void updateCurrentItem(Core::IEditor*);

private:
    Ui::OpenEditorsView m_ui;
    QWidget *m_widget;
};

class OpenEditorsViewFactory : public Core::INavigationWidgetFactory
{
public:
    OpenEditorsViewFactory();
    virtual ~OpenEditorsViewFactory();
    QString displayName();
    virtual QKeySequence activationSequence();
    Core::NavigationView createWidget();
};

} // namespace Internal
} // namespace Core

class OpenEditorsDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    OpenEditorsDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};


#endif // OPENEDITORSVIEW_H
