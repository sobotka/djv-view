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

//! \file djvCglContextPrivate.cpp

#include <djvCglContextPrivate.h>

#include <djvDebug.h>

//------------------------------------------------------------------------------
// PixelFormat
//------------------------------------------------------------------------------

namespace
{

class PixelFormat
{
public:

    PixelFormat() throw (djvError) :
        _format(0)
    {
        CGLPixelFormatAttribute attribs [] =
        {
            //kCGLPFAColorSize, (CGLPixelFormatAttribute)32,
            //kCGLPFADoubleBuffer,
            kCGLPFAAccelerated,
            (CGLPixelFormatAttribute)0
        };

        GLint npix = 0;

        CGLError error = CGLChoosePixelFormat(attribs, &_format, &npix);

        if (error != kCGLNoError)
            throw djvError(
                "djvCglContextPrivate",
                QString("Cannot get CGL pixel format: #%1").arg(error));
    }

    ~PixelFormat()
    {
        if (_format)
        {
            CGLDestroyPixelFormat(_format);
        }
    }

    CGLPixelFormatObj format() const
    {
        return _format;
    }

private:

    CGLPixelFormatObj _format;
};

} // namespace

//------------------------------------------------------------------------------
// djvCglContextPrivate
//------------------------------------------------------------------------------

djvCglContextPrivate::djvCglContextPrivate() throw (djvError) :
    _context(0)
{
    //DJV_DEBUG("djvCglContextPrivate::djvCglContextPrivate");

    // Create the pixel format.

    PixelFormat pixelFormat;

    // Create the context.

    CGLError error = CGLCreateContext(pixelFormat.format(), 0, &_context);

    if (error != kCGLNoError || ! _context)
        throw djvError(
            "djvCglContextPrivate",
            QString("Cannot create OpenGL context: #%1").arg(error));

    // Bind the context.

    bind();

    // Initialize GLEW.

    GLint glError = glewInit();

    if (glError != GLEW_OK)
        throw djvError(
            "djvCglContextPrivate",
            QString("Cannot initialize GLEW: #%1").arg(glError));

    setVendor  ((const char *)glGetString(GL_VENDOR));
    setRenderer((const char *)glGetString(GL_RENDERER));
    setVersion ((const char *)glGetString(GL_VERSION));

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
            "djvCglContextPrivate",
            "No OpenGL FBO support");
}

djvCglContextPrivate::~djvCglContextPrivate()
{
    //DJV_DEBUG("djvCglContextPrivate::~djvCglContextPrivate");

    if (_context)
    {
        //DJV_DEBUG_PRINT("context");

        CGLDestroyContext(_context);
    }
}

void djvCglContextPrivate::bind() throw (djvError)
{
    if (! _context)
    {
        throw djvError("djvCglContextPrivate", "Invalid OpenGL context");
    }

    //DJV_DEBUG("djvCglContextPrivate::bind");

    CGLError error = CGLSetCurrentContext(_context);

    if (error != kCGLNoError)
        throw djvError(
            "djvCglContextPrivate",
            QString("Cannot bind OpenGL context: #%1").arg(error));
}

void djvCglContextPrivate::unbind()
{
    //DJV_DEBUG("djvCglContextPrivate::unbind");

    CGLSetCurrentContext(0);
}