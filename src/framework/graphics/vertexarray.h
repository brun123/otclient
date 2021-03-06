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

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "declarations.h"
#include <framework/util/databuffer.h>

class VertexArray
{
public:
    inline void addVertex(GLfloat x, GLfloat y) {  m_buffer << x << y; }
    inline void addRect(const Rect& rect) {
        GLfloat top = rect.top();
        GLfloat right = rect.right()+1;
        GLfloat bottom = rect.bottom()+1;
        GLfloat left = rect.left();

        addVertex(left, top);
        addVertex(right, top);
        addVertex(left, bottom);
        addVertex(left, bottom);
        addVertex(right, top);
        addVertex(right, bottom);
    }

    inline void addQuad(const Rect& rect) {
        GLfloat top = rect.top();
        GLfloat right = rect.right()+1;
        GLfloat bottom = rect.bottom()+1;
        GLfloat left = rect.left();

        addVertex(left, top);
        addVertex(right, top);
        addVertex(left, bottom);
        addVertex(right, bottom);
    }

    void clear() { m_buffer.reset(); }
    GLfloat *vertices() const { return m_buffer.data(); }
    int vertexCount() const { return m_buffer.size() / 2; }

private:
    DataBuffer<GLfloat> m_buffer;
};

#endif
