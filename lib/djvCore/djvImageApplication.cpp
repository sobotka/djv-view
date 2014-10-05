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

//! \file djvImageApplication.cpp

#include <djvImageApplication.h>

#include <djvError.h>
#include <djvImageIo.h>
#include <djvOpenGlContext.h>
#include <djvOpenGlImage.h>
#include <djvStringUtil.h>

#include <QVector>

//------------------------------------------------------------------------------
// djvAbstractImageApplication::P
//------------------------------------------------------------------------------

struct djvAbstractImageApplication::P
{
    P() :
        context(0)
    {}
    
    djvOpenGlContext * context;
};

//------------------------------------------------------------------------------
// djvAbstractImageApplication
//------------------------------------------------------------------------------

djvAbstractImageApplication::djvAbstractImageApplication(
    const QString & name,
    int &           argc,
    char **         argv) throw (djvError) :
    djvAbstractCoreApplication(name, argc, argv),
    _p(new P)
{
    //DJV_DEBUG("djvAbstractImageApplication::djvAbstractImageApplication");
    //DJV_DEBUG_PRINT("name = " << name);

    // Create the default OpenGL context.

    _p->context = djvOpenGlContextFactory::create();

    //! Force image I/O plugin loading.

    djvImageIoFactory::global();

    //DJV_DEBUG_PRINT("image I/O base = " <<
    //    djvImageIoBaseFactory::global()->plugins().count());
    //DJV_DEBUG_PRINT("image load = " <<
    //    ImageLoadFactory::global()->plugins().count());
    //DJV_DEBUG_PRINT("image save = " <<
    //    ImageSaveFactory::global()->plugins().count());
}

djvAbstractImageApplication::~djvAbstractImageApplication()
{
    //DJV_DEBUG("djvAbstractImageApplication::~djvAbstractImageApplication");

    delete _p->context;
    
    delete _p;
}

djvOpenGlContext * djvAbstractImageApplication::context()
{
    return _p->context;
}

namespace
{

const QString labelInfo =
"%1"
"\n"
" OpenGL\n"
"\n"
"     Vendor: %2\n"
"     Renderer: %3\n"
"     Version: %4\n"
"\n"
" Image I/O\n"
"\n"
"     Plugins: %5\n";

} // namespace

QString djvAbstractImageApplication::info() const
{
    return QString(labelInfo).
        arg(djvAbstractCoreApplication::info()).
        arg(_p->context->vendor()).
        arg(_p->context->renderer()).
        arg(_p->context->version()).
        arg(djvImageIoFactory::global()->names().join(", "));
}

void djvAbstractImageApplication::commandLine(QStringList & in) throw (djvError)
{
    //DJV_DEBUG("djvAbstractImageApplication::commandLine");
    //DJV_DEBUG_PRINT("in = " << in);

    djvAbstractCoreApplication::commandLine(in);

    if (djvAbstractCoreApplication::exitValue() != EXIT_VALUE_DEFAULT)
        return;
    
    Q_FOREACH (djvPlugin * plugin, djvImageIoFactory::global()->plugins())
    {
        djvImageIo * io = static_cast<djvImageIo *>(plugin);
        
        io->commandLine(in);
    }

    QStringList tmp;
    QString     arg;

    try
    {
        while (! in.isEmpty())
        {
            in >> arg;

            // OpenGL options.

            if ("-render_filter" == arg)
            {
                djvOpenGlImageFilter value;
                in >> value;
                djvOpenGlImageFilter::setFilterGlobal(value);
            }

            else if ("-render_filter_high" == arg)
            {
                djvOpenGlImageFilter::setFilterGlobal(
                    djvOpenGlImageFilter::filterHighQuality());
            }

            // Leftovers.

            else
            {
                tmp << arg;
            }
        }
    }
    catch (const QString &)
    {
        in = tmp;

        throw djvError(arg);
    }

    in = tmp;
}

namespace
{

const QString commandLineHelpLabel =
"%1"
"\n"
" OpenGL Options\n"
"\n"
"     -render_filter (minify) (magnify)\n"
"         Set the render filter. Options = %2. Default = %3, %4.\n"
"     -render_filter_high\n"
"         Set the render filter to high quality settings (%5, %6).\n"
"%7";

} // namespace

QString djvAbstractImageApplication::commandLineHelp() const
{
    QString imageIoHelp;

    Q_FOREACH(djvPlugin * plugin, djvImageIoFactory::global()->plugins())
    {
        djvImageIo * io = static_cast<djvImageIo *>(plugin);
        
        imageIoHelp += io->commandLineHelp();
    }
    
    return QString(commandLineHelpLabel).
        arg(imageIoHelp).
        arg(djvOpenGlImageFilter::filterLabels().join(", ")).
        arg(djvStringUtil::label(djvOpenGlImageFilter::filterGlobal().min).join(", ")).
        arg(djvStringUtil::label(djvOpenGlImageFilter::filterGlobal().mag).join(", ")).
        arg(djvStringUtil::label(djvOpenGlImageFilter::filterHighQuality().min).join(", ")).
        arg(djvStringUtil::label(djvOpenGlImageFilter::filterHighQuality().mag).join(", ")).
        arg(djvAbstractCoreApplication::commandLineHelp());
}

//------------------------------------------------------------------------------
// djvImageApplication
//------------------------------------------------------------------------------

djvImageApplication::djvImageApplication(const QString & name, int & argc, char ** argv)
    throw (djvError) :
    QCoreApplication(argc, argv),
    djvAbstractImageApplication(name, argc, argv)
{
    setOrganizationName("djv.sourceforge.net");
    
    setApplicationName(name);
}
