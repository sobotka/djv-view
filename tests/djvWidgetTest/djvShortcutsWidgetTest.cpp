//------------------------------------------------------------------------------
// Copyright (c) 2004-2014 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djvShortcutsWidgetTest.cpp

#include <djvShortcutsWidgetTest.h>

#include <djvShortcutsWidget.h>

#include <djvSystem.h>

#include <QHBoxLayout>

QString djvShortcutsWidgetTest::name()
{
    return "djvShortcutsWidgetTest";
}

void djvShortcutsWidgetTest::run(const QStringList & args)
{
    QWidget * window = new QWidget;
    
    djvShortcutsWidget * widget = new djvShortcutsWidget;
    
    QHBoxLayout * layout = new QHBoxLayout(window);
    layout->addWidget(widget);
    
    widget->setShortcuts(QVector<djvShortcut>() <<
        djvShortcut("A", QKeySequence("A")) <<
        djvShortcut("B", QKeySequence("Ctrl+B")) <<
        djvShortcut("C", QKeySequence("Alt+C")));
    
    connect(
        widget,
        SIGNAL(shortcutsChanged(const QVector<djvShortcut> &)),
        SLOT(callback(const QVector<djvShortcut> &)));
    
    window->show();
}

void djvShortcutsWidgetTest::callback(const QVector<djvShortcut> & shortcuts)
{
    for (int i = 0; i < shortcuts.count(); ++i)
    {
        djvSystem::print(QString("%1: %2").
            arg(shortcuts[i].name).
            arg(shortcuts[i].value.toString()));
    }
}