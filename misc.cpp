// -----------------------------------------------------------------------------
// Copyright (c) 2019 Jens Kallup - non-profit => kallup.jens@web.de
// all rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// -----------------------------------------------------------------------------
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QWidget>
#include <QPixmap>
#include <QIcon>
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include <QDomElement>

class QIcon;
class QFile;
class QPixmap;

void SetAttrRecur(QDomElement elem, QString strtagname, QString strattr, QString strattrval);

QIcon *
ChangeSVGColor(QWidget *widget, QString resName)
{
    //open svg resource load contents to qbytearray
    QFile file(resName);
    file.open(QIODevice::ReadOnly);
    QByteArray baData(file.readAll());

    //load svg contents to xml document and edit contents
    QDomDocument * doc = new QDomDocument;
    doc->setContent(baData);

    //recurivelly change color
    SetAttrRecur(doc->documentElement(), "path", "fill", "white");

    //create svg renderer with edited contents
    QSvgRenderer svgRenderer(doc->toByteArray());

    //create pixmap target (could be a QImage)
    QPixmap pix(svgRenderer.defaultSize());
    pix.fill(Qt::transparent);

    //create painter to act over pixmap
    QPainter * pixPainter = new QPainter(widget);
               pixPainter->begin(widget);
               pixPainter->end();

    //use renderer to render over painter which paints on pixmap
    svgRenderer.render(pixPainter);

    QIcon  * myicon = new QIcon(pix);
    return myicon;
}


void SetAttrRecur(QDomElement elem, QString strtagname, QString strattr, QString strattrval)
{
    //if it has the tagname then overwritte desired attribute
    if (elem.tagName().compare(strtagname) == 0) {
        elem.setAttribute(strattr, strattrval);
    }

    //loop all children
    for (int i = 0; i < elem.childNodes().count(); i++) {
        if (!elem.childNodes().at(i).isElement())
        continue;
        SetAttrRecur(elem.childNodes().at(i).toElement(), strtagname, strattr, strattrval);
    }
}
