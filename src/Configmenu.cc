// -*- mode: C++; indent-tabs-mode: nil; c-basic-offset: 2; -*-
// Configmenu.cc for Blackbox - An X11 Window Manager
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

#ifdef    HAVE_CONFIG_H
# include "../config.h"
#endif // HAVE_CONFIG_H

#include "i18n.hh"
#include "Configmenu.hh"

#include "Image.hh"
#include "Toolbar.hh"
#include "Window.hh"
#include "Screen.hh"
#include "blackbox.hh"
#include "i18n.hh"

class ConfigPlacementmenu : public Basemenu
{
public:
  ConfigPlacementmenu(int scr)
    : Basemenu(scr)
  {
    insert(i18n(ConfigmenuSet, ConfigmenuSmartRows,
                "Smart Placement (Rows)"),
           BScreen::SmartRow);
    insert(i18n(ConfigmenuSet, ConfigmenuSmartCols,
                "Smart Placement (Columns)"),
           BScreen::SmartColumn);
    insert(i18n(ConfigmenuSet, ConfigmenuCascade,
                "Cascade Placement"),
           BScreen::Cascade);

    BScreen *screen = Blackbox::instance()->screen(screenNumber());
    switch (screen->windowPlacement()) {
    case BScreen::SmartRow:
      setItemChecked(0, True);
      break;

    case BScreen::SmartColumn:
      setItemChecked(1, True);
      break;

    case BScreen::Cascade:
      setItemChecked(2, True);
      break;
    }

  }

protected:
  virtual void itemClicked(const Item & item, int button)
  {
    if (button != 1 || ! item.function())
      return;

    BScreen *screen = Blackbox::instance()->screen(screenNumber());
    switch (item.function()) {
    case BScreen::SmartRow:
      screen->setWindowPlacement((BScreen::WindowPlacement) item.function());
      setItemChecked(0, True);
      setItemChecked(1, False);
      setItemChecked(2, False);
      break;

    case BScreen::SmartColumn:
      screen->setWindowPlacement((BScreen::WindowPlacement) item.function());
      setItemChecked(0, False);
      setItemChecked(1, True);
      setItemChecked(2, False);
      break;

    case BScreen::Cascade:
      screen->setWindowPlacement((BScreen::WindowPlacement) item.function());
      setItemChecked(0, False);
      setItemChecked(1, False);
      setItemChecked(2, True);
      break;
    }
  }
};

class ConfigFocusmenu : public Basemenu
{
public:
  ConfigFocusmenu(int scrn)
    : Basemenu(scrn)
  {
    insert(i18n(ConfigmenuSet, ConfigmenuClickToFocus,
                "Click To Focus"), 1);
    insert(i18n(ConfigmenuSet, ConfigmenuSloppyFocus,
                "Sloppy Focus"), 2);
    insert(i18n(ConfigmenuSet, ConfigmenuAutoRaise,
                "Auto Raise"), 3);

    BScreen *screen = Blackbox::instance()->screen(screenNumber());
    setItemChecked(0, (! screen->isSloppyFocus()));
    setItemChecked(1, screen->isSloppyFocus());
    setItemEnabled(2, screen->isSloppyFocus());
    setItemChecked(2, screen->doAutoRaise());
  }

protected:
  virtual void itemClicked(const Item & item, int button)
  {
    if (button != 1 || ! item.function())
      return;

    BScreen *screen = Blackbox::instance()->screen(screenNumber());
    switch (item.function()) {
    case 1: // click to focus
      screen->saveSloppyFocus(False);
      screen->saveAutoRaise(False);
      hideAll();
      screen->reconfigure();
      break;

    case 2: // sloppy focus
      screen->saveSloppyFocus(True);
      screen->reconfigure();
      break;

    case 3: // auto raise with sloppy focus
      Bool change = ((screen->doAutoRaise()) ? False : True);
      screen->saveAutoRaise(change);
      break;
    }

    setItemChecked(0, (! screen->isSloppyFocus()));
    setItemChecked(1, screen->isSloppyFocus());
    setItemEnabled(2, screen->isSloppyFocus());
    setItemChecked(2, screen->doAutoRaise());
  }
};

Configmenu::Configmenu(int scr)
    : Basemenu(scr)
{
  setAutoDelete(false);

  setTitle(i18n(ConfigmenuSet, ConfigmenuConfigOptions,
                "Config options"));
  showTitle();

  insert(i18n(ConfigmenuSet, ConfigmenuFocusModel,
              "Focus Model"),
         new ConfigFocusmenu(scr));
  insert(i18n(ConfigmenuSet, ConfigmenuWindowPlacement,
              "Window Placement"),
         new ConfigPlacementmenu(scr));
  insert(i18n(ConfigmenuSet, ConfigmenuImageDithering,
              "Image Dithering"), 1);
  insert(i18n(ConfigmenuSet, ConfigmenuOpaqueMove,
              "Opaque Window Moving"), 2);
  insert(i18n(ConfigmenuSet, ConfigmenuFullMax,
              "Full Maximization"), 3);
  insert(i18n(ConfigmenuSet, ConfigmenuFocusNew,
              "Focus New Windows"), 4);
  insert(i18n(ConfigmenuSet, ConfigmenuFocusLast,
              "Focus Last Window on Workspace"), 5);

  BScreen *screen = Blackbox::instance()->screen(screenNumber());
  setItemChecked(2, screen->getImageControl()->doDither());
  setItemChecked(3, screen->doOpaqueMove());
  setItemChecked(4, screen->doFullMax());
  setItemChecked(5, screen->doFocusNew());
  setItemChecked(6, screen->doFocusLast());
}

void Configmenu::itemClicked(const Item &item, int button)
{
  if (button != 1 || ! item.function())
    return;

  BScreen *screen = Blackbox::instance()->screen(screenNumber());
  switch(item.function()) {
  case 1: { // dither
    screen->getImageControl()->
      setDither((! screen->getImageControl()->doDither()));

    setItemChecked(item.index(), screen->getImageControl()->doDither());

    break;
  }

  case 2: { // opaque move
    screen->saveOpaqueMove((! screen->doOpaqueMove()));

    setItemChecked(item.index(), screen->doOpaqueMove());

    break;
  }

  case 3: { // full maximization
    screen->saveFullMax((! screen->doFullMax()));

    setItemChecked(item.index(), screen->doFullMax());

    break;
  }
  case 4: { // focus new windows
    screen->saveFocusNew((! screen->doFocusNew()));

    setItemChecked(item.index(), screen->doFocusNew());
    break;
  }

  case 5: { // focus last window on workspace
    screen->saveFocusLast((! screen->doFocusLast()));
    setItemChecked(item.index(), screen->doFocusLast());
    break;
  }
  } // switch
}
