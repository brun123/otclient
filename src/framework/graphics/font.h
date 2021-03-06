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

#ifndef FONT_H
#define FONT_H

#include "declarations.h"

#include <framework/otml/declarations.h>

class Font
{
public:
    Font(const std::string& name) : m_name(name) { }

    /// Load font from otml node
    void load(const OTMLNodePtr& fontNode);

    /// Simple text render starting at startPos
    void renderText(const std::string& text,
                    const Point& startPos,
                    const Color& color = Fw::white);

    /// Advanced text render delimited by a screen region and alignment
    void renderText(const std::string& text,
                    const Rect& screenCoords,
                    Fw::AlignmentFlag align = Fw::AlignTopLeft,
                    const Color& color = Fw::white);

    /// Calculate glyphs positions to use on render, also calculates textBoxSize if wanted
    const std::vector<Point>& calculateGlyphsPositions(const std::string& text,
                                                       Fw::AlignmentFlag align = Fw::AlignTopLeft,
                                                       Size* textBoxSize = NULL) const;

    /// Simulate render and calculate text size
    Size calculateTextRectSize(const std::string& text);

    std::string getName() const { return m_name; }
    int getGlyphHeight() const { return m_glyphHeight; }
    const Rect* getGlyphsTextureCoords() const { return m_glyphsTextureCoords; }
    const Size* getGlyphsSize() const { return m_glyphsSize; }
    const TexturePtr& getTexture() const { return m_texture; }
    int getYOffset() const { return m_yOffset; }
    Size getGlyphSpacing() const { return m_glyphSpacing; }

private:
    /// Calculates each font character by inspecting font bitmap
    void calculateGlyphsWidthsAutomatically(const Size& glyphSize);

    std::string m_name;
    int m_glyphHeight;
    int m_firstGlyph;
    int m_yOffset;
    Size m_glyphSpacing;
    TexturePtr m_texture;
    Rect m_glyphsTextureCoords[256];
    Size m_glyphsSize[256];
};


#endif

