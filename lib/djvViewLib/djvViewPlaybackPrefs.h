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

//! \file djvViewPlaybackPrefs.h

#ifndef DJV_VIEW_PLAYBACK_PREFS_H
#define DJV_VIEW_PLAYBACK_PREFS_H

#include <djvView.h>
#include <djvViewAbstractPrefs.h>

//! \addtogroup djvViewPlayback
//@{

//------------------------------------------------------------------------------
//! \class djvViewPlaybackPrefs
//!
//! This class provides the playback group preferences.
//------------------------------------------------------------------------------

class DJV_VIEW_LIB_EXPORT djvViewPlaybackPrefs : public djvViewAbstractPrefs
{
    Q_OBJECT
    
public:

    //! Constructor.

    explicit djvViewPlaybackPrefs(QObject * parent = 0);

    //! Destructor.

    virtual ~djvViewPlaybackPrefs();
    
    //! Get the default for whether playback is started automatically.
    
    static bool autoStartDefault();

    //! Get whether playback is started automatically.

    bool hasAutoStart() const;

    //! Get the default playback loop.
    
    static djvView::LOOP loopDefault();

    //! Get the playback loop.

    djvView::LOOP loop() const;

    //! Get the default for whether every frame is played.
    
    static bool everyFrameDefault();

    //! Get whether every frame is played.

    bool hasEveryFrame() const;

    //! Get the default playback layout.
    
    static djvView::LAYOUT layoutDefault();

    //! Get the playback layout.

    djvView::LAYOUT layout() const;

    //! Get the global preferences.

    static djvViewPlaybackPrefs * global();

public Q_SLOTS:

    //! Set whether playback is started automatically.

    void setAutoStart(bool);
    
    //! Set the playback loop.

    void setLoop(djvView::LOOP);

    //! Set whether every frame is played.

    void setEveryFrame(bool);

    //! Set the playback layout.

    void setLayout(djvView::LAYOUT);

Q_SIGNALS:

    //! This signal is emitted when playback auto start is changed.

    void autoStartChanged(bool);

    //! This signal is emitted when the playback loop is changed.

    void loopChanged(djvView::LOOP);
    
    //! This signal is emitted when every frame played is changed.

    void everyFrameChanged(bool);
    
    //! This signal is emitted when the playback layout is changed.

    void layoutChanged(djvView::LAYOUT);

    //! This signal is emitted when a preference is changed.

    void prefChanged();

private:

    bool            _autoStart;
    djvView::LOOP   _loop;
    bool            _everyFrame;
    djvView::LAYOUT _layout;
};

//@} // djvViewPlayback

#endif // DJV_VIEW_PLAYBACK_PREFS_H
