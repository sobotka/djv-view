//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
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

//! \file djvViewInputPrefsWidget.cpp

#include <djvViewInputPrefsWidget.h>

#include <djvViewContext.h>
#include <djvViewInputPrefs.h>

#include <djvPrefsGroupBox.h>

#include <djvSignalBlocker.h>

#include <QApplication>
#include <QComboBox>
#include <QFormLayout>
#include <QVBoxLayout>

//------------------------------------------------------------------------------
// djvViewInputPrefsWidgetPrivate
//------------------------------------------------------------------------------

struct djvViewInputPrefsWidgetPrivate
{
    djvViewInputPrefsWidgetPrivate() :
        mouseWheelWidget     (0),
        mouseWheelShiftWidget(0),
        mouseWheelCtrlWidget (0)
    {}
    
    QComboBox * mouseWheelWidget;
    QComboBox * mouseWheelShiftWidget;
    QComboBox * mouseWheelCtrlWidget;
};

//------------------------------------------------------------------------------
// djvViewInputPrefsWidget
//------------------------------------------------------------------------------

djvViewInputPrefsWidget::djvViewInputPrefsWidget(djvViewContext * context) :
    djvViewAbstractPrefsWidget(
        qApp->translate("djvViewInputPrefsWidget", "Input"), context),
    _p(new djvViewInputPrefsWidgetPrivate)
{
    // Create the widgets.

    _p->mouseWheelWidget = new QComboBox;
    _p->mouseWheelWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _p->mouseWheelWidget->addItems(djvViewUtil::mouseWheelLabels());

    _p->mouseWheelShiftWidget = new QComboBox;
    _p->mouseWheelShiftWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _p->mouseWheelShiftWidget->addItems(djvViewUtil::mouseWheelLabels());

    _p->mouseWheelCtrlWidget = new QComboBox;
    _p->mouseWheelCtrlWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _p->mouseWheelCtrlWidget->addItems(djvViewUtil::mouseWheelLabels());

    // Layout the widgets.

    QVBoxLayout * layout = new QVBoxLayout(this);

    djvPrefsGroupBox * prefsGroupBox = new djvPrefsGroupBox(
        qApp->translate("djvViewInputPrefsWidget", "Mouse Wheel"), context);
    QFormLayout * formLayout = prefsGroupBox->createLayout();
    formLayout->addRow(
        qApp->translate("djvViewInputPrefsWidget", "Wheel:"),
        _p->mouseWheelWidget);
    formLayout->addRow(
        qApp->translate("djvViewInputPrefsWidget", "Shift + wheel:"),
        _p->mouseWheelShiftWidget);
    formLayout->addRow(
        qApp->translate("djvViewInputPrefsWidget", "Ctrl + wheel:"),
        _p->mouseWheelCtrlWidget);
    layout->addWidget(prefsGroupBox);

    layout->addStretch();

    // Initialize.
    
    widgetUpdate();

    // Setup the callbacks.

    connect(
        _p->mouseWheelWidget,
        SIGNAL(activated(int)),
        SLOT(mouseWheelCallback(int)));

    connect(
        _p->mouseWheelShiftWidget,
        SIGNAL(activated(int)),
        SLOT(mouseWheelShiftCallback(int)));

    connect(
        _p->mouseWheelCtrlWidget,
        SIGNAL(activated(int)),
        SLOT(mouseWheelCtrlCallback(int)));
}

djvViewInputPrefsWidget::~djvViewInputPrefsWidget()
{
    delete _p;
}

void djvViewInputPrefsWidget::resetPreferences()
{
    context()->inputPrefs()->setMouseWheel(
        djvViewInputPrefs::mouseWheelDefault());
    context()->inputPrefs()->setMouseWheelShift(
        djvViewInputPrefs::mouseWheelShiftDefault());
    context()->inputPrefs()->setMouseWheelCtrl(
        djvViewInputPrefs::mouseWheelCtrlDefault());
    
    widgetUpdate();
}

void djvViewInputPrefsWidget::mouseWheelCallback(int in)
{
    context()->inputPrefs()->setMouseWheel(
        static_cast<djvViewUtil::MOUSE_WHEEL>(in));
}

void djvViewInputPrefsWidget::mouseWheelShiftCallback(int in)
{
    context()->inputPrefs()->setMouseWheelShift(
        static_cast<djvViewUtil::MOUSE_WHEEL>(in));
}

void djvViewInputPrefsWidget::mouseWheelCtrlCallback(int in)
{
    context()->inputPrefs()->setMouseWheelCtrl(
        static_cast<djvViewUtil::MOUSE_WHEEL>(in));
}

void djvViewInputPrefsWidget::widgetUpdate()
{
    djvSignalBlocker signalBlocker(QObjectList() <<
        _p->mouseWheelWidget <<
        _p->mouseWheelShiftWidget <<
        _p->mouseWheelCtrlWidget);

    _p->mouseWheelWidget->setCurrentIndex(
        context()->inputPrefs()->mouseWheel());

    _p->mouseWheelShiftWidget->setCurrentIndex(
        context()->inputPrefs()->mouseWheelShift());

    _p->mouseWheelCtrlWidget->setCurrentIndex(
        context()->inputPrefs()->mouseWheelCtrl());
}
