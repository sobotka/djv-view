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

//! \file djvFormLayoutAlign.h

#ifndef DJV_FORM_LAYOUT_ALIGN_H
#define DJV_FORM_LAYOUT_ALIGN_H

#include <djvGuiExport.h>

#include <djvUtil.h>

#include <QObject>

class QFormLayout;

//! \addtogroup djvGuiMisc
//@{

//------------------------------------------------------------------------------
//! \class djvFormLayoutAlign
//!
//! This class provides functionality to align the items in multiple form
//! layouts.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT djvFormLayoutAlign : public QObject
{
    Q_OBJECT
    
public:

    //! Constructor.

    explicit djvFormLayoutAlign(QObject * parent = 0);

    //! Destructor.

    virtual ~djvFormLayoutAlign();

    //! Add a layout to align.

    void addLayout(QFormLayout *);

    //! Clear the layouts.

    void clear();

public Q_SLOTS:

    //! Align the layouts.

    void align();

private:

    DJV_PRIVATE_COPY(djvFormLayoutAlign);
    DJV_PRIVATE_IMPLEMENTATION();
};

//@} // djvGuiMisc

#endif // DJV_FORM_LAYOUT_ALIGN_H
