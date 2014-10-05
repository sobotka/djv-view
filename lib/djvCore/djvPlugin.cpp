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

//! \file djvPlugin.cpp

#include <djvPlugin.h>

#include <djvCoreApplication.h>
#include <djvDebug.h>
#include <djvError.h>
#include <djvErrorUtil.h>
#include <djvFileInfoUtil.h>

#include <QStringList>
#include <QMap>

#if defined(DJV_WINDOWS)

#define WIN32_LEAN_AND_MEAN

#define NOMINMAX

#include <windows.h>

#endif // DJV_WINDOWS

#include <algorithm>
#if ! defined(DJV_WINDOWS)
#include <dlfcn.h>
#endif

//----------------------------------------------7-------------------------------
// djvPlugin
//------------------------------------------------------------------------------

djvPlugin::~djvPlugin()
{}

//------------------------------------------------------------------------------
// djvPluginFactory::P
//------------------------------------------------------------------------------

namespace
{

class Handle
{
public:

    Handle() :
        _p(0)
    {}

    ~Handle()
    {
#if defined(DJV_WINDOWS)

        if (_p)
        {
            ::FreeLibrary(_p);
        }

#else // DJV_WINDOWS

        if (_p)
        {
            ::dlclose(_p);
        }

#endif // DJV_WINDOWS
    }

    void open(const QString & in) throw (djvError)
    {
#if defined(DJV_WINDOWS)

        _p = ::LoadLibrary(in.toLatin1().data());

#else // DJV_WINDOWS

        _p = ::dlopen(in.toLatin1().data(), RTLD_LAZY);

#endif // DJV_WINDOWS

        if (! _p)
        {
            QString error;

#if defined(DJV_WINDOWS)

            error = djvErrorUtil::lastError();

#else // DJV_WINDOWS

            error = ::dlerror();

#endif // DJV_WINDOWS

            throw djvError("djvPluginFactory", error);
        }
    }

    void * fnc(const QString & in) const
    {
#if defined(DJV_WINDOWS)

        return (void *)::GetProcAddress(_p, in.toLatin1().data());

#else // DJV_WINDOWS

        return ::dlsym(_p, in.toLatin1().data());

#endif // DJV_WINDOWS
    }

private:

#if defined(DJV_WINDOWS)

    HINSTANCE _p;

#else // DJV_WINDOWS

    void * _p;

#endif // DJV_WINDOWS
};

} // namespace

struct djvPluginFactory::P
{
    typedef QPair<djvPlugin *, Handle *> Pair;
    
    QString             pluginPrefix;
    QString             pluginEntry;
    QMap<QString, Pair> plugins;
};

//------------------------------------------------------------------------------
// djvPluginFactory
//------------------------------------------------------------------------------

djvPluginFactory::djvPluginFactory(
    const QStringList & searchPath,
    const QString &     pluginEntry,
    const QString &     pluginPrefix,
    const QString &     pluginSuffix,
    QObject *           parent) throw (djvError) :
    QObject(parent),
    _p(new P)
{
    //DJV_DEBUG("djvPluginFactory::djvPluginFactory");
    //DJV_DEBUG_PRINT("search path = " << searchPath);
    //DJV_DEBUG_PRINT("plugin entry = " << pluginEntry);
    //DJV_DEBUG_PRINT("plugin prefix = " << pluginPrefix);
    //DJV_DEBUG_PRINT("plugin suffix = " << pluginSuffix);

    //! \todo Hard-coded OS specific shared library file extensions.

    QStringList glob;

#if defined(DJV_WINDOWS)

    glob += pluginPrefix + "*" + pluginSuffix + ".dll";

#else // DJV_WINDOWS

    glob += "lib" + pluginPrefix + "*" + pluginSuffix + ".so";

#endif // DJV_WINDOWS

    //DJV_DEBUG_PRINT("glob = " << glob);

    // Find plugins.

    QStringList list;

    for (int i = 0; i < searchPath.count(); ++i)
    {
        //DJV_DEBUG_PRINT("searching = " << searchPath[i]);

        djvFileInfoList items = djvFileInfoUtil::list(searchPath[i], djvSequence::COMPRESS_OFF);
        
        djvFileInfoUtil::filter(items, djvFileInfoUtil::FILTER_NONE, QString(), glob);

        //DJV_DEBUG_PRINT("items = " << items.count());

        for (int i = 0; i < items.count(); ++i)
        {
            //DJV_DEBUG_PRINT("found = " << items[i]);

            list += items[i];
        }
    }

    // Load plugins.

    //DJV_DEBUG_PRINT("size = " << list.count());

    for (int i = 0; i < list.count(); ++i)
    {
        //DJV_DEBUG_PRINT("loading = " << list[i]);

        // Open.

        QScopedPointer<Handle> handle(new Handle);

        try
        {
            handle->open(list[i]);
        }
        catch (const djvError & error)
        {
            DJV_CORE_APP->printError(djvError(
                "djvPluginFactory",
                QString("Cannot open plugin \"%1\": %2").
                    arg(list[i]).
                    arg(error.string())));

            continue;
        }

        djvPluginEntry * entry = (djvPluginEntry *)handle->fnc(pluginEntry);

        if (! entry)
        {
            continue;
        }

        QScopedPointer<djvPlugin> plugin;

        try
        {
            plugin.reset(entry());
        }
        catch (const djvError & error)
        {
            DJV_CORE_APP->printError(error);

            plugin.reset();
        }

        if (! plugin.data())
        {
            DJV_CORE_APP->printError(djvError(
                "djvPluginFactory",
                QString("Cannot load plugin \"%1\"").
                    arg(list[i])));

            continue;
        }

        //DJV_DEBUG_PRINT("name = " << plugin->pluginName());

        // Check for duplicates.

        if (_p->plugins.contains(plugin->pluginName()))
        {
            //DJV_DEBUG_PRINT("duplicate");

            plugin.reset();

            continue;
        }

        // Initialize.

        try
        {
            plugin->initPlugin();
        }
        catch (const djvError & error)
        {
            DJV_CORE_APP->printError(error);

            plugin->releasePlugin();
            plugin.reset();

            continue;
        }

        // Add.

        djvPlugin * p = plugin.take();
        Handle    * h = handle.take();

        _p->plugins[p->pluginName()] = P::Pair(p, h);
    }
}

djvPluginFactory::~djvPluginFactory()
{
    //DJV_DEBUG("djvPluginFactory::~djvPluginFactory");

    Q_FOREACH(P::Pair pair, _p->plugins)
    {
        pair.first->releasePlugin();

        delete pair.first;
        delete pair.second;
    }

    delete _p;
}

QList<djvPlugin *> djvPluginFactory::plugins() const
{
    QList<djvPlugin *> list;
    
    Q_FOREACH(P::Pair pair, _p->plugins)
    {
        list += pair.first;
    }
    
    return list;
}
    
djvPlugin * djvPluginFactory::plugin(const QString & name) const
{
    return _p->plugins.contains(name) ? _p->plugins[name].first : 0;
}

QStringList djvPluginFactory::names() const
{
    QStringList out;

    Q_FOREACH(QString name, _p->plugins.keys())
    {
        out += name;
    }

    return out;
}
