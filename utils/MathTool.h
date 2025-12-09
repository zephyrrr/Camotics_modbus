#pragma once

#include <cbang/Math.h>
#include <cbang/geom/Vector.h>
#include <vector>

class MathTool
{
public:
	static void GetCircleFromThreePoint(double x1, double y1, double x2, double y2, double x3, double y3, double& cx, double& cy, double& r);
	static cb::Vector3D TransformPoint(const cb::Vector3D& p, const cb::Vector3D& pFrom, const cb::Vector3D& pTo);
};

