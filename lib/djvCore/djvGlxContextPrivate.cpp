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

//! \file djvGlxContextPrivate.cpp

#include <djvGlxContextPrivate.h>

#include <djvDebug.h>

//------------------------------------------------------------------------------
// djvGlxContextPrivate
//------------------------------------------------------------------------------

djvGlxContextPrivate::djvGlxContextPrivate() throw (djvError) :
    _display     (0),
    _screen      (0),
    _visuals     (0),
    _visualsCount(0),
    _colormap    (0),
    _window      (0),
    _context     (0)
{
    //DJV_DEBUG("djvGlxContextPrivate::djvGlxContextPrivate");

    // Open the X display.

    _display = XOpenDisplay(NULL);

    if (! _display)
        throw djvError(
            "djvGlxContextPrivate",
            "Cannot open X display");

    _screen = DefaultScreen(_display);

    // Choose a visual.

    XVisualInfo visualInfo;
    visualInfo.screen = _screen;
    visualInfo.depth  = 32;

    _visuals = XGetVisualInfo(
        _display,
        VisualScreenMask | VisualDepthMask,
        &visualInfo,
        &_visualsCount);

    if (! _visuals || ! _visualsCount)
        throw djvError(
            "djvGlxContextPrivate",
            "No appropriate X visuals");

    // Create the color map.

    _colormap = XCreateColormap(
        _display,
        RootWindow(_display, _screen),
        _visuals[0].visual,
        AllocNone);

    if (! _colormap)
        throw djvError(
            "djvGlxContextPrivate",
            "Cannot create X colormap");

    // Check for GLX support.

    if (! glXQueryExtension(_display, 0, 0))
        throw djvError(
            "djvGlxContextPrivate",
            "No GLX extension");

    // Create a dummy window and rendering context for glewInit.

    XSetWindowAttributes winAttrib;
    winAttrib.colormap     = _colormap;
    winAttrib.border_pixel = 0;

    _window = XCreateWindow(
        _display,
        RootWindow(_display, _screen),
        0, 0, 1, 1, 0,
        visualInfo.depth,
        InputOutput,
        _visuals[0].visual,
        CWColormap | CWBorderPixel,
        &winAttrib);

    if (! _window)
        throw djvError(
            "djvGlxContextPrivate",
            "Cannot create an X window");

    // Create the OpenGL context.

    _context = glXCreateContext(
        _display,
        &_visuals[0],
        0,
        True);

    if (! _context)
        throw djvError(
            "djvGlxContextPrivate",
            "Cannot create OpenGL context");

    // Bind the context.

    bind();

    // Initialize GLEW.

    GLint glError = glewInit();

    if (glError != GLEW_OK)
        throw djvError(
            "djvGlxContextPrivate",
            QString("Cannot initialize GLEW: #%1").arg(glError));

    setVendor((const char *)glGetString(GL_VENDOR));
    setRenderer((const char *)glGetString(GL_RENDERER));
    setVersion((const char *)glGetString(GL_VERSION));

    //DJV_DEBUG_PRINT("vendor string = " << vendor());
    //DJV_DEBUG_PRINT("renderer string = " << renderer());
    //DJV_DEBUG_PRINT("version string = " << version());
    //DJV_DEBUG_PRINT("extensions = " <<
    //    (const char *)glGetString(GL_EXTENSIONS));
    //DJV_DEBUG_PRINT("glu version = " <<
    //    (const char *)gluGetString(GLU_VERSION));
    //DJV_DEBUG_PRINT("glu extensions = " <<
    //    (const char *)gluGetString(GLU_EXTENSIONS));

    if (! GL_EXT_framebuffer_object)
        throw djvError(
            "djvGlxContextPrivate",
            "No OpenGL FBO support");
}

djvGlxContextPrivate::~djvGlxContextPrivate()
{
    //DJV_DEBUG("djvGlxContextPrivate::~djvGlxContextPrivate");

    if (_context)
    {
        //DJV_DEBUG_PRINT("context");

        glXDestroyContext(_display, _context);
    }

    if (_window)
    {
        //DJV_DEBUG_PRINT("window");

        XDestroyWindow(_display, _window);
    }

    if (_colormap)
    {
        XFreeColormap(_display, _colormap);
    }

    if (_visuals)
    {
        XFree(_visuals);
    }

    if (_display)
    {
        //DJV_DEBUG_PRINT("display");

        XCloseDisplay(_display);
    }
}

void djvGlxContextPrivate::bind() throw (djvError)
{
    if (! _context)
        throw djvError(
            "djvGlxContextPrivate",
            "Invalid OpenGL context");

    //DJV_DEBUG("djvGlxContextPrivate::bind");

    if (! glXMakeCurrent(_display, _window, _context))
        throw djvError(
            "djvGlxContextPrivate",
            "Cannot bind OpenGL context");
}

void djvGlxContextPrivate::unbind()
{
    //DJV_DEBUG("djvGlxContextPrivate::unbind");

    glXMakeCurrent(_display, 0, 0);
}
