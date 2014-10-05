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

//! \file djvViewDisplayProfile.h

#ifndef DJV_VIEW_DISPLAY_PROFILE_H
#define DJV_VIEW_DISPLAY_PROFILE_H

#include <djvViewLibExport.h>

#include <djvFileInfo.h>
#include <djvOpenGlImage.h>

//! \addtogroup djvViewImage
//@{

//------------------------------------------------------------------------------
//! \struct djvViewDisplayProfile
//!
//! This struct provides a display profile.
//------------------------------------------------------------------------------

struct DJV_VIEW_LIB_EXPORT djvViewDisplayProfile :
    public djvOpenGlImageDisplayProfile
{
    QString     name;
    djvFileInfo lutFile;
};

//------------------------------------------------------------------------------

#if defined(DJV_WINDOWS)

DJV_VIEW_LIB_EXPORT_TEMPLATE
template class DJV_VIEW_LIB_EXPORT QVector<djvViewDisplayProfile>;

#endif // DJV_WINDOWS

DJV_VIEW_LIB_EXPORT bool operator == (
    const djvViewDisplayProfile &,
    const djvViewDisplayProfile &);

DJV_VIEW_LIB_EXPORT bool operator != (
    const djvViewDisplayProfile &,
    const djvViewDisplayProfile &);

DJV_VIEW_LIB_EXPORT QStringList & operator >> (QStringList &,
    djvViewDisplayProfile &) throw (QString);

DJV_VIEW_LIB_EXPORT QStringList & operator << (QStringList &,
    const djvViewDisplayProfile &);

DJV_VIEW_LIB_EXPORT djvDebug & operator << (djvDebug &,
    const djvViewDisplayProfile &);

//@} // djvViewImage

#endif // DJV_VIEW_DISPLAY_PROFILE_H
