/***************************************************************************
 *   Copyright (C) 2010 by P. Sereno                                       *
 *   http://www.sereno-online.com                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation              *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *   http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.               *
 ***************************************************************************/

#include <QColor>
#include <QtGlobal>
#include <QtGui>
#include <QPolygon>
#include <QtSvg>
#include <QSvgRenderer>

#include "qled.h"

/*!
  \brief QLed: this is the QLed constructor.
  \param parent: The Parent Widget
*/
QLed::QLed(QWidget *parent)
    : QWidget(parent),
      m_value(false),
      m_onColor("Red"),
      m_offColor("Grey"),
      m_shape(Circle),
      renderer(new QSvgRenderer())
{
    shapes << "circle" << "square" << "triang" << "round";
    updateRenderer();
}


QLed::~QLed() {
    delete renderer;
}


/*!
  \brief paintEvent: painting method
  \param QPaintEvent *
  \return void
*/
void QLed::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    renderer->render(&painter);
}


/*!
  \brief updateRenderer: this updates the renderer to contain the correct data for the current properties
  \return void
*/
void QLed::updateRenderer()
{
    QColor currentColor = m_value ? m_onColor : m_offColor;

    QString filePathTemplate(":/resources/%1.svg");
    QString filePath = filePathTemplate.arg(shapes[m_shape]);

    QFile svgFile(filePath);
    svgFile.open(QIODevice::ReadOnly);
    auto svgContent = svgFile.readAll();
    svgFile.close();

    svgContent.replace("{$color_dark}", currentColor.darker().name(QColor::HexRgb).toLatin1());
    svgContent.replace("{$color_light}", currentColor.name(QColor::HexRgb).toLatin1());

    renderer->load(svgContent);
}


/*!
  \brief setOnColor: this method allows to change the On color
  \param QColor newColor
  \return void
*/
void QLed::setOnColor(QColor newColor)
{
    m_onColor = newColor;
    updateRenderer();
    update();
}


/*!
  \brief setOffColor: this method allows to change the Off color
  \param QColor newColor
  \return void
*/
void QLed::setOffColor(QColor newColor)
{
    m_offColor = newColor;
    updateRenderer();
    update();
}


/*!
  \brief setShape: this method allows to change the led shape {Circle,Square,Triangle,Rounded rectangle}
  \param ledColor newColor
  \return void
*/
void QLed::setShape(ledShape newShape)
{
    m_shape = newShape;
    updateRenderer();
    update();
}


/*!
  \brief setValue: this method allows to set the led value {true,false}
  \param ledColor newColor
  \return void
*/
void QLed::setValue(bool value)
{
    m_value = value;
    updateRenderer();
    update();
}


/*!
  \brief toggleValue: this method toggles the led value
  \param ledColor newColor
  \return void
*/
void QLed::toggleValue()
{ 
    m_value =! m_value;
    updateRenderer();
    update();
}
