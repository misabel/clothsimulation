#ifndef INCLUDED_CATMULLROM_CURVE_EVALUATOR_H
#define INCLUDED_CATMULLROM_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  
#pragma warning(disable : 4018)
#pragma warning(disable : 4267)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)

#include "curveevaluator.h"

class CatmullRomCurveEvaluator : public CurveEvaluator
{
public:
		Animator::Point calculateBezierPoint(const std::vector<Animator::Point>& bezierPoints,
								   			 const float t) const;

		std::vector<Animator::Point> getBezierPoints(
			const std::vector<Animator::Point>& points) const;

        void evaluateCurve(const std::vector<Animator::Point>& ptvCtrlPts,
                		   std::vector<Animator::Point>& ptvEvaluatedCurvePts,
						   const float& fAniLength, 
						   const bool& bWrap,
						   const float default_val) const;
};

#endif
