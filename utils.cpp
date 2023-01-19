#include "pch.h"
//#define _USE_MATH_DEFINES
//#include <cmath>
#include <algorithm>
#include <iostream>
#include "utils.h"

namespace utils
{
#pragma region OpenGLDrawFunctionality
	void ClearBackground(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void ClearBackground()
	{
		ClearBackground(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	}
	void SetColor(float r, float g, float b, float a)
	{
		glColor4f(r, g, b, a);
	}

	void SetColor(const Color4f& color)
	{
		glColor4f(color.r, color.g, color.b, color.a);
	}

	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINES);
		{
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
		}
		glEnd();
	}

	void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth)
	{
		DrawLine(p1.x, p1.y, p2.x, p2.y, lineWidth);
	}

	void DrawRect(float left, float bottom, float width, float height, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(left, bottom);
			glVertex2f(left + width, bottom);
			glVertex2f(left + width, bottom + height);
			glVertex2f(left, bottom + height);
		}
		glEnd();
	}

	void DrawTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
			glVertex2f(p3.x, p3.y);
		}
		glEnd();
	}

	void FillTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3)
	{
		glBegin(GL_TRIANGLES);
		{
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
			glVertex2f(p3.x, p3.y);
		}
		glEnd();
	}

	void DrawRect(const Point2f& bottomLeft, float width, float height, float lineWidth)
	{
		DrawRect(bottomLeft.x, bottomLeft.y, width, height, lineWidth);
	}

	void DrawRect(const Rectf& rect, float lineWidth)
	{
		DrawRect(rect.left, rect.bottom, rect.width, rect.height, lineWidth);
	}

	void FillRect(float left, float bottom, float width, float height)
	{
		glBegin(GL_POLYGON);
		{
			glVertex2f(left, bottom);
			glVertex2f(left + width, bottom);
			glVertex2f(left + width, bottom + height);
			glVertex2f(left, bottom + height);
		}
		glEnd();
	}

	void FillRect(const Point2f& bottomLeft, float width, float height)
	{
		FillRect(bottomLeft.x, bottomLeft.y, width, height);
	}

	void FillRect(const Rectf& rect)
	{
		FillRect(rect.left, rect.bottom, rect.width, rect.height);
	}

	void DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth)
	{
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
		}
		glEnd();
	}

	void DrawEllipse(const Point2f& center, float radX, float radY, float lineWidth)
	{
		DrawEllipse(center.x, center.y, radX, radY, lineWidth);
	}

	void DrawEllipse(const Ellipsef& ellipse, float lineWidth)
	{
		DrawEllipse(ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY, lineWidth);
	}

	void FillEllipse(float centerX, float centerY, float radX, float radY)
	{
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glBegin(GL_POLYGON);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
		}
		glEnd();
	}

	void FillEllipse(const Ellipsef& ellipse)
	{
		FillEllipse(ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY);
	}

	void FillEllipse(const Point2f& center, float radX, float radY)
	{
		FillEllipse(center.x, center.y, radX, radY);
	}

	void DrawArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle, float lineWidth)
	{
		if (fromAngle > tillAngle)
		{
			return;
		}

		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glLineWidth(lineWidth);
		glBegin(GL_LINE_STRIP);
		{
			for (float angle = fromAngle; angle < tillAngle; angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
			glVertex2f(centerX + radX * float(cos(tillAngle)), centerY + radY * float(sin(tillAngle)));
		}
		glEnd();

	}

	void DrawArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth)
	{
		DrawArc(center.x, center.y, radX, radY, fromAngle, tillAngle, lineWidth);
	}

	void FillArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle)
	{
		if (fromAngle > tillAngle)
		{
			return;
		}
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glBegin(GL_POLYGON);
		{
			glVertex2f(centerX, centerY);
			for (float angle = fromAngle; angle < tillAngle; angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
			glVertex2f(centerX + radX * float(cos(tillAngle)), centerY + radY * float(sin(tillAngle)));
		}
		glEnd();
	}

	void FillArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle)
	{
		FillArc(center.x, center.y, radX, radY, fromAngle, tillAngle);
	}

	void DrawPolygon(const std::vector<Point2f>& vertices, bool closed, float lineWidth)
	{
		DrawPolygon(vertices.data(), vertices.size(), closed, lineWidth);
	}

	void DrawPolygon(const Point2f* pVertices, size_t nrVertices, bool closed, float lineWidth)
	{
		glLineWidth(lineWidth);
		closed ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
		{
			for (size_t idx{ 0 }; idx < nrVertices; ++idx)
			{
				glVertex2f(pVertices[idx].x, pVertices[idx].y);
			}
		}
		glEnd();
	}

	void FillPolygon(const std::vector<Point2f>& vertices)
	{
		FillPolygon(vertices.data(), vertices.size());
	}

	void FillPolygon(const Point2f* pVertices, size_t nrVertices)
	{
		glBegin(GL_POLYGON);
		{
			for (size_t idx{ 0 }; idx < nrVertices; ++idx)
			{
				glVertex2f(pVertices[idx].x, pVertices[idx].y);
			}
		}
		glEnd();
	}
#pragma endregion OpenGLDrawFunctionality

#pragma region CollisionFunctionality
	bool utils::IsPointInRect(const Point2f& p, const Rectf& r)
	{
		return (p.x >= r.left &&
			p.x <= r.left + r.width &&
			p.y >= r.bottom &&
			p.y <= r.bottom + r.height);
	}
	float GetDistance(float x1, float y1, float x2, float y2)
	{
		return float(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
	}

	float GetDistance(const Point2f& p1, const Point2f& p2)
	{
		return GetDistance(p1.x, p1.y, p2.x, p2.y);
	}

	bool IsPointInCircle(const Circlef& circle, const Point2f& point)
	{
		float distance = GetDistance(circle.center, point);
		if (distance > circle.radius)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool IsOverlapping(const Rectf& r1, const Rectf& r2)
	{
		// If one rectangle is on left side of the other
		if (r1.left + r1.width < r2.left || r2.left + r2.width < r1.left)
		{
			return false;
		}

		// If one rectangle is under the other
		if (r1.bottom > r2.bottom + r2.height || r2.bottom > r1.bottom + r1.height)
		{
			return false;
		}

		return true;
	}
	bool IsOverlapping(const Circlef& circle1, const Circlef& circle2)
	{
		// Two circles intersect if, and only if, 
		// the distance between their centers is less than the sum  of their radii.
		float distance{ utils::GetDistance(circle1.center, circle2.center) };
		if (distance > circle1.radius + circle2.radius)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool IsOverlapping(const Rectf& rect, const Circlef& circle)
	{
		// Later
		return false;
	}
#pragma endregion CollisionFunctionality

#pragma region MathVector
	Vector2f Add(const Vector2f& v1, const Vector2f& v2)
	{
		return Vector2f{ v1.x + v2.x, v1.y + v2.y };
	}
	Vector2f Subtract(const Vector2f& v1, const Vector2f& v2)
	{
		return Vector2f{ v1.x - v2.x, v1.y - v2.y };
	}
	float DotProduct(const Vector2f& v1, const Vector2f& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	float CrossProduct(const Vector2f& v1, const Vector2f& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;

		//Reference https ://www.physicsforums.com/threads/2-dimensional-cross-product.68984/
	}
	std::string ToString(const Vector2f& v)
	{
		return std::string("[") + std::to_string(v.x) + ",  " + std::to_string(v.y) + "]";
	}
	Vector2f Scale(const Vector2f& v, float scalar)
	{
		return Vector2f{ v.x * scalar,v.y * scalar };
	}
	float Length(const Vector2f& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}
	Vector2f Normalize(const Vector2f& v)
	{
		// modify the copy !
		float length = Length(v);
		if (length > 0.001)
		{
			return{ v.x / length, v.y / length };
		}
		else
		{
			return Vector2f{};
		}
	}
	float AngleBetween(const Vector2f& v1, const Vector2f& v2)
	{
		//https://www.mathworks.com/matlabcentral/answers/101590-how-can-i-determine-the-angle-between-two-vectors-in-matlab
		//atan2(x1*y2-x2*y1,x1*x2+y1*y2)
		return atan2(CrossProduct(v1, v2), DotProduct(v1, v2));
	}
	void DrawVector(const Vector2f& v, const Point2f& startPoint, float lineWidth)
	{
		Point2f endPoint{ startPoint.x + v.x, startPoint.y + v.y };
		float angle = atan2(endPoint.y - startPoint.y, endPoint.x - startPoint.x);
		angle += float(M_PI - M_PI / 6);
		float radius{ 10.0f };
		Point2f arrowPoint1{ endPoint.x + radius * cosf(angle), endPoint.y + radius * sinf(angle) };
		angle += float(M_PI / 3);
		Point2f arrowPoint2{ endPoint.x + radius * cosf(angle), endPoint.y + radius * sinf(angle) };
		glLineWidth(lineWidth);
		glBegin(GL_LINES);
		{
			glVertex2f(startPoint.x, startPoint.y);
			glVertex2f(endPoint.x, endPoint.y);
			// arrow
			glVertex2f(endPoint.x, endPoint.y);
			glVertex2f(arrowPoint1.x, arrowPoint1.y);

			glVertex2f(endPoint.x, endPoint.y);
			glVertex2f(arrowPoint2.x, arrowPoint2.y);
		}
		glEnd();

	}
	bool AreEqual(const Vector2f& v1, const Vector2f& v2)
	{
		float epsilon{ 0.001f };
		return (abs(v1.x - v2.x) < epsilon) && (abs(v1.y - v2.y) < epsilon);
	}

#pragma endregion  MathVector



}
