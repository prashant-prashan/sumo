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
/// @file    MFXComboBoxIcon.h
/// @author  Jakob Erdmann
/// @author  Pablo Alvarez Lopez
/// @date    2018-12-19
///
//
/****************************************************************************/

#pragma once
#include <config.h>

#include "MFXListIcon.h"
#include "MFXTextFieldSearch.h"


/// @brief ComboBox with icon
class MFXComboBoxIcon : public FXPacker {
    /// @brief FOX declaration
    FXDECLARE(MFXComboBoxIcon)

public:
    /// @brief enum for ID
    enum {
        ID_LIST = FXPacker::ID_LAST,
        ID_TEXT,
        ID_SEARCH,
        ID_LAST
    };

    /// @brief Construct a Combo Box widget with room to display cols columns of text
    MFXComboBoxIcon(FXComposite* p, FXint cols, const bool haveIcons, const bool canSearch,
                    FXObject* tgt = nullptr, FXSelector sel = 0, FXuint opts = COMBOBOX_NORMAL,
                    FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
                    FXint pl = DEFAULT_PAD, FXint pr = DEFAULT_PAD, FXint pt = DEFAULT_PAD, FXint pb = DEFAULT_PAD);

    /// @brief Destructor
    virtual ~MFXComboBoxIcon();

    /// @brief Create server-side resources
    virtual void create();

    /// @brief Detach server-side resources
    virtual void detach();

    /// @brief Destroy server-side resources
    virtual void destroy();

    /// @brief Enable combo box
    virtual void enable();

    /// @brief Disable combo box
    virtual void disable();

    /// @brief Return default width
    virtual FXint getDefaultWidth();

    /// @brief Return default height
    virtual FXint getDefaultHeight();

    /// @brief Perform layout
    virtual void layout();

    /// @brief Get the text
    FXString getText() const;

    /// @brief Return the number of items in the list
    FXint getNumItems() const;

    /// @brief Return the number of visible items
    FXint getNumVisible() const;

    /// @brief Set the number of visible items in the drop down list
    void setNumVisible(FXint nvis);

    /// @brief Return true if current item
    FXbool isItemCurrent(FXint index) const;

    /// @brief Set the current item (index is zero-based)
    long setCurrentItem(const FXint index, FXbool notify = FALSE);

    /// @brief Set the current item
    long setCurrentItem(const FXString& text, FXbool notify = FALSE);
    
    /// @brief Get the current item's index
    FXint getCurrentItem() const;

    /// @brief Return the item at the given index
    FXString getItem(FXint index) const;

    /// @brief Replace the item at index
    FXint updateIconItem(FXint index, const FXString& text, FXIcon* icon = nullptr, FXColor bgColor = FXRGB(255, 255, 255), void* ptr = nullptr);
    
    /// @brief insert icon item in the given position
    FXint insertIconItem(FXint index, const FXString& text, FXIcon* icon = nullptr, FXColor bgColor = FXRGB(255, 255, 255), void* ptr = nullptr);

    /// @brief append icon item
    FXint appendIconItem(const FXString& text, FXIcon* icon = nullptr, FXColor bgColor = FXRGB(255, 255, 255), void* ptr = nullptr);

    /// @brief set custom text
    void setCustomText(const FXString text);

    /// @brief Remove all items from the list
    void clearItems();

    /// @brief find item
    FXint findItem(const FXString& text) const;

    /// @brief Get text for specified item
    FXString getItemText(FXint index) const;

    /// @brief Is the pane shown
    FXbool isPaneShown() const;

    /// @brief Get text font
    FXFont* getFont() const;

    /// @brief Set window background color
    virtual void setBackColor(FXColor clr);

    /// @brief Get background color
    FXColor getBackColor() const;

    /// @brief Change text color
    void setTextColor(FXColor clr);

    /// @brief Return text color
    FXColor getTextColor() const;

    /// @brief Return selected background color
    FXColor getSelBackColor() const;

    /// @brief Return selected text color
    FXColor getSelTextColor() const;

    /// @brief Set the combobox help text
    void setHelpText(const FXString& txt);

    /// @brief Get the combobox help text
    const FXString& getHelpText() const;

    /// @brief Set the tool tip message for this combobox
    void setTipText(const FXString& txt);

    /// @brief Get the tool tip message for this combobox
    const FXString& getTipText() const;

    /// @brief Commands
    /// @{

    long onFocusUp(FXObject*, FXSelector, void*);
    long onFocusDown(FXObject*, FXSelector, void*);
    long onFocusSelf(FXObject*, FXSelector, void*);
    long onMouseWheel(FXObject*, FXSelector, void*);
    long onTextButton(FXObject*, FXSelector, void*);
    long onTextChanged(FXObject*, FXSelector, void*);
    long onTextCommand(FXObject*, FXSelector, void*);
    long onListClicked(FXObject*, FXSelector, void*);
    long onFwdToText(FXObject*, FXSelector, void*);
    long onUpdFmText(FXObject*, FXSelector, void*);
    long onCmdFilter(FXObject*, FXSelector, void*);
    
    /// @}

protected:
    /// @brief FOX need this
    MFXComboBoxIcon();

    /// @brief label for icon
    FXLabel* myIconLabel = nullptr;

    /// @brief textField
    FXTextField* myTextField = nullptr;

    /// @brief myButton
    FXMenuButton* myButton = nullptr;

    /// @brief list with all items
    MFXListIcon* myList = nullptr;

    /// @brief text field search
    MFXTextFieldSearch* myTextFieldSearch = nullptr;

    /// @brief popup in which place search label and list
    FXPopup* myPane = nullptr;

private:
    /// @brief invalidate copy constructor
    MFXComboBoxIcon(const MFXComboBoxIcon&);

    /// @brief invalidate assignment operator
    MFXComboBoxIcon& operator=(const MFXComboBoxIcon&) = delete;
};
