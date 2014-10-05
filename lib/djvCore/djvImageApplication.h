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

//! \file djvImageApplication.h

#ifndef DJV_IMAGE_APPLICATION_H
#define DJV_IMAGE_APPLICATION_H

#include <djvCoreApplication.h>

class djvOpenGlContext;

//! \addtogroup djvCoreMisc
//@{

//------------------------------------------------------------------------------
//! \class djvAbstractImageApplication
//!
//! This class provides the core functionality for image applications.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT djvAbstractImageApplication : public djvAbstractCoreApplication
{
public:

    //! Constructor.

    djvAbstractImageApplication(const QString & name, int & argc, char ** argv)
        throw (djvError);

    //! Destructor.

    virtual ~djvAbstractImageApplication() = 0;

    //! Get the default OpenGL context.

    djvOpenGlContext * context();
    
    virtual QString info() const;

    virtual void commandLine(QStringList &) throw (djvError);

    virtual QString commandLineHelp() const;

private:

    DJV_PRIVATE_IMPLEMENTATION();
};

//------------------------------------------------------------------------------
//! \class djvImageApplication
//!
//! This class provides the base functionality for image applications.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT djvImageApplication :
    public QCoreApplication,
    public djvAbstractImageApplication
{
public:

    //! Constructor.

    djvImageApplication(const QString & name, int & argc, char ** argv)
        throw (djvError);
};

//! Get the global application instance.

#define DJV_IMAGE_APP \
    dynamic_cast<djvAbstractImageApplication *>(AbstractImageApplication::global())

//@} // djvCoreMisc

#endif // DJV_IMAGE_APPLICATION_H
