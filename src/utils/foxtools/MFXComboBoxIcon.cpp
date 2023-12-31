/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.dev/sumo
// Copyright (C) 2006-2023 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    MFXComboBoxIcon.cpp
/// @author  Jakob Erdmann
/// @author  Pablo Alvarez Lopez
/// @date    2018-12-19
///
//
/****************************************************************************/

// =========================================================================
// included modules
// =========================================================================

#include <utils/gui/windows/GUIAppEnum.h>
#include <utils/gui/images/GUIIconSubSys.h>

#include "MFXComboBoxIcon.h"

// =========================================================================
// defines
// =========================================================================

#define ICON_SPACING        4   // Spacing between icon and label (2 + 2)
#define ICON_SIZE           16

// ===========================================================================
// FOX callback mapping
// ===========================================================================

FXDEFMAP(MFXComboBoxIcon) MFXComboBoxIconMap[] = {
    FXMAPFUNC(SEL_FOCUS_UP,         0,                              MFXComboBoxIcon::onFocusUp),
    FXMAPFUNC(SEL_FOCUS_DOWN,       0,                              MFXComboBoxIcon::onFocusDown),
    FXMAPFUNC(SEL_FOCUS_SELF,       0,                              MFXComboBoxIcon::onFocusSelf),
    FXMAPFUNC(SEL_UPDATE,           MFXComboBoxIcon::ID_TEXT,       MFXComboBoxIcon::onUpdFmText),
    FXMAPFUNC(SEL_CLICKED,          MFXComboBoxIcon::ID_LIST,       MFXComboBoxIcon::onListClicked),
    FXMAPFUNC(SEL_COMMAND,          MFXComboBoxIcon::ID_LIST,       MFXComboBoxIcon::onListClicked),
    FXMAPFUNC(SEL_LEFTBUTTONPRESS,  MFXComboBoxIcon::ID_TEXT,       MFXComboBoxIcon::onTextButton),
    FXMAPFUNC(SEL_MOUSEWHEEL,       MFXComboBoxIcon::ID_TEXT,       MFXComboBoxIcon::onMouseWheel),
    FXMAPFUNC(SEL_CHANGED,          MFXComboBoxIcon::ID_TEXT,       MFXComboBoxIcon::onTextChanged),
    FXMAPFUNC(SEL_COMMAND,          MFXComboBoxIcon::ID_TEXT,       MFXComboBoxIcon::onTextCommand),
    FXMAPFUNC(SEL_COMMAND,          FXWindow::ID_SETVALUE,          MFXComboBoxIcon::onFwdToText),
    FXMAPFUNC(SEL_COMMAND,          FXWindow::ID_SETINTVALUE,       MFXComboBoxIcon::onFwdToText),
    FXMAPFUNC(SEL_COMMAND,          FXWindow::ID_SETREALVALUE,      MFXComboBoxIcon::onFwdToText),
    FXMAPFUNC(SEL_COMMAND,          FXWindow::ID_SETSTRINGVALUE,    MFXComboBoxIcon::onFwdToText),
    FXMAPFUNC(SEL_COMMAND,          FXWindow::ID_GETINTVALUE,       MFXComboBoxIcon::onFwdToText),
    FXMAPFUNC(SEL_COMMAND,          FXWindow::ID_GETREALVALUE,      MFXComboBoxIcon::onFwdToText),
    FXMAPFUNC(SEL_COMMAND,          FXWindow::ID_GETSTRINGVALUE,    MFXComboBoxIcon::onFwdToText),
    FXMAPFUNC(SEL_COMMAND,          MID_MTEXTFIELDSEARCH_UPDATED,   MFXComboBoxIcon::onCmdFilter),
};

// Object implementation
FXIMPLEMENT(MFXComboBoxIcon,    FXPacker,   MFXComboBoxIconMap, ARRAYNUMBER(MFXComboBoxIconMap))

// ===========================================================================
// member method definitions
// ===========================================================================

MFXComboBoxIcon::MFXComboBoxIcon(FXComposite* p, FXint cols, const bool haveIcons, const bool canSearch, 
        FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h, FXint pl, FXint pr, FXint pt, FXint pb):
    FXPacker(p, opts, x, y, w, h, 0, 0, 0, 0, 0, 0) {
    flags |= FLAG_ENABLED;
    target = tgt;
    message = sel;
    myIconLabel = new FXLabel(this, "", nullptr, 0, 0, 0, 0, 0, pl, pr, pt, pb);
    if (!haveIcons) {
        myIconLabel->hide();
    }
    myTextField = new FXTextField(this, cols, this, MFXComboBoxIcon::ID_TEXT, 0, 0, 0, 0, 0, pl, pr, pt, pb);
    if (options & COMBOBOX_STATIC) {
        myTextField->setEditable(FALSE);
    }
    myPane = new FXPopup(this, FRAME_LINE);
    // check if create search button
    if (canSearch) {
        myTextFieldSearch = new MFXTextFieldSearch(myPane, 1, this, ID_SEARCH, FRAME_THICK | LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 0, 0, 2, 2, 2, 2);
    }
    myList = new MFXListIcon(myPane, this, MFXComboBoxIcon::ID_LIST, LIST_BROWSESELECT | LIST_AUTOSELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y | SCROLLERS_TRACK | HSCROLLER_NEVER);
    if (options & COMBOBOX_STATIC) {
        myList->setScrollStyle(SCROLLERS_TRACK | HSCROLLING_OFF);
    }
    myButton = new FXMenuButton(this, FXString::null, NULL, myPane, FRAME_RAISED | FRAME_THICK | MENUBUTTON_DOWN | MENUBUTTON_ATTACH_RIGHT, 0, 0, 0, 0, 0, 0, 0, 0);
    myButton->setXOffset(border);
    myButton->setYOffset(border);
    flags &= ~FLAG_UPDATE;  // Never GUI update
}


MFXComboBoxIcon::~MFXComboBoxIcon() {
    delete myPane;
    myPane = (FXPopup*) - 1L;
    myIconLabel = (FXLabel*) - 1L;
    myTextField = (FXTextField*) - 1L;
    myButton = (FXMenuButton*) - 1L;
    if (myTextFieldSearch) {
        myTextFieldSearch = (MFXTextFieldSearch*) - 1L;
    }
    myList = (MFXListIcon*) - 1L;
}


void
MFXComboBoxIcon::create() {
    FXPacker::create();
    myPane->create();
}


void
MFXComboBoxIcon::detach() {
    FXPacker::detach();
    myPane->detach();
}


void
MFXComboBoxIcon::destroy() {
    myPane->destroy();
    FXPacker::destroy();
}


void
MFXComboBoxIcon::enable() {
    if (!isEnabled()) {
        FXPacker::enable();
        myIconLabel->enable();
        myTextField->enable();
        myButton->enable();
    }
}


void
MFXComboBoxIcon::disable() {
    if (isEnabled()) {
        FXPacker::disable();
        myIconLabel->disable();
        myTextField->disable();
        myButton->disable();
    }
}


FXint
MFXComboBoxIcon::getDefaultWidth() {
    FXint ww, pw;
    if (myIconLabel->shown()) {
        ww = myIconLabel->getDefaultWidth() + myTextField->getDefaultWidth() + myButton->getDefaultWidth() + (border << 1);
    } else {
        ww = myTextField->getDefaultWidth() + myButton->getDefaultWidth() + (border << 1);
    }
    pw = myPane->getDefaultWidth();
    return FXMAX(ww, pw);
}


FXint
MFXComboBoxIcon::getDefaultHeight() {
    FXint th, bh;
    th = myTextField->getDefaultHeight();
    bh = myButton->getDefaultHeight();
    return FXMAX(th, bh) + (border << 1);
}


void
MFXComboBoxIcon::layout() {
    const FXint itemHeight = height - (border << 1);
    const FXint iconSize = myIconLabel->shown()? itemHeight : 0;
    const FXint buttonWidth = myButton->getDefaultWidth();
    const FXint textWidth = width - buttonWidth - iconSize - (border << 1);
    myIconLabel->position(border, border, iconSize, iconSize);
    myTextField->position(border + iconSize, border, textWidth, itemHeight);
    myButton->position(border + textWidth + iconSize, border, buttonWidth, itemHeight);
    if(myTextFieldSearch) {
        myTextFieldSearch->resize(width, height);
    }
    myPane->resize(width, myPane->getDefaultHeight());
    flags &= ~FLAG_DIRTY;
}


FXString
MFXComboBoxIcon::getText() const {
    return myTextField->getText();
}


FXint
MFXComboBoxIcon::getNumItems() const {
    return myList->getNumItems();
}


FXint
MFXComboBoxIcon::getNumVisible() const {
    return myList->getNumVisible();
}


void
MFXComboBoxIcon::setNumVisible(FXint nvis) {
    myList->setNumVisible(nvis);
}


FXbool
MFXComboBoxIcon::isItemCurrent(FXint index) const {
    return myList->isItemCurrent(index);
}


long
MFXComboBoxIcon::setCurrentItem(const FXint index, FXbool notify) {
    if (index >= 0 && index <= myList->getNumItems()) {
        // get item
        MFXListIconItem* item = myList->getItem(index);
        // set it as current item and make it visible
        myList->setCurrentItem(item);
        myList->makeItemVisible(index);
        // update both text fields
        myTextField->setText(item->getText());
        myIconLabel->setIcon(item->getIcon());
        myTextField->setBackColor(item->getBackGroundColor());
        myIconLabel->setBackColor(item->getBackGroundColor());
        // check if notify
        if (notify && target) {
            return target->tryHandle(this, FXSEL(SEL_COMMAND, message), (void*)item);
        }
    } else {
        fxerror("%s::setItem: index out of range.\n", getClassName());
    }
    return 0;
}


long
MFXComboBoxIcon::setCurrentItem(const FXString& text, FXbool notify) {
    // check if item exist
    for (int i = 0; i < myList->getNumItems(); i++) {
        const auto itemText = myList->tolowerString(myList->getItem(i)->getText());
        if (myList->tolowerString(text) == itemText) {
            // use "set curent item" function
            return setCurrentItem(i, notify);
        }
    }
    return 0;
}


FXint
MFXComboBoxIcon::getCurrentItem() const {
    return myList->getCurrentItemIndex();
}


FXString
MFXComboBoxIcon::getItem(FXint index) const {
    return myList->getItem(index)->getText();
}


FXint
MFXComboBoxIcon::updateIconItem(FXint index, const FXString& text, FXIcon* icon, FXColor bgColor, void* ptr) {
    if (index < 0 || myList->getNumItems() <= index) {
        fxerror("%s::setItem: index out of range.\n", getClassName());
    }
    myList->editItem(index, text, NULL, ptr);
    if (isItemCurrent(index)) {
        myTextField->setText(text);
        myTextField->setBackColor(bgColor);
        myIconLabel->setIcon(icon);
        myIconLabel->setBackColor(bgColor);
    }
    recalc();
    return index;
}


FXint
MFXComboBoxIcon::insertIconItem(FXint index, const FXString& text, FXIcon* icon, FXColor bgColor, void* ptr) {
    index = myList->insertItem(index, new MFXListIconItem(text, icon, bgColor, ptr));
    if (isItemCurrent(index)) {
        myTextField->setText(text);
        myTextField->setBackColor(bgColor);
        myIconLabel->setIcon(icon);
        myIconLabel->setBackColor(bgColor);
    }
    recalc();
    return index;
}


FXint
MFXComboBoxIcon::appendIconItem(const FXString& text, FXIcon* icon, FXColor bgColor, void* ptr) {
    FXint index = myList->appendItem(new MFXListIconItem(text, icon, bgColor, ptr));
    if (isItemCurrent(getNumItems() - 1)) {
        myTextField->setText(text);
        myTextField->setBackColor(bgColor);
        myIconLabel->setIcon(icon);
        myIconLabel->setBackColor(bgColor);
    }
    recalc();
    return index;
}


void
MFXComboBoxIcon::setCustomText(const FXString text) {
    myTextField->setText(text);
    myTextField->setTextColor(FXRGB(128, 128, 128));
}


void
MFXComboBoxIcon::clearItems() {
    myTextField->setText("");
    myList->clearItems();
    recalc();
}


FXint
MFXComboBoxIcon::findItem(const FXString& text) const {
    return myList->findItem(text);
}


FXString
MFXComboBoxIcon::getItemText(FXint index) const {
    return myList->getItem(index)->getText();
}


FXbool
MFXComboBoxIcon::isPaneShown() const {
    return myPane->shown();
}


FXFont*
MFXComboBoxIcon::getFont() const {
    return myTextField->getFont();
}


void
MFXComboBoxIcon::setBackColor(FXColor clr) {
    myTextField->setBackColor(clr);
    myIconLabel->setBackColor(clr);
    myList->setBackColor(clr);
}


FXColor
MFXComboBoxIcon::getBackColor() const {
    return myTextField->getBackColor();
}


void
MFXComboBoxIcon::setTextColor(FXColor clr) {
    myTextField->setTextColor(clr);
    myList->setTextColor(clr);
}


FXColor
MFXComboBoxIcon::getTextColor() const {
    return myTextField->getTextColor();
}


FXColor
MFXComboBoxIcon::getSelBackColor() const {
    return myTextField->getSelBackColor();
}


FXColor
MFXComboBoxIcon::getSelTextColor() const {
    return myTextField->getSelTextColor();
}


void
MFXComboBoxIcon::setHelpText(const FXString& txt) {
    myTextField->setHelpText(txt);
}


const FXString&
MFXComboBoxIcon::getHelpText() const {
    return myTextField->getHelpText();
}


void
MFXComboBoxIcon::setTipText(const FXString& txt) {
    myTextField->setTipText(txt);
}


const FXString&
MFXComboBoxIcon::getTipText() const {
    return myTextField->getTipText();
}


long
MFXComboBoxIcon::onUpdFmText(FXObject*, FXSelector, void*) {
    return (target && !isPaneShown())? target->tryHandle(this, FXSEL(SEL_UPDATE, message), NULL) : 0;
}


long
MFXComboBoxIcon::onCmdFilter(FXObject*, FXSelector, void* ptr) {
    myList->setFilter(myTextFieldSearch->getText());
    myPane->resize(width, myPane->getDefaultHeight());
    myPane->recalc();
    myPane->onPaint(0, 0, ptr);
    return 1;
}


long
MFXComboBoxIcon::onFwdToText(FXObject* sender, FXSelector sel, void* ptr) {
    return myTextField->handle(sender, sel, ptr);
}


long
MFXComboBoxIcon::onListClicked(FXObject*, FXSelector sel, void* ptr) {
    // hide pane
    myButton->handle(this, FXSEL(SEL_COMMAND, ID_UNPOST), NULL);
    if (FXSELTYPE(sel) == SEL_COMMAND) {
        // cast MFXListIconItem
        const MFXListIconItem* item = (MFXListIconItem*)ptr;
        // set icon and background color
        myTextField->setText(item->getText());
        myTextField->setBackColor(item->getBackGroundColor());
        myIconLabel->setIcon(item->getIcon());
        myIconLabel->setBackColor(item->getBackGroundColor());
        // Select if editable
        if (!(options & COMBOBOX_STATIC)) {
            myTextField->selectAll();
        }
        if (target) {
            target->tryHandle(this, FXSEL(SEL_COMMAND, message), (void*)getText().text());
        }
    }
    return 1;
}


long
MFXComboBoxIcon::onTextButton(FXObject*, FXSelector, void*) {
    if (options & COMBOBOX_STATIC) {
        // Post the myList
        myButton->handle(this, FXSEL(SEL_COMMAND, ID_POST), NULL);
        return 1;
    }
    return 0;
}


long
MFXComboBoxIcon::onTextChanged(FXObject*, FXSelector, void* ptr) {
    return target? target->tryHandle(this, FXSEL(SEL_CHANGED, message), ptr) : 0;
}


long
MFXComboBoxIcon::onTextCommand(FXObject*, FXSelector, void* ptr) {
    // reset background colors
    myTextField->setBackColor(FXRGB(255, 255, 255));
    myIconLabel->setBackColor(FXRGB(255, 255, 255));
    // check if item exist
    for (int i = 0; i < myList->getNumItems(); i++) {
        const auto itemText = myList->tolowerString(myList->getItem(i)->getText());
        if (myList->tolowerString(myTextField->getText()) == itemText) {
            // use "set curent item" function
            return setCurrentItem(i, TRUE);
        }
    }
    // no item found, then reset icon label
    myIconLabel->setIcon(GUIIconSubSys::getIcon(GUIIcon::CANCEL));
    return target? target->tryHandle(this, FXSEL(SEL_COMMAND, message), ptr) : 0;
}


long
MFXComboBoxIcon::onFocusSelf(FXObject* sender, FXSelector, void* ptr) {
    return myTextField->handle(sender, FXSEL(SEL_FOCUS_SELF, 0), ptr);
}


long
MFXComboBoxIcon::onFocusUp(FXObject*, FXSelector, void*) {
    if (isEnabled()) {
        FXint index = getCurrentItem();
        if (index < 0) {
            index = getNumItems() - 1;
        } else if (0 < index) {
            index--;
        }
        if (0 <= index && index < getNumItems()) {
            setCurrentItem(index, TRUE);
        }
        return 1;
    }
    return 0;
}


long
MFXComboBoxIcon::onFocusDown(FXObject*, FXSelector, void*) {
    if (isEnabled()) {
        FXint index = getCurrentItem();
        if (index < 0) {
            index = 0;
        } else if (index < getNumItems() - 1) {
            index++;
        }
        if (0 <= index && index < getNumItems()) {
            setCurrentItem(index, TRUE);
        }
        return 1;
    }
    return 0;
}


long MFXComboBoxIcon::onMouseWheel(FXObject*, FXSelector, void* ptr) {
    FXEvent* event = (FXEvent*)ptr;
    if (isEnabled()) {
        FXint index = getCurrentItem();
        if (event->code < 0) {
            if (index < 0) {
                index = 0;
            } else if (index < getNumItems() - 1) {
                index++;
            }
        } else if (event->code > 0) {
            if (index < 0) {
                index = getNumItems() - 1;
            } else if (0 < index) {
                index--;
            }
        }
        if (0 <= index && index < getNumItems()) {
            setCurrentItem(index, TRUE);
        }
        return 1;
    }
    return 0;
}


MFXComboBoxIcon::MFXComboBoxIcon() {}
