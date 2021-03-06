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

//! \file djvImageViewTest.h

#include <djvGuiContext.h>
#include <djvImageView.h>

#include <djvFileInfo.h>
#include <djvImage.h>
#include <djvImageIo.h>

#include <QApplication>
#include <QScopedPointer>

class djvImageViewTestWidget : public djvImageView
{
    Q_OBJECT

public:

    explicit djvImageViewTestWidget(djvGuiContext *);
    
protected:

    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *);

private:

    djvVector2i _viewPosTmp;
    djvVector2i _mousePress;
};

class djvImageViewTestApplication : public QApplication
{
    Q_OBJECT
    
public:

    djvImageViewTestApplication(int & argc, char ** argv);

private Q_SLOTS:

    void commandLineExit();
    void work();

private:

    QScopedPointer<djvGuiContext>          _context;
    djvFileInfo                            _fileInfo;
    QScopedPointer<djvImageLoad>           _load;
    djvImage                               _image;
    QScopedPointer<djvImageViewTestWidget> _widget;
};
