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

//! \file djvViewViewPrefsWidget.cpp

#include <djvViewViewPrefsWidget.h>

#include <djvViewViewPrefs.h>

#include <djvColorSwatch.h>
#include <djvPrefsGroupBox.h>

#include <djvSignalBlocker.h>

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QListWidget>
#include <QVBoxLayout>

namespace
{

class SmallListWidget : public QListWidget
{
public:

    QSize sizeHint() const
    {
        const QSize size = QListWidget::sizeHint();

        return QSize(size.width(), size.height() / 2);
    }
};

} // namespace

//------------------------------------------------------------------------------
// djvViewViewPrefsWidget::P
//------------------------------------------------------------------------------

struct djvViewViewPrefsWidget::P
{
    P() :
        backgroundColorWidget   (0),
        resizeWidget            (0),
        gridWidget              (0),
        gridColorWidget         (0),
        hudEnabledWidget        (0),
        hudVisibleWidget        (0),
        hudColorWidget          (0),
        hudBackgroundWidget     (0),
        hudBackgroundColorWidget(0)
    {}

    djvColorSwatch *  backgroundColorWidget;
    QComboBox *       resizeWidget;
    QComboBox *       gridWidget;
    djvColorSwatch *  gridColorWidget;
    QCheckBox *       hudEnabledWidget;
    QListWidget *     hudVisibleWidget;
    djvColorSwatch *  hudColorWidget;
    QComboBox *       hudBackgroundWidget;
    djvColorSwatch *  hudBackgroundColorWidget;
};

//------------------------------------------------------------------------------
// djvViewViewPrefsWidget
//------------------------------------------------------------------------------

djvViewViewPrefsWidget::djvViewViewPrefsWidget() :
    djvViewAbstractPrefsWidget("Views"),
    _p(new P)
{
    // Create the options widgets.

    _p->backgroundColorWidget = new djvColorSwatch;
    _p->backgroundColorWidget->setSwatchSize(djvColorSwatch::SwatchSmall);
    _p->backgroundColorWidget->setColorDialogEnabled(true);

    _p->resizeWidget = new QComboBox;
    _p->resizeWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _p->resizeWidget->addItems(djvView::viewResizeLabels());

    // Create the grid widgets.

    _p->gridWidget = new QComboBox;
    _p->gridWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _p->gridWidget->addItems(djvView::gridLabels());

    _p->gridColorWidget = new djvColorSwatch();
    _p->gridColorWidget->setSwatchSize(djvColorSwatch::SwatchSmall);
    _p->gridColorWidget->setColorDialogEnabled(true);

    // Create the HUD widgets.

    _p->hudEnabledWidget = new QCheckBox("Enable the HUD");

    _p->hudVisibleWidget = new SmallListWidget;
    
    for (int i = 0; i < djvView::HUD_COUNT; ++i)
    {
        QListWidgetItem * item = new QListWidgetItem(_p->hudVisibleWidget);
        item->setText(djvView::hudVisibleLabels()[i]);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }

    _p->hudColorWidget = new djvColorSwatch;
    _p->hudColorWidget->setSwatchSize(djvColorSwatch::SwatchSmall);
    _p->hudColorWidget->setColorDialogEnabled(true);

    _p->hudBackgroundWidget = new QComboBox;
    _p->hudBackgroundWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _p->hudBackgroundWidget->addItems(djvView::hudBackgroundLabels());

    _p->hudBackgroundColorWidget = new djvColorSwatch;
    _p->hudBackgroundColorWidget->setSwatchSize(djvColorSwatch::SwatchSmall);
    _p->hudBackgroundColorWidget->setColorDialogEnabled(true);

    // Layout the widgets.

    QVBoxLayout * layout = new QVBoxLayout(this);

    djvPrefsGroupBox * prefsGroupBox = new djvPrefsGroupBox(
        "Views",
        "Set general view options.");
    QFormLayout * formLayout = prefsGroupBox->createLayout();
    formLayout->addRow("Background color:", _p->backgroundColorWidget);
    layout->addWidget(prefsGroupBox);

    prefsGroupBox = new djvPrefsGroupBox(
        "Resizing",
        "Set what happens to the image when the view is resized.");
    formLayout = prefsGroupBox->createLayout();
    formLayout->addRow(_p->resizeWidget);
    layout->addWidget(prefsGroupBox);

    prefsGroupBox = new djvPrefsGroupBox(
        "Grid",
        "Set options for the grid overlay.");
    formLayout = prefsGroupBox->createLayout();
    formLayout->addRow("Size:", _p->gridWidget);
    formLayout->addRow("Color:", _p->gridColorWidget);
    layout->addWidget(prefsGroupBox);

    prefsGroupBox = new djvPrefsGroupBox(
        "HUD",
        "Set options for the HUD (Heads Up Display).");
    formLayout = prefsGroupBox->createLayout();
    formLayout->addRow(_p->hudEnabledWidget);
    formLayout->addRow("Visible:", _p->hudVisibleWidget);
    formLayout->addRow("Color:", _p->hudColorWidget);
    formLayout->addRow("Background:", _p->hudBackgroundWidget);
    formLayout->addRow("Background color:", _p->hudBackgroundColorWidget);
    layout->addWidget(prefsGroupBox);

    layout->addStretch();

    // Initialize.
    
    widgetUpdate();

    // Setup the callbacks.

    connect(
        _p->backgroundColorWidget,
        SIGNAL(colorChanged(const djvColor &)),
        SLOT(backgroundCallback(const djvColor &)));

    connect(
        _p->resizeWidget,
        SIGNAL(currentIndexChanged(int)),
        SLOT(resizeCallback(int)));

    connect(
        _p->gridWidget,
        SIGNAL(currentIndexChanged(int)),
        SLOT(gridCallback(int)));

    connect(
        _p->gridColorWidget,
        SIGNAL(colorChanged(const djvColor &)),
        SLOT(gridColorCallback(const djvColor &)));

    connect(
        _p->hudEnabledWidget,
        SIGNAL(toggled(bool)),
        SLOT(hudEnabledCallback(bool)));

    connect(
        _p->hudVisibleWidget,
        SIGNAL(itemChanged(QListWidgetItem *)),
        SLOT(hudVisibleCallback(QListWidgetItem *)));

    connect(
        _p->hudColorWidget,
        SIGNAL(colorChanged(const djvColor &)),
        SLOT(hudColorCallback(const djvColor &)));

    connect(
        _p->hudBackgroundWidget,
        SIGNAL(currentIndexChanged(int)),
        SLOT(hudBackgroundCallback(int)));

    connect(
        _p->hudBackgroundColorWidget,
        SIGNAL(colorChanged(const djvColor &)),
        SLOT(hudBackgroundColorCallback(const djvColor &)));
}

djvViewViewPrefsWidget::~djvViewViewPrefsWidget()
{
    delete _p;
}

void djvViewViewPrefsWidget::resetPreferences()
{
    djvViewViewPrefs::global()->setBackground(
        djvViewViewPrefs::backgroundDefault());
    djvViewViewPrefs::global()->setResize(
        djvViewViewPrefs::resizeDefault());
    djvViewViewPrefs::global()->setGrid(
        djvViewViewPrefs::gridDefault());
    djvViewViewPrefs::global()->setGridColor(
        djvViewViewPrefs::gridColorDefault());
    djvViewViewPrefs::global()->setHudEnabled(
        djvViewViewPrefs::hudEnabledDefault());
    djvViewViewPrefs::global()->setHudVisible(
        djvViewViewPrefs::hudVisibleDefault());
    djvViewViewPrefs::global()->setHudColor(
        djvViewViewPrefs::hudColorDefault());
    djvViewViewPrefs::global()->setHudBackground(
        djvViewViewPrefs::hudBackgroundDefault());
    djvViewViewPrefs::global()->setHudBackgroundColor(
        djvViewViewPrefs::hudBackgroundColorDefault());

    widgetUpdate();
}

void djvViewViewPrefsWidget::backgroundCallback(const djvColor & in)
{
    _p->backgroundColorWidget->setColor(in);

    djvViewViewPrefs::global()->setBackground(in);
}

void djvViewViewPrefsWidget::resizeCallback(int in)
{
    djvViewViewPrefs::global()->setResize(
        static_cast<djvView::VIEW_RESIZE>(in));
}

void djvViewViewPrefsWidget::gridCallback(int in)
{
    djvViewViewPrefs::global()->setGrid(
        static_cast<djvView::GRID>(in));
}

void djvViewViewPrefsWidget::gridColorCallback(const djvColor & in)
{
    _p->gridColorWidget->setColor(in);

    djvViewViewPrefs::global()->setGridColor(in);
}

void djvViewViewPrefsWidget::hudEnabledCallback(bool in)
{
    djvViewViewPrefs::global()->setHudEnabled(in);
}

void djvViewViewPrefsWidget::hudVisibleCallback(QListWidgetItem * item)
{
    const int row = _p->hudVisibleWidget->row(item);

    QVector<bool> visible = djvViewViewPrefs::global()->hudVisible();

    visible[row] = ! visible[row];

    djvViewViewPrefs::global()->setHudVisible(visible);
}

void djvViewViewPrefsWidget::hudColorCallback(const djvColor & in)
{
    _p->hudColorWidget->setColor(in);

    djvViewViewPrefs::global()->setHudColor(in);
}

void djvViewViewPrefsWidget::hudBackgroundCallback(int in)
{
    djvViewViewPrefs::global()->setHudBackground(
        static_cast<djvView::HUD_BACKGROUND>(in));
}

void djvViewViewPrefsWidget::hudBackgroundColorCallback(const djvColor & in)
{
    _p->hudBackgroundColorWidget->setColor(in);

    djvViewViewPrefs::global()->setHudBackgroundColor(in);
}

void djvViewViewPrefsWidget::widgetUpdate()
{
    djvSignalBlocker signalBlocker(QObjectList() <<
        _p->backgroundColorWidget <<
        _p->resizeWidget <<
        _p->gridWidget <<
        _p->gridColorWidget <<
        _p->hudEnabledWidget <<
        _p->hudVisibleWidget <<
        _p->hudColorWidget <<
        _p->hudBackgroundWidget <<
        _p->hudBackgroundColorWidget);

    _p->backgroundColorWidget->setColor(
        djvViewViewPrefs::global()->background());

    _p->resizeWidget->setCurrentIndex(
        djvViewViewPrefs::global()->resize());

    _p->gridWidget->setCurrentIndex(
        djvViewViewPrefs::global()->grid());

    _p->gridColorWidget->setColor(
        djvViewViewPrefs::global()->gridColor());

    _p->hudEnabledWidget->setChecked(
        djvViewViewPrefs::global()->isHudEnabled());

    QVector<bool> hudVisible = djvViewViewPrefs::global()->hudVisible();

    for (int i = 0; i < djvView::HUD_COUNT; ++i)
    {
        QListWidgetItem * item = _p->hudVisibleWidget->item(i);

        item->setCheckState(hudVisible[i] ? Qt::Checked : Qt::Unchecked);
    }

    _p->hudColorWidget->setColor(
        djvViewViewPrefs::global()->hudColor());

    _p->hudBackgroundWidget->setCurrentIndex(
        djvViewViewPrefs::global()->hudBackground());

    _p->hudBackgroundColorWidget->setColor(
        djvViewViewPrefs::global()->hudBackgroundColor());
}
