/*
 * Copyright (c) 2010-2011 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "declarations.h"
#include <framework/core/inputevent.h>
#include <framework/otml/declarations.h>

class UIManager
{
public:
    void init();
    void terminate();

    void render();
    void resize(const Size& size);
    void inputEvent(const InputEvent& event);

    bool importStyles(const std::string& file);
    void importStyleFromOTML(const OTMLNodePtr& styleNode);
    OTMLNodePtr getStyle(const std::string& styleName);

    UIWidgetPtr loadUI(const std::string& file, const UIWidgetPtr& parent = nullptr);
    UIWidgetPtr loadWidgetFromOTML(const OTMLNodePtr& widgetNode, const UIWidgetPtr& parent);

    //void setMouseGrabWidget();
    //void setKeyboardGrabWidget();

    UIWidgetPtr getRootWidget() { return m_rootWidget; }

private:
    UIWidgetPtr m_rootWidget;
    //UIWidgetPtr m_mouseGrabWidget;
    //UIWidgetPtr m_keyboardGrabWidget;
    std::map<std::string, OTMLNodePtr> m_styles;
};

extern UIManager g_ui;

#endif
