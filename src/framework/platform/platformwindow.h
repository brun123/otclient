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

#ifndef PLATFORMWINDOW_H
#define PLATFORMWINDOW_H

#include <framework/global.h>
#include <framework/core/inputevent.h>

class PlatformWindow
{
    typedef std::function<void(const Size&)> OnResizeCallback;
    typedef std::function<void(const InputEvent&)> OnInputEventCallback;

public:
    virtual void init() = 0;
    virtual void terminate() = 0;

    virtual void move(const Point& pos) = 0;
    virtual void resize(const Size& size) = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void maximize() = 0;
    virtual void poll() = 0;
    virtual void swapBuffers() = 0;
    virtual void showMouse() = 0;
    virtual void hideMouse() = 0;

    virtual void setTitle(const std::string& title) = 0;
    virtual void setMinimumSize(const Size& minimumSize) = 0;
    virtual void setFullscreen(bool fullscreen) = 0;
    virtual void setVerticalSync(bool enable) = 0;
    virtual void setIcon(const std::string& iconFile) = 0;
    virtual void setClipboardText(const std::string& text) = 0;

    virtual Size getDisplaySize() = 0;
    virtual std::string getClipboardText() = 0;

    int getDisplayWidth() { return getDisplaySize().width(); }
    int getDisplayHeight() { return getDisplaySize().width(); }

    Size getSize() { return m_size; }
    int getWidth() { return m_size.width(); }
    int getHeight() { return m_size.height(); }
    Point getPos() { return m_pos; }
    int getX() { return m_pos.x; }
    int getY() { return m_pos.y; }
    Point getMousePos() { return m_inputEvent.mousePos; }
    int getKeyboardModifiers() { return m_inputEvent.keyboardModifiers; }

    bool isVisible() { return m_visible; }
    bool isFullscreen() { return m_fullscreen; }
    bool hasFocus() { return m_focused; }

    void setOnClose(const SimpleCallback& onClose) { m_onClose = onClose; }
    void setOnResize(const OnResizeCallback& onResize) { m_onResize = onResize; }
    void setOnInputEvent(const OnInputEventCallback& onInputEvent) { m_onInputEvent = onInputEvent; }

protected:
    Size m_size;
    Point m_pos;
    InputEvent m_inputEvent;

    Boolean<false> m_created;
    Boolean<false> m_visible;
    Boolean<false> m_focused;
    Boolean<false> m_fullscreen;

    SimpleCallback m_onClose;
    OnResizeCallback m_onResize;
    OnInputEventCallback m_onInputEvent;
};

extern PlatformWindow& g_window;

#endif
