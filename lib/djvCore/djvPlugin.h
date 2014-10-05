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

//! \file djvPlugin.h

#ifndef DJV_PLUGIN_H
#define DJV_PLUGIN_H

#include <djvError.h>
#include <djvUtil.h>

#include <QObject>
#include <QString>

//! \addtogroup djvCoreMisc
//@{

//------------------------------------------------------------------------------
//! \class djvPlugin
//!
//! This class provides the base functionality for plugins.
//!
//! \todo Describe the plugin naming conventions.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT djvPlugin
{
public:

    //! Destructor.

    virtual ~djvPlugin() = 0;

    //! Initialize the plugin.

    virtual void initPlugin() throw (djvError) {}

    //! Release the plugin.

    virtual void releasePlugin() {}

    //! Copy the plugin.

    virtual djvPlugin * copyPlugin() const = 0;

    //! Get the plugin name.

    virtual QString pluginName() const = 0;
};

#if defined(DJV_WINDOWS)

DJV_CORE_EXPORT_TEMPLATE template class DJV_CORE_EXPORT QList<djvPlugin *>;

#endif // DJV_WINDOWS

//! Plugin entry point export.

#if defined(DJV_WINDOWS)
#define DJV_PLUGIN_EXPORT __declspec(dllexport)
#else
#define DJV_PLUGIN_EXPORT
#endif

//! Plugin entry point.

typedef djvPlugin * (djvPluginEntry)();

//------------------------------------------------------------------------------
//! \class djvPluginFactory
//!
//! This class provides the base functionality for plugin factories.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT djvPluginFactory : public QObject
{
    Q_OBJECT
    
public:

    //! Constructor.

    djvPluginFactory(
        const QStringList & searchPath,
        const QString &     pluginEntry,
        const QString &     pluginPrefix = "djv",
        const QString &     pluginSuffix = "Plugin",
        QObject *           parent       = 0)
        throw (djvError);

    //! Destructor.

    virtual ~djvPluginFactory() = 0;

    //! Get the list of plugins.

    QList<djvPlugin *> plugins() const;
    
    //! Get a plugin by name.
    
    djvPlugin * plugin(const QString &) const;

    //! Get the list of plugin names.

    QStringList names() const;

private:
    
    DJV_PRIVATE_COPY(djvPluginFactory);
    DJV_PRIVATE_IMPLEMENTATION();
};

//@} // djvCoreMisc

#endif // DJV_PLUGIN_H
