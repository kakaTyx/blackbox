// -*- mode: C++; indent-tabs-mode: nil; c-basic-offset: 2; -*-
// Texture.hh for Blackbox - an X11 Window manager
// Copyright (c) 2001 - 2002 Sean 'Shaleh' Perry <shaleh at debian.org>
// Copyright (c) 1997 - 2000, 2002 Bradley T Hughes <bhughes at trolltech.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef TEXTURE_HH
#define TEXTURE_HH

#include "Color.hh"

#include <string>

typedef unsigned long Pixmap;


namespace bt {

  class ImageControl;
  class Resource;

  class Texture {
  public:
    enum Type {
      // bevel options
      Flat                = (1l<<0),
      Sunken              = (1l<<1),
      Raised              = (1l<<2),
      // textures
      Solid               = (1l<<3),
      Gradient            = (1l<<4),
      // gradients
      Horizontal          = (1l<<5),
      Vertical            = (1l<<6),
      Diagonal            = (1l<<7),
      CrossDiagonal       = (1l<<8),
      Rectangle           = (1l<<9),
      Pyramid             = (1l<<10),
      PipeCross           = (1l<<11),
      Elliptic            = (1l<<12),
      // inverted image
      Invert              = (1l<<14),
      // parent relative image
      Parent_Relative     = (1l<<15),
      // fake interlaced image
      Interlaced          = (1l<<16),
      // border around image
      Border              = (1l<<17)
    };

    Texture(void);
    Texture(const Texture &tt);

    const std::string &description(void) const { return descr; }
    void setDescription(const std::string &d);

    void setColor(const Color &new_color);
    void setColorTo(const Color &new_colorTo) { ct = new_colorTo; }
    void setBorderColor(const Color &new_borderColor) { bc = new_borderColor; }

    const Color &color(void) const { return c; }
    const Color &colorTo(void) const { return ct; }
    const Color &borderColor(void) const { return bc; }
    const Color &lightColor(void) const { return lc; }
    const Color &shadowColor(void) const { return sc; }

    unsigned long texture(void) const { return t; }
    void setTexture(unsigned long _texture) { t  = _texture; }
    void addTexture(unsigned long _texture) { t |= _texture; }

    unsigned int borderWidth(void) const { return bw; }
    void setBorderWidth(unsigned int new_bw) { bw = new_bw; }

    Texture &operator=(const Texture &tt);
    bool operator==(const Texture &tt)
    { return (c == tt.c && ct == tt.ct && bc == tt.bc &&
              lc == tt.lc && sc == tt.sc && t == tt.t && bw == tt.bw); }
    bool operator!=(const Texture &tt)
    { return (! operator==(tt)); }

    Pixmap render(const Display &display, unsigned int screen,
                  ImageControl &image_control, // this needs to go away
                  unsigned int width,  unsigned int height,
                  Pixmap old = 0);

  private:
    std::string descr;
    Color c, ct, bc, lc, sc;
    unsigned long t;
    unsigned int bw;
  };

  Texture
  textureResource(const Display &display, unsigned int screen,
                  const Resource &resource,
                  const std::string &name,
                  const std::string &classname,
                  const std::string &default_color = std::string("black"));

} // namespace bt

#endif // TEXTURE_HH