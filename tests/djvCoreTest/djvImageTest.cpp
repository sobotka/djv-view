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

//! \file djvImageTest.cpp

#include <djvImageTest.h>

#include <djvAssert.h>
#include <djvDebug.h>
#include <djvImage.h>

void djvImageTest::run(int &, char **)
{
    DJV_DEBUG("djvImageTest::run");
    
    ctors();
    operators();
}

void djvImageTest::ctors()
{
    DJV_DEBUG("djvImageTest::ctors");
    
    {
        const djvImage image;
        
        DJV_ASSERT(image.data() == 0);
    }
    
    {
        const djvImage image(djvPixelDataInfo(32, 32, djvPixel::RGBA_U8));
        
        djvImage other(image);
        
        DJV_ASSERT(other.info() == image.info());
    }
}

void djvImageTest::operators()
{
    DJV_DEBUG("djvImageTest::operators");
    
    {
        djvImage
            a(djvPixelDataInfo(1, 1, djvPixel::L_U8)),
            b(djvPixelDataInfo(1, 1, djvPixel::L_U8));
        
        a.data()[0] = b.data()[0] = 127;
        
        DJV_ASSERT(a == b);
        DJV_ASSERT(a != djvImage());
    }
    
    {
        DJV_DEBUG_PRINT(djvImage());
    }
}
