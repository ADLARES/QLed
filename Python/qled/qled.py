from PySide6.QtWidgets import QAbstractButton
from PySide6.QtCore import Qt, QByteArray, QFile, QTextStream
from PySide6.QtGui import QColor, QPainter
from PySide6.QtSvg import QSvgRenderer
from . import resources
from enum import Enum


class QLed(QAbstractButton):

    class ledShape(Enum):
        Circle = "circle.svg"
        Square = "square.svg"
        Triangle = "triangle.svg"
        Rounded = "rounded.svg"
        Rectangle = "rect.svg"

    def __init__(self, parent=None):
        super().__init__(parent)

        self._value = False
        self._colorOn = QColor("red")
        self._colorOff = QColor("gray")

        self._renderer = QSvgRenderer()
        self.setShape(self.ledShape.Circle)

        self.setFocusPolicy(Qt.StrongFocus)
        self.setMouseTracking(True)
        self.setAttribute(Qt.WidgetAttribute.WA_MacShowFocusRect, True)

    def paintEvent(self, _event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        self._renderer.render(painter, self.rect())

    def checkStateSet(self):
        self._updateRenderer()

    def nextCheckState(self):
        super().nextCheckState()
        self._updateRenderer()

    def _updateRenderer(self):
        color = self._colorOn if self._value else self._colorOff
        svgContent = self._shape
        svgContent = svgContent.replace("{$color_dark}", color.darker().name(QColor.HexRgb))
        svgContent = svgContent.replace("{$color_light}", color.lighter().name(QColor.HexRgb))

        self._renderer.load(QByteArray(svgContent.encode("utf-8")))
        self.update()

    def setOnColor(self, color: QColor):
        self._colorOn = color
        self._updateRenderer()

    def setOffColor(self, color: QColor):
        self._colorOff = color
        self._updateRenderer()

    def setShape(self, shape):
        self._shape = self.loadShapeTemplate(shape)
        self._updateRenderer()

    def loadShapeTemplate(self, shapeType):
        shapePath = f":/qled/resources/{shapeType.value}"
        file = QFile(shapePath)
        if not file.open(QFile.ReadOnly | QFile.Text):
            raise FileNotFoundError(f"Cannot open resource: {shapePath}")
        stream = QTextStream(file)
        svgData = stream.readAll()
        file.close()

        return svgData

    def setValue(self, value: bool):
        self._value = value
        self._updateRenderer()

    def toggleValue(self):
        self._value = not self._value
        self._updateRenderer()

    def sizeHint(self):
        return self.minimumSizeHint()

    def minimumSizeHint(self):
        return self._renderer.defaultSize()
