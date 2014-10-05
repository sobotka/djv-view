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

//! \file djvImageIo.cpp

#include <djvImageIo.h>

#include <djvDebug.h>
#include <djvError.h>
#include <djvFileInfo.h>

#include <QCoreApplication>
#include <QMap>
#include <QPointer>

#include <algorithm>

//------------------------------------------------------------------------------
// djvImageIoInfo
//------------------------------------------------------------------------------

djvImageIoInfo::djvImageIoInfo() :
    _info(1)
{}

djvImageIoInfo::djvImageIoInfo(const djvPixelDataInfo & in) :
    djvPixelDataInfo(in),
    _info(1)
{}

void djvImageIoInfo::addLayer(const djvPixelDataInfo & in)
{
    _info += in;
}

int djvImageIoInfo::layerCount() const
{
    return _info.count();
}

void djvImageIoInfo::setLayerCount(int count)
{
    _info.resize(count);
}

void djvImageIoInfo::clearLayers()
{
    _info.resize(1);
}

djvPixelDataInfo & djvImageIoInfo::operator [] (int index)
{
    return 0 == index ? static_cast<djvPixelDataInfo &>(*this) : _info[index];
}

const djvPixelDataInfo & djvImageIoInfo::operator [] (int index) const
{
    return 0 == index ? static_cast<const djvPixelDataInfo &>(*this) : _info[index];
}

//------------------------------------------------------------------------------
// djvImageIoFrameInfo
//------------------------------------------------------------------------------

djvImageIoFrameInfo::djvImageIoFrameInfo(
    qint64                  frame,
    int                     layer,
    djvPixelDataInfo::PROXY proxy) :
    frame     (frame),
    layer     (layer),
    proxy     (proxy)
{}

//------------------------------------------------------------------------------
// djvImageLoad
//------------------------------------------------------------------------------

djvImageLoad::~djvImageLoad()
{}

void djvImageLoad::close() throw (djvError)
{}

//------------------------------------------------------------------------------
// djvImageSave
//------------------------------------------------------------------------------

djvImageSave::~djvImageSave()
{}

void djvImageSave::close() throw (djvError)
{}

//------------------------------------------------------------------------------
// djvImageIo
//------------------------------------------------------------------------------

djvImageIo::djvImageIo()
{}

djvImageIo::~djvImageIo()
{}

QStringList djvImageIo::extensions() const
{
    return QStringList();
}

bool djvImageIo::isSequence() const
{
    return true;
}

QStringList djvImageIo::option(const QString &) const
{
    return QStringList();
}

bool djvImageIo::setOption(const QString &, QStringList &)
{
    return false;
}

QStringList djvImageIo::options() const
{
    return QStringList();
}

void djvImageIo::commandLine(QStringList &) throw (djvError)
{}

QString djvImageIo::commandLineHelp() const
{
    return QString();
}

djvImageLoad * djvImageIo::createLoad() const
{
    return 0;
}
    
djvImageSave * djvImageIo::createSave() const
{
    return 0;
}

djvAbstractPrefsWidget * djvImageIo::createWidget()
{
    return 0;
}

void djvImageIo::throwUnrecognized(
    const QString & plugin,
    const QString & fileName) throw (djvError)
{
    throw djvError(
        plugin,
        QString("Unrecognized file: %1").arg(fileName));
}

void djvImageIo::throwUnsupported(
    const QString & plugin,
    const QString & fileName) throw (djvError)
{
    throw djvError(
        plugin,
        QString("Unsupported file: %1").arg(fileName));
}

void djvImageIo::throwErrorOpen(
    const QString & plugin,
    const QString & fileName) throw (djvError)
{
    throw djvError(
        plugin,
        QString("Cannot open file: %1").arg(fileName));
}

void djvImageIo::throwErrorRead(
    const QString & plugin,
    const QString & fileName) throw (djvError)
{
    throw djvError(
        plugin,
        QString("Error reading file: %1").arg(fileName));
}

void djvImageIo::throwErrorWrite(
    const QString & plugin,
    const QString & fileName) throw (djvError)
{
    throw djvError(
        plugin,
        QString("Error writing file: %1").arg(fileName));
}

//------------------------------------------------------------------------------
// djvImageIoFactory::P
//------------------------------------------------------------------------------

struct djvImageIoFactory::P
{
    // This map is used to lookup an image I/O plugin by it's name.

    QMap<QString, djvImageIo *> nameMap;

    // This map is used to lookup an image I/O plugin for a given file
    // extension.

    QMap<QString, djvImageIo *> extensionMap;
};

//------------------------------------------------------------------------------
// djvImageIoFactory
//------------------------------------------------------------------------------

djvImageIoFactory::djvImageIoFactory(const QStringList & searchPath, QObject * parent)
    throw (djvError) :
    djvPluginFactory(searchPath, "djvImageIo", "djv", "Plugin", parent),
    _p(new P)
{
    //DJV_DEBUG("djvImageIoFactory::djvImageIoFactory");

    // Setup maps.

    const QList<djvPlugin *> & plugins = this->plugins();

    Q_FOREACH(djvPlugin * plugin, plugins)
    {
        if (djvImageIo * imageIo = dynamic_cast<djvImageIo *>(plugin))
        {
            //DJV_DEBUG_PRINT("imageIo = " << imageIo->pluginName());
            //DJV_DEBUG_PRINT("extensions = " << imageIo->extensions());

            _p->nameMap[plugin->pluginName().toLower()] = imageIo;

            Q_FOREACH(const QString & extension, imageIo->extensions())
            {
                _p->extensionMap[extension.toLower()] = imageIo;
            }
        }
    }

    // Setup callbacks for image I/O plugins.

    for (int i = 0; i < plugins.count(); ++i)
    {
        if (djvImageIo * plugin = dynamic_cast<djvImageIo *>(plugins[i]))
        {
            // This callback listens to option changes in the image I/O
            // plugins.

            connect(
                plugin,
                SIGNAL(optionChanged(const QString &)),
                SLOT(pluginOptionCallback(const QString &)));
        }
    }
}

djvImageIoFactory::~djvImageIoFactory()
{
    //DJV_DEBUG("djvImageIoFactory::~djvImageIoFactory");

    delete _p;
}

QStringList djvImageIoFactory::option(
    const QString & name,
    const QString & option) const
{
    const QString nameLower = name.toLower();
    
    if (_p->nameMap.contains(nameLower))
    {
        //DJV_DEBUG("djvImageIoFactory::option");
        //DJV_DEBUG_PRINT("name   = " << name);
        //DJV_DEBUG_PRINT("option = " << option);

        djvImageIo * imageIo = _p->nameMap[nameLower];

        return imageIo->option(option);
    }

    return QStringList();
}

bool djvImageIoFactory::setOption(
    const QString & name,
    const QString & option,
    QStringList &   data)
{
    const QString nameLower = name.toLower();
    
    if (_p->nameMap.contains(nameLower))
    {
        //DJV_DEBUG("djvImageIoFactory::setOption");
        //DJV_DEBUG_PRINT("name   = " << name);
        //DJV_DEBUG_PRINT("option = " << option);
        //DJV_DEBUG_PRINT("data   = " << data);

        djvImageIo * imageIo = _p->nameMap[nameLower];

        return imageIo->setOption(option, data);
    }

    return false;
}

djvImageLoad * djvImageIoFactory::load(
    const djvFileInfo & fileInfo,
    djvImageIoInfo &    imageIoInfo) const throw (djvError)
{
    //DJV_DEBUG("djvImageIoFactory::load");
    //DJV_DEBUG_PRINT("fileInfo = " << fileInfo);

    const QString extensionLower = fileInfo.extension().toLower();

    if (_p->extensionMap.contains(extensionLower))
    {
        djvImageIo * imageIo = _p->extensionMap[extensionLower];

        if (djvImageLoad * imageLoad = imageIo->createLoad())
        {
            imageLoad->open(fileInfo, imageIoInfo);

            return imageLoad;
        }
    }
    else
    {
        throw djvError(QString("Unrecognized file: %1").arg(fileInfo));
    }
    
    return 0;
}
    
djvImageSave * djvImageIoFactory::save(
    const djvFileInfo &    fileInfo,
    const djvImageIoInfo & imageIoInfo) const throw (djvError)
{
    //DJV_DEBUG("djvImageIoFactory::save");
    //DJV_DEBUG_PRINT("fileInfo = " << fileInfo);
    //DJV_DEBUG_PRINT("imageIoInfo = " << imageIoInfp);

    const QString extensionLower = fileInfo.extension().toLower();

    if (_p->extensionMap.contains(extensionLower))
    {
        djvImageIo * imageIo = _p->extensionMap[extensionLower];

        if (djvImageSave * imageSave = imageIo->createSave())
        {
            imageSave->open(fileInfo, imageIoInfo);

            return imageSave;
        }
    }
    else
    {
        throw djvError(QString("Unrecognized file: %1").arg(fileInfo));
    }
    
    return 0;
}

djvImageIoFactory * djvImageIoFactory::global()
{
    static QPointer<djvImageIoFactory> factory;

    if (! factory.data())
    {
        //! \todo Parenting the image I/O factory to the application is
        //! causing issues when the application exits on Windows.

        //factory = new djvImageIoFactory(djvSystem::searchPath(), qApp);
        factory = new djvImageIoFactory;
    }

    return factory.data();
}

void djvImageIoFactory::pluginOptionCallback(const QString &)
{
    Q_EMIT optionChanged();
}

//------------------------------------------------------------------------------

bool operator == (const djvImageIoInfo & a, const djvImageIoInfo & b)
{
    if (a.layerCount() != b.layerCount())
        return false;

    for (int i = 0; i < a.layerCount(); ++i)
        if (a[i] != b[i])
            return false;

    return
        operator == (
            static_cast<const djvPixelDataInfo &>(a),
            static_cast<const djvPixelDataInfo &>(b)) &&
        a.tags     == b.tags &&
        a.sequence == b.sequence;
}

bool operator != (const djvImageIoInfo & a, const djvImageIoInfo & b)
{
    return ! (a == b);
}

bool operator == (const djvImageIoFrameInfo & a, const djvImageIoFrameInfo & b)
{
    return
        a.frame == b.frame &&
        a.layer == b.layer &&
        a.proxy == b.proxy;
}

bool operator != (const djvImageIoFrameInfo & a, const djvImageIoFrameInfo & b)
{
    return ! (a == b);
}

djvDebug & operator << (djvDebug & debug, const djvImageIoInfo & in)
{
    return operator << (debug, static_cast<const djvPixelDataInfo &>(in));
}

djvDebug & operator << (djvDebug & debug, const djvImageIoFrameInfo & in)
{
    return debug << in.frame;
}