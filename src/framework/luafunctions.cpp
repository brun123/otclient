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

#include <framework/luascript/luainterface.h>
#include <framework/application.h>
#include <framework/graphics/fontmanager.h>
#include <framework/ui/ui.h>
#include <framework/net/protocol.h>
#include <framework/core/eventdispatcher.h>
#include <framework/core/configmanager.h>
#include <framework/otml/otml.h>
#include <framework/graphics/graphics.h>
#include <framework/platform/platformwindow.h>

void Application::registerLuaFunctions()
{
    // UIWidget
    g_lua.registerClass<UIWidget>();
    g_lua.bindClassStaticFunction<UIWidget>("create", &UIWidget::create<UIWidget>);
    g_lua.bindClassMemberFunction<UIWidget>("destroy", &UIWidget::destroy);
    g_lua.bindClassMemberFunction<UIWidget>("setVisible", &UIWidget::setVisible);
    g_lua.bindClassMemberFunction<UIWidget>("setEnabled", &UIWidget::setEnabled);
    g_lua.bindClassMemberFunction<UIWidget>("setPressed", &UIWidget::setPressed);
    g_lua.bindClassMemberFunction<UIWidget>("setId", &UIWidget::setId);
    g_lua.bindClassMemberFunction<UIWidget>("setFocusable", &UIWidget::setFocusable);
    g_lua.bindClassMemberFunction<UIWidget>("setPhantom", &UIWidget::setPhantom);
    g_lua.bindClassMemberFunction<UIWidget>("setStyle", &UIWidget::setStyle);
    g_lua.bindClassMemberFunction<UIWidget>("setStyleFromNode", &UIWidget::setStyleFromNode);
    //g_lua.bindClassMemberFunction<UIWidget>("setLayout", &UIWidget::setLayout);
    g_lua.bindClassMemberFunction<UIWidget>("setParent", &UIWidget::setParent);
    g_lua.bindClassMemberFunction<UIWidget>("setRect", &UIWidget::setRect);
    g_lua.bindClassMemberFunction<UIWidget>("setX", &UIWidget::setX);
    g_lua.bindClassMemberFunction<UIWidget>("setY", &UIWidget::setY);
    g_lua.bindClassMemberFunction<UIWidget>("setWidth", &UIWidget::setWidth);
    g_lua.bindClassMemberFunction<UIWidget>("setHeight", &UIWidget::setHeight);
    //g_lua.bindClassMemberFunction<UIWidget>("setImage", &UIWidget::setImage);
    //g_lua.bindClassMemberFunction<UIWidget>("setFont", &UIWidget::setFont);
    g_lua.bindClassMemberFunction<UIWidget>("setOpacity", &UIWidget::setOpacity);
    g_lua.bindClassMemberFunction<UIWidget>("setBackgroundColor", &UIWidget::setBackgroundColor);
    g_lua.bindClassMemberFunction<UIWidget>("setForegroundColor", &UIWidget::setForegroundColor);
    g_lua.bindClassMemberFunction<UIWidget>("setMarginTop", &UIWidget::setMarginTop);
    g_lua.bindClassMemberFunction<UIWidget>("setMarginRight", &UIWidget::setMarginRight);
    g_lua.bindClassMemberFunction<UIWidget>("setMarginBottom", &UIWidget::setMarginBottom);
    g_lua.bindClassMemberFunction<UIWidget>("setMarginLeft", &UIWidget::setMarginLeft);
    g_lua.bindClassMemberFunction<UIWidget>("setSizeFixed", &UIWidget::setSizeFixed);
    g_lua.bindClassMemberFunction<UIWidget>("setLastFocusReason", &UIWidget::setLastFocusReason);
    g_lua.bindClassMemberFunction<UIWidget>("resize", &UIWidget::resize);
    g_lua.bindClassMemberFunction<UIWidget>("moveTo", &UIWidget::moveTo);
    g_lua.bindClassMemberFunction<UIWidget>("hide", &UIWidget::hide);
    g_lua.bindClassMemberFunction<UIWidget>("show", &UIWidget::show);
    g_lua.bindClassMemberFunction<UIWidget>("disable", &UIWidget::disable);
    g_lua.bindClassMemberFunction<UIWidget>("enable", &UIWidget::enable);
    g_lua.bindClassMemberFunction<UIWidget>("lock", &UIWidget::lock);
    g_lua.bindClassMemberFunction<UIWidget>("unlock", &UIWidget::unlock);
    g_lua.bindClassMemberFunction<UIWidget>("focus", &UIWidget::focus);
    g_lua.bindClassMemberFunction<UIWidget>("isActive", &UIWidget::isActive);
    g_lua.bindClassMemberFunction<UIWidget>("isEnabled", &UIWidget::isEnabled);
    g_lua.bindClassMemberFunction<UIWidget>("isDisabled", &UIWidget::isDisabled);
    g_lua.bindClassMemberFunction<UIWidget>("isFocused", &UIWidget::isFocused);
    g_lua.bindClassMemberFunction<UIWidget>("isHovered", &UIWidget::isHovered);
    g_lua.bindClassMemberFunction<UIWidget>("isPressed", &UIWidget::isPressed);
    g_lua.bindClassMemberFunction<UIWidget>("isVisible", &UIWidget::isVisible);
    g_lua.bindClassMemberFunction<UIWidget>("isHidden", &UIWidget::isHidden);
    g_lua.bindClassMemberFunction<UIWidget>("isExplicitlyEnabled", &UIWidget::isExplicitlyEnabled);
    g_lua.bindClassMemberFunction<UIWidget>("isExplicitlyVisible", &UIWidget::isExplicitlyVisible);
    g_lua.bindClassMemberFunction<UIWidget>("isFocusable", &UIWidget::isFocusable);
    g_lua.bindClassMemberFunction<UIWidget>("isPhantom", &UIWidget::isPhantom);
    g_lua.bindClassMemberFunction<UIWidget>("isSizeFixed", &UIWidget::isSizeFixed);
    g_lua.bindClassMemberFunction<UIWidget>("hasChildren", &UIWidget::hasChildren);
    g_lua.bindClassMemberFunction<UIWidget>("hasChild", &UIWidget::hasChild);
    g_lua.bindClassMemberFunction<UIWidget>("getId", &UIWidget::getId);
    g_lua.bindClassMemberFunction<UIWidget>("getChildCount", &UIWidget::getChildCount);
    //g_lua.bindClassMemberFunction<UIWidget>("getLayout", &UIWidget::getLayout);
    g_lua.bindClassMemberFunction<UIWidget>("getParent", &UIWidget::getParent);
    g_lua.bindClassMemberFunction<UIWidget>("getRootParent", &UIWidget::getRootParent);
    g_lua.bindClassMemberFunction<UIWidget>("getPosition", &UIWidget::getPosition);
    g_lua.bindClassMemberFunction<UIWidget>("getSize", &UIWidget::getSize);
    g_lua.bindClassMemberFunction<UIWidget>("getRect", &UIWidget::getRect);
    g_lua.bindClassMemberFunction<UIWidget>("getX", &UIWidget::getX);
    g_lua.bindClassMemberFunction<UIWidget>("getY", &UIWidget::getY);
    g_lua.bindClassMemberFunction<UIWidget>("getWidth", &UIWidget::getWidth);
    g_lua.bindClassMemberFunction<UIWidget>("getHeight", &UIWidget::getHeight);
    //g_lua.bindClassMemberFunction<UIWidget>("getImage", &UIWidget::getImage);
    //g_lua.bindClassMemberFunction<UIWidget>("getFont", &UIWidget::getFont);
    g_lua.bindClassMemberFunction<UIWidget>("getForegroundColor", &UIWidget::getForegroundColor);
    g_lua.bindClassMemberFunction<UIWidget>("getBackgroundColor", &UIWidget::getBackgroundColor);
    g_lua.bindClassMemberFunction<UIWidget>("getOpacity", &UIWidget::getOpacity);
    g_lua.bindClassMemberFunction<UIWidget>("getMarginTop", &UIWidget::getMarginTop);
    g_lua.bindClassMemberFunction<UIWidget>("getMarginRight", &UIWidget::getMarginRight);
    g_lua.bindClassMemberFunction<UIWidget>("getMarginBottom", &UIWidget::getMarginBottom);
    g_lua.bindClassMemberFunction<UIWidget>("getMarginLeft", &UIWidget::getMarginLeft);
    g_lua.bindClassMemberFunction<UIWidget>("getLastFocusReason", &UIWidget::getLastFocusReason);
    g_lua.bindClassMemberFunction<UIWidget>("getStyle", &UIWidget::getStyle);
    g_lua.bindClassMemberFunction<UIWidget>("getChildren", &UIWidget::getChildren);
    g_lua.bindClassMemberFunction<UIWidget>("getFocusedChild", &UIWidget::getFocusedChild);
    g_lua.bindClassMemberFunction<UIWidget>("getChildAfter", &UIWidget::getChildAfter);
    g_lua.bindClassMemberFunction<UIWidget>("getChildBefore", &UIWidget::getChildBefore);
    g_lua.bindClassMemberFunction<UIWidget>("getChildById", &UIWidget::getChildById);
    g_lua.bindClassMemberFunction<UIWidget>("getChildByPos", &UIWidget::getChildByPos);
    g_lua.bindClassMemberFunction<UIWidget>("getChildByIndex", &UIWidget::getChildByIndex);
    g_lua.bindClassMemberFunction<UIWidget>("recursiveGetChildById", &UIWidget::recursiveGetChildById);
    g_lua.bindClassMemberFunction<UIWidget>("recursiveGetChildByPos", &UIWidget::recursiveGetChildByPos);
    g_lua.bindClassMemberFunction<UIWidget>("backwardsGetWidgetById", &UIWidget::backwardsGetWidgetById);
    g_lua.bindClassMemberFunction<UIWidget>("addChild", &UIWidget::addChild);
    g_lua.bindClassMemberFunction<UIWidget>("insertChild", &UIWidget::insertChild);
    g_lua.bindClassMemberFunction<UIWidget>("removeChild", &UIWidget::removeChild);
    g_lua.bindClassMemberFunction<UIWidget>("focusChild", &UIWidget::focusChild);
    g_lua.bindClassMemberFunction<UIWidget>("focusNextChild", &UIWidget::focusNextChild);
    g_lua.bindClassMemberFunction<UIWidget>("focusPreviousChild", &UIWidget::focusPreviousChild);
    g_lua.bindClassMemberFunction<UIWidget>("moveChildToTop", &UIWidget::moveChildToTop);
    g_lua.bindClassMemberFunction<UIWidget>("moveChildToIndex", &UIWidget::moveChildToIndex);
    g_lua.bindClassMemberFunction<UIWidget>("lockChild", &UIWidget::lockChild);
    g_lua.bindClassMemberFunction<UIWidget>("unlockChild", &UIWidget::unlockChild);
    g_lua.bindClassMemberFunction<UIWidget>("isChildLocked", &UIWidget::isChildLocked);
    g_lua.bindClassMemberFunction<UIWidget>("getChildIndex", &UIWidget::getChildIndex);
    g_lua.bindClassMemberFunction<UIWidget>("updateParentLayout", &UIWidget::updateParentLayout);
    g_lua.bindClassMemberFunction<UIWidget>("updateLayout", &UIWidget::updateLayout);
    g_lua.bindClassMemberFunction<UIWidget>("updateStates", &UIWidget::updateStates);
    g_lua.bindClassMemberFunction<UIWidget>("updateState", &UIWidget::updateState);
    g_lua.bindClassMemberFunction<UIWidget>("setState", &UIWidget::setState);
    g_lua.bindClassMemberFunction<UIWidget>("hasState", &UIWidget::hasState);
    g_lua.bindClassMemberFunction<UIWidget>("updateStyle", &UIWidget::updateStyle);
    g_lua.bindClassMemberFunction<UIWidget>("applyStyle", &UIWidget::applyStyle);

    // UILabel
    g_lua.registerClass<UILabel, UIWidget>();
    g_lua.bindClassStaticFunction<UILabel>("create", &UIWidget::create<UILabel>);
    g_lua.bindClassMemberFunction<UILabel>("getText", &UILabel::getText);
    g_lua.bindClassMemberFunction<UILabel>("setText", &UILabel::setText);
    g_lua.bindClassMemberFunction("resizeToText", &UILabel::resizeToText);

    // UILabel
    g_lua.registerClass<UIProgressBar, UIWidget>();
    g_lua.bindClassStaticFunction<UIProgressBar>("create", &UIWidget::create<UIProgressBar>);
    g_lua.bindClassMemberFunction<UIProgressBar>("getPercent", &UIProgressBar::getPercent);
    g_lua.bindClassMemberFunction<UIProgressBar>("setPercent", &UIProgressBar::setPercent);

    // UIButton
    g_lua.registerClass<UIButton, UIWidget>();
    g_lua.bindClassStaticFunction<UIButton>("create", &UIWidget::create<UIButton>);
    g_lua.bindClassMemberFunction<UIButton>("getText", &UIButton::getText);
    g_lua.bindClassMemberFunction<UIButton>("setText", &UIButton::setText);

    // UILineEdit
    g_lua.registerClass<UILineEdit, UIWidget>();
    g_lua.bindClassStaticFunction<UILineEdit>("create", &UIWidget::create<UILineEdit>);
    g_lua.bindClassMemberFunction<UILineEdit>("getText", &UILineEdit::getText);
    g_lua.bindClassMemberFunction<UILineEdit>("setText", &UILineEdit::setText);
    g_lua.bindClassMemberFunction<UILineEdit>("clearText", &UILineEdit::clearText);
    g_lua.bindClassMemberFunction<UILineEdit>("getCursorPos", &UILineEdit::getCursorPos);

    // UICheckBox
    g_lua.registerClass<UICheckBox, UIWidget>();
    g_lua.bindClassStaticFunction<UICheckBox>("create", &UIWidget::create<UICheckBox>);
    g_lua.bindClassMemberFunction<UICheckBox>("isChecked", &UICheckBox::isChecked);
    g_lua.bindClassMemberFunction<UICheckBox>("setChecked", &UICheckBox::setChecked);


    // UIWindow
    g_lua.registerClass<UIWindow, UIWidget>();
    g_lua.bindClassStaticFunction<UIWindow>("create", &UIWidget::create<UIWindow>);
    g_lua.bindClassMemberFunction<UIWindow>("getTitle", &UIWindow::getTitle);
    g_lua.bindClassMemberFunction<UIWindow>("setTitle", &UIWindow::setTitle);

    // UIFrameCounter
    g_lua.registerClass<UIFrameCounter, UIWidget>();
    g_lua.bindClassStaticFunction<UIFrameCounter>("create", &UIWidget::create<UIFrameCounter>);
    g_lua.bindClassMemberFunction("getFrameCount", &UIFrameCounter::getFrameCount);

    // Protocol
    g_lua.registerClass<Protocol>();

    // ConfigManager
    g_lua.registerStaticClass("g_configs");
    g_lua.bindClassStaticFunction("g_configs", "set", std::bind(&ConfigManager::set, &g_configs, _1, _2));
    g_lua.bindClassStaticFunction("g_configs", "get", std::bind(&ConfigManager::get, &g_configs, _1));
    g_lua.bindClassStaticFunction("g_configs", "exists", std::bind(&ConfigManager::exists, &g_configs, _1));

    g_lua.registerStaticClass("g_window");
    g_lua.bindClassStaticFunction("g_window", "show", std::bind(&PlatformWindow::show, &g_window));
    g_lua.bindClassStaticFunction("g_window", "hide", std::bind(&PlatformWindow::hide, &g_window));
    g_lua.bindClassStaticFunction("g_window", "move", std::bind(&PlatformWindow::move, &g_window, _1));
    g_lua.bindClassStaticFunction("g_window", "resize", std::bind(&PlatformWindow::resize, &g_window, _1));
    g_lua.bindClassStaticFunction("g_window", "setVerticalSync", std::bind(&PlatformWindow::setVerticalSync, &g_window, _1));
    g_lua.bindClassStaticFunction("g_window", "setTitle", std::bind(&PlatformWindow::setTitle, &g_window, _1));
    g_lua.bindClassStaticFunction("g_window", "setIcon", std::bind(&PlatformWindow::setIcon, &g_window, _1));
    g_lua.bindClassStaticFunction("g_window", "getMousePos", std::bind(&PlatformWindow::getMousePos, &g_window));
    g_lua.bindClassStaticFunction("g_window", "getSize", std::bind(&PlatformWindow::getSize, &g_window));

    // Logger
    g_lua.registerClass<Logger>();
    g_lua.bindClassStaticFunction<Logger>("log", std::bind(&Logger::log, &g_logger, _1, _2));
    g_lua.bindClassStaticFunction<Logger>("fireOldMessages", std::bind(&Logger::fireOldMessages, &g_logger));
    g_lua.bindClassStaticFunction<Logger>("setOnLog", std::bind(&Logger::setOnLog, &g_logger, _1));

    // Font

    /*
    // FontManager
    g_lua.registerStaticClass("g_fonts");
    g_lua.bindClassStaticFunction("g_fonts", "releaseFonts", std::bind(&FontManager::releaseFonts, &g_fonts));
    g_lua.bindClassStaticFunction("g_fonts", "importFont", std::bind(&FontManager::importFont, &g_fonts, _1));
    g_lua.bindClassStaticFunction("g_fonts", "fontExists", std::bind(&FontManager::fontExists, &g_fonts, _1));
    g_lua.bindClassStaticFunction("g_fonts", "getFont", std::bind(&FontManager::getFont, &g_fonts, _1));
    g_lua.bindClassStaticFunction("g_fonts", "getDefaultFont", std::bind(&FontManager::getDefaultFont, &g_fonts));
    g_lua.bindClassStaticFunction("g_fonts", "setDefaultFont", std::bind(&FontManager::setDefaultFont, &g_fonts, _1));
    */

    g_lua.registerStaticClass("g_dispatcher");
    g_lua.bindClassStaticFunction("g_dispatcher", "addEvent", std::bind(&EventDispatcher::addEvent, &g_dispatcher, _1, _2));
    g_lua.bindClassStaticFunction("g_dispatcher", "scheduleEvent", std::bind(&EventDispatcher::scheduleEvent, &g_dispatcher, _1, _2));

    // global functions
    g_lua.bindGlobalFunction("importFont", std::bind(&FontManager::importFont, &g_fonts, _1));
    g_lua.bindGlobalFunction("importStyles", std::bind(&UIManager::importStyles, &g_ui, _1));
    g_lua.bindGlobalFunction("setDefaultFont", std::bind(&FontManager::setDefaultFont, &g_fonts, _1));
    g_lua.bindGlobalFunction("loadUI", std::bind(&UIManager::loadUI, &g_ui, _1, _2));
    g_lua.bindGlobalFunction("getRootWidget", std::bind(&UIManager::getRootWidget, &g_ui));
}
