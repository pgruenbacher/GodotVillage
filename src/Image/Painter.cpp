/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Painter.h"
#include "Color.h"
#include <iostream>

#include "../Maths/Calcul.h"
#include "GaussianBlurFilter.h"
#include "PainterAlgorithms.h"

void Painter::drawRectangle(const Vector2I &bottomLeft,
		const Vector2I &topRight,
		Image *image,
		const Vector3F &color) {
	for (unsigned int y = bottomLeft.y; y <= topRight.y; ++y) {
		for (unsigned int x = bottomLeft.x; x <= topRight.x; ++x) {
			switch (image->getFormat()) {
				case Color::RGB_32:
					image->setAt(x, y, Color::RGBF(color.x, color.y, color.z));
					break;
				default:
					image->setAt(x, y, Color::RGBX(color.x, color.y, color.z));
					break;
			}
		}
	}
}

Vector3F Painter::getColor(const Vector2I &position,
		Image *image) {
	if (position.x < 0 || position.x >= image->getWidth() || position.y < 0 || position.y >= image->getHeight())
		return Vector3F();

	Color color(image->getAt(position.x, position.y));
	switch (image->getFormat()) {
		case Color::RGB_32:
			return Vector3F(color.rf(), color.gf(), color.bf());
			break;
		default:
			return Vector3F(color.r(), color.g(), color.b());
			break;
	}
}

void Painter::drawPoint(const Vector2I &point,
		Image *image,
		const Vector3F &color) {
	if (point.x >= 0 && point.x < image->getWidth() && point.y >= 0 && point.y < image->getHeight()) {
		switch (image->getFormat()) {
			case Color::RGB_32:
				image->setAt(point.x, point.y, Color::RGBF(MAX(0.0, MIN(1.0, color.x)),
													   MAX(0.0, MIN(1.0, color.y)),
													   MAX(0.0, MIN(1.0, color.z))));
				break;
			default:
				image->setAt(point.x, point.y, Color::RGBX(MAX(0.0, MIN(255.0, color.x)),
													   MAX(0.0, MIN(255.0, color.y)),
													   MAX(0.0, MIN(255.0, color.z))));
				break;
		}
	}
}

void Painter::drawPoints(Table<Vector2I> &points,
		Image *image,
		const Vector3F &color) {
	unsigned int size = points.getSize();
	for (unsigned int i = 0; i < size; ++i) {
		drawPoint(points[i], image, color);
	}
}

void Painter::drawBresenhamLine(const Vector2I &start,
		const Vector2I &end,
		Image *image,
		const Vector3F &color,
		int width) {
	if (image == NULL)
		return;

	if (start.x < 0 || start.x > image->getWidth() || start.y < 0 || start.y > image->getHeight() || end.x < 0 || end.x > image->getWidth() || end.y < 0 || end.y > image->getHeight()) {
		return;
	}

	Table<Vector2I> pointsToDraw = PainterAlgorithms::findBresenhamLinePoints(start, end);
	for (unsigned int i = 0; i < pointsToDraw.getSize(); ++i) {
		for (int w = -width; w <= width; ++w) {
			for (int h = -width; h <= width; ++h) {
				if (sqrt(double(h * h + w * w)) <= width) {
					drawPoint(pointsToDraw[i] + Vector2I(w, h), image, color);
				}
			}
		}
	}
}

void Painter::drawEmptyPolygon(const Table<Vector2I> &points,
		Image *image,
		const Vector3F &color) {
	Table<Vector2I> pointsToDraw = PainterAlgorithms::findEmptyPolygonPoints(points);
	drawPoints(pointsToDraw, image, color);
}

void Painter::drawFilledPolygon(const Table<Vector2I> &points,
		Image *image,
		const Vector3F &color) {
	Table<Vector2I> pointsToDraw = PainterAlgorithms::findFilledPolygonPoints(points);
	drawPoints(pointsToDraw, image, color);
}

void Painter::drawCircle(const Vector2I &origin,
		int radius,
		Image *image,
		const Vector3F &color) {
	for (int i = -radius; i <= radius; ++i) {
		for (int j = -radius; j <= radius; ++j) {
			if (i * i + j * j <= radius * radius) {
				Vector2I shift(i, j);
				drawPoint(origin + shift, image, color);
			}
		}
	}
}

void Painter::drawGaussian(const Vector2I &origin,
		int radius,
		Image *image,
		const Vector3F &color,
		float alpha) {
	Image copy(*image);
	Table<float> pyramid = GaussianBlurFilter::computeGaussianPyramid(radius, 4);
	for (int i = -radius * 4; i <= radius * 4; ++i) {
		for (int j = -radius * 4; j <= radius * 4; ++j) {
			Vector2I shift(i, j);
			Vector3F oldcolor = getColor(origin + shift, &copy);
			float t = 2.0 * M_PI * radius * radius * pyramid[radius * 4 + i] * pyramid[radius * 4 + j] * alpha;
			Vector3F resultColor = t * Vector3F(color) + (1.f - t) * oldcolor;
			drawPoint(origin + shift, image, resultColor);
		}
	}
}

Vector3F Painter::getGaussian(const Vector2I &origin,
		int radius,
		Image *image) {
	Vector3F color;
	Table<float> pyramid = GaussianBlurFilter::computeGaussianPyramid(radius, 4);
	for (int i = -radius * 4; i <= radius * 4; ++i) {
		for (int j = -radius * 4; j <= radius * 4; ++j) {
			Vector2I shift(i, j);
			float t = pyramid[radius * 4 + i] * pyramid[radius * 4 + j];
			color += Vector3F(getColor(origin + shift, image)) * t;
		}
	}
	std::cout << "Color : " << color << std::endl;
	return Vector3F(color);
}
