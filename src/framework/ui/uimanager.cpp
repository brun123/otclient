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

#include "uimanager.h"
#include "ui.h"

#include <framework/otml/otml.h>
#include <framework/graphics/graphics.h>
#include <framework/platform/platformwindow.h>

UIManager g_ui;

void UIManager::init()
{
    // creates root widget
    m_rootWidget = UIWidget::create<UIWidget>();
    m_rootWidget->setId("root");
    m_rootWidget->resize(g_window.getSize());
}

void UIManager::terminate()
{
    // destroy root widget and its children'
    m_rootWidget->destroy();
    m_rootWidget.reset();
}

void UIManager::render()
{
    m_rootWidget->render();
}

void UIManager::resize(const Size& size)
{
    m_rootWidget->resize(g_window.getSize());
}

void UIManager::inputEvent(const InputEvent& event)
{
    switch(event.type) {
        case Fw::KeyPressInputEvent:
            m_rootWidget->onKeyPress(event.keyCode, event.keyText, event.keyboardModifiers);
            break;
        case Fw::KeyReleaseInputEvent:
            m_rootWidget->onKeyRelease(event.keyCode, event.keyText, event.keyboardModifiers);
            break;
        case Fw::MousePressInputEvent:
            m_rootWidget->onMousePress(event.mousePos, event.mouseButton);
            break;
        case Fw::MouseReleaseInputEvent:
            m_rootWidget->onMouseRelease(event.mousePos, event.mouseButton);
            break;
        case Fw::MouseMoveInputEvent:
            m_rootWidget->updateState(Fw::HoverState);
            m_rootWidget->onMouseMove(event.mousePos, event.mouseMoved);
            break;
        case Fw::MouseWheelInputEvent:
            m_rootWidget->onMouseWheel(event.mousePos, event.wheelDirection);
            break;
    };
}

bool UIManager::importStyles(const std::string& file)
{
    try {
        OTMLDocumentPtr doc = OTMLDocument::parse(file);

        for(const OTMLNodePtr& styleNode : doc->children())
            importStyleFromOTML(styleNode);
        return true;
    } catch(Exception& e) {
        logError("Failed to import UI styles from '", file, "': ", e.what());
        return false;
    }
}

void UIManager::importStyleFromOTML(const OTMLNodePtr& styleNode)
{
    std::string tag = styleNode->tag();
    std::vector<std::string> split;
    boost::split(split, tag, boost::is_any_of(std::string("<")));
    if(split.size() != 2)
        throw OTMLException(styleNode, "not a valid style declaration");

    std::string name = split[0];
    std::string base = split[1];

    boost::trim(name);
    boost::trim(base);

    // TODO: styles must be searched by widget scopes, in that way this warning could be fixed
    // disable this warning because many ppl was complening about it
    /*
    auto it = m_styles.find(name);
    if(it != m_styles.end())
        logWarning("style '", name, "' is being redefined");
    */

    OTMLNodePtr style = getStyle(base)->clone();
    style->merge(styleNode);
    style->setTag(name);
    m_styles[name] = style;
}

OTMLNodePtr UIManager::getStyle(const std::string& styleName)
{
    if(boost::starts_with(styleName, "UI")) {
        OTMLNodePtr node = OTMLNode::create();
        node->writeAt("__widgetType", styleName);
        return node;
    }

    auto it = m_styles.find(styleName);
    if(it == m_styles.end()) {
        logError("Unable to retrive style '", styleName, "': not a defined style");
        return nullptr;
    }

    return m_styles[styleName];
}

UIWidgetPtr UIManager::loadUI(const std::string& file, const UIWidgetPtr& parent)
{
    try {
        OTMLDocumentPtr doc = OTMLDocument::parse(file);
        UIWidgetPtr widget;
        for(const OTMLNodePtr& node : doc->children()) {
            std::string tag = node->tag();

            // import styles in these files too
            if(tag.find("<") != std::string::npos)
                importStyleFromOTML(node);
            else {
                if(widget)
                    Fw::throwException("cannot have multiple main widgets in otui files");
                widget = loadWidgetFromOTML(node, parent);
            }
        }

        return widget;
    } catch(Exception& e) {
        logError("Failed to load UI from '", file, "': ", e.what());
        return nullptr;
    }
}

UIWidgetPtr UIManager::loadWidgetFromOTML(const OTMLNodePtr& widgetNode, const UIWidgetPtr& parent)
{
    OTMLNodePtr styleNode = getStyle(widgetNode->tag())->clone();
    styleNode->merge(widgetNode);

    std::string widgetType = styleNode->valueAt("__widgetType");

    // call widget creation from lua
    UIWidgetPtr widget = g_lua.callGlobalField<UIWidgetPtr>(widgetType, "create");
    if(parent)
        parent->addChild(widget);

    if(widget) {
        widget->setStyleFromNode(styleNode);

        for(const OTMLNodePtr& childNode : widgetNode->children()) {
            if(!childNode->isUnique())
                loadWidgetFromOTML(childNode, widget);
        }
    } else
        logError("Unable to create widget of type '", widgetType, "'");

    return widget;
}

