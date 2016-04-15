#include "bsplinecurveevaluator.h"
#include <cassert>
using namespace Animator;

Point BSplineCurveEvaluator::calculateBezierPoint(const std::vector<Point>& bezierPoints, 
												 	 const float t) const
{
	float u   = 1 - t;
	float tt  = t * t;
	float uu  = u * u;
	float ttt = tt * t;
	float uuu = uu * u;

	float x = (uuu*bezierPoints[0].x) + (3*t*uu*bezierPoints[1].x) + (3*tt*u*bezierPoints[2].x) + (ttt*bezierPoints[3].x);
	float y = (uuu*bezierPoints[0].y) + (3*t*uu*bezierPoints[1].y) + (3*tt*u*bezierPoints[2].y) + (ttt*bezierPoints[3].y);

	return Point(x, y);
}

std::vector<Animator::Point> BSplineCurveEvaluator::getBezierPoints(
	const std::vector<Animator::Point>& points) const
{
	std::vector<Animator::Point> bezierPoints;
	bezierPoints.reserve(4);

	// std::cout << "\nControl Points\n";
	// std::cout << "(" << points[0].x << "," << points[0].y << ")\n";
	// std::cout << "(" << points[1].x << "," << points[1].y << ")\n";
	// std::cout << "(" << points[2].x << "," << points[2].y << ")\n";
	// std::cout << "(" << points[3].x << "," << points[3].y << ")\n";

	// Convert to Bezier Points
	bezierPoints.push_back(Point(
		((1.0/6.0) * points[0].x) + ((2.0/3.0) * points[1].x) + ((1.0/6.0) * points[2].x),
		((1.0/6.0) * points[0].y) + ((2.0/3.0) * points[1].y) + ((1.0/6.0) * points[2].y)));
	bezierPoints.push_back(Point(
		((2.0/3.0) * points[1].x) + ((1.0/3.0) * points[2].x),
		((2.0/3.0) * points[1].y) + ((1.0/3.0) * points[2].y)));
	bezierPoints.push_back(Point(
		((1.0/3.0) * points[1].x) + ((2.0/3.0) * points[2].x),
		((1.0/3.0) * points[1].y) + ((2.0/3.0) * points[2].y)));
	bezierPoints.push_back(Point(
		((1.0/6.0) * points[1].x) + ((2.0/3.0) * points[2].x) + ((1.0/6.0) * points[3].x),
		((1.0/6.0) * points[1].y) + ((2.0/3.0) * points[2].y) + ((1.0/6.0) * points[3].y)));

	// std::cout << "\nBezier Points\n";
	// std::cout << "(" << bezierPoints[0].x << "," << bezierPoints[0].y << ")\n";
	// std::cout << "(" << bezierPoints[1].x << "," << bezierPoints[1].y << ")\n";
	// std::cout << "(" << bezierPoints[2].x << "," << bezierPoints[2].y << ")\n";
	// std::cout << "(" << bezierPoints[3].x << "," << bezierPoints[3].y << ")\n";

	return bezierPoints;
}

void BSplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
											 std::vector<Point>& ptvEvaluatedCurvePts, 
											 const float& fAniLength, 
											 const bool& bWrap,
											 const float default_val) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();

	if (iCtrlPtCount > 2) 
	{
		std::vector<Animator::Point> points;
		points.reserve(4);

		for (int i = 0; i <= iCtrlPtCount; i++) 
		{
			points.clear();
			// Triplicate first endpoint
			if (i == 0) 
			{
				points.push_back(ptvCtrlPts[i]);
				points.push_back(ptvCtrlPts[i]);
				points.push_back(ptvCtrlPts[i]);
				points.push_back(ptvCtrlPts[i + 1]);
			}
			else if (i == 1) 
			{
				points.push_back(ptvCtrlPts[i - 1]);
				points.push_back(ptvCtrlPts[i - 1]);
				points.push_back(ptvCtrlPts[i]);
				points.push_back(ptvCtrlPts[i + 1]);
			}
			// Triplicate last endpoint
			else if (i == iCtrlPtCount -1)
			{
				points.push_back(ptvCtrlPts[i - 2]);
				points.push_back(ptvCtrlPts[i - 1]);
				points.push_back(ptvCtrlPts[i]);
				points.push_back(ptvCtrlPts[i]);
			}
			else if (i == iCtrlPtCount)
			{
				points.push_back(ptvCtrlPts[i - 2]);
				points.push_back(ptvCtrlPts[i - 1]);
				points.push_back(ptvCtrlPts[i - 1]);
				points.push_back(ptvCtrlPts[i - 1]);
			}
			else
			{
				points.push_back(ptvCtrlPts[i - 2]);
				points.push_back(ptvCtrlPts[i - 1]);
				points.push_back(ptvCtrlPts[i]);
				points.push_back(ptvCtrlPts[i + 1]);
			}

			std::vector<Animator::Point> bezierPoints = getBezierPoints(points);
			// sampling from 0 to 1 by 0.01
			for (float sample = 0; sample < 1; sample += 0.01)
			{
		    	ptvEvaluatedCurvePts.push_back(calculateBezierPoint(bezierPoints, sample));
			}
		}
	}
	else {
		ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
	}

	float x = 0.0;
	float y1;

	if (bWrap) {
		// if wrapping is on, interpolate the y value at xmin and
		// xmax so that the slopes of the lines adjacent to the
		// wraparound are equal.

		if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
			y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) + 
				  ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
				 (ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
		}
		else 
			y1 = ptvCtrlPts[0].y;
	}
	else {
		// if wrapping is off, make the first and last segments of
		// the curve horizontal.

		y1 = ptvCtrlPts[0].y;
    }

	ptvEvaluatedCurvePts.push_back(Point(x, y1));

	/// set the endpoint based on the wrap flag.
	float y2;
    x = fAniLength;
    if (bWrap)
		y2 = y1;
    else
		y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

	ptvEvaluatedCurvePts.push_back(Point(x, y2));
}
