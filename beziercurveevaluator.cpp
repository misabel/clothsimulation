#include "beziercurveevaluator.h"
#include <cassert>
using namespace Animator;

Point BezierCurveEvaluator::calculateBezierPoint(const std::vector<Point>& ptvCtrlPts, 
												 const int i,
												 const float t) const
{
	float u   = 1 - t;
	float tt  = t * t;
	float uu  = u * u;
	float ttt = tt * t;
	float uuu = uu * u;

	float x = (uuu*ptvCtrlPts[i].x) + (3*t*uu*ptvCtrlPts[i+1].x) + (3*tt*u*ptvCtrlPts[i+2].x) + (ttt*ptvCtrlPts[i+3].x);
	float y = (uuu*ptvCtrlPts[i].y) + (3*t*uu*ptvCtrlPts[i+1].y) + (3*tt*u*ptvCtrlPts[i+2].y) + (ttt*ptvCtrlPts[i+3].y);

	return Point(x, y);
}

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
										 std::vector<Point>& ptvEvaluatedCurvePts, 
										 const float& fAniLength, 
										 const bool& bWrap,
										 const float default_val) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	int last = 0;
	ptvEvaluatedCurvePts.clear();

	while (last < iCtrlPtCount) 
	{
		// we need at least 4 control points to make a single bezier curve
		if (last + 3 < iCtrlPtCount)
		{
			// sampling from 0 to 1 by 0.01
			for (float sample = 0; sample < 1; sample += 0.01)
			{
		    	ptvEvaluatedCurvePts.push_back(calculateBezierPoint(ptvCtrlPts, last, sample));
			}

			last += 3;
		}
		// for less than 4 control points, it behaves as a linear curve
		else 
		{
			ptvEvaluatedCurvePts.push_back(ptvCtrlPts[last]);
			last++;
		}
	}

	// // we need at least 4 control points to make a single bezier curve
	// if (iCtrlPtCount >= 4) 
	// {
	// 	for (float sample = 0; sample < 1; sample += 0.01)
	// 	{
	// 	    ptvEvaluatedCurvePts.push_back(calculateBezierPoint(ptvCtrlPts, 0, sample));
	// 	}
	// }
	// // for less than 4 control points, it behaves as a linear curve
	// else {
	// 	ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
	// }

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
