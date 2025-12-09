#include "MathTool.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

using namespace std;

// 定义点结构
struct Point {
    double x;
    double y;
    double z;
};

// 计算向量
Point vectorFromPoints(const Point& p1, const Point& p2) {
    return { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
}

// 计算向量长度
double vectorLength(const Point& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// 向量归一化
Point normalizeVector(const Point& v) {
    double len = vectorLength(v);
    if (len == 0) return { 0, 0, 0 }; // 避免除以零
    return { v.x / len, v.y / len, v.z / len };
}

// 向量叉积
Point crossProduct(const Point& v1, const Point& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

// 向量点积
double dotProduct(const Point& v1, const Point& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Transform point E to the coordinate system defined by A and B
Point transformPoint(const Point& A, const Point& B, const Point& E) {
    // 1. 计算向量 C (B - A)
    Point C = vectorFromPoints(A, B);

    // 2. 计算坐标系 D 的 z 轴单位向量 (即 C 的单位向量)
    Point Z = normalizeVector(C);

    // 3. 找到一个与 Z 不平行的向量 (例如 (1, 0, 0))
    Point temp = { 1, 0, 0 };
    if (std::abs(dotProduct(Z, temp)) > 1 - 1e-6) { // Check for near-parallelism
        temp = { 0, 1, 0 };
    }

    // 4. Calculate the Y-axis (orthogonal to Z and temp)
    Point Y = normalizeVector(crossProduct(temp, Z));

    // 5. Calculate the X-axis (orthogonal to Y and Z)
    Point X = crossProduct(Y, Z);

    // 6. Calculate vector AE (from A to E)
    Point AE = vectorFromPoints(A, E);

    // 7. Project AE onto the new axes to get the coordinates in the new system
    double E_local_x = dotProduct(AE, X);
    double E_local_y = dotProduct(AE, Y);
    double E_local_z = dotProduct(AE, Z);

    return { E_local_x, E_local_y, E_local_z };
}

void MathTool::GetCircleFromThreePoint(double x1, double y1, double x2, double y2, double x3, double y3, double& cx, double& cy, double& r)
{
    double x12 = x1 - x2;
    double x13 = x1 - x3;

    double y12 = y1 - y2;
    double y13 = y1 - y3;

    double y31 = y3 - y1;
    double y21 = y2 - y1;

    double x31 = x3 - x1;
    double x21 = x2 - x1;

    // x1^2 - x3^2
    double sx13 = pow(x1, 2) - pow(x3, 2);

    // y1^2 - y3^2
    double sy13 = pow(y1, 2) - pow(y3, 2);

    double sx21 = pow(x2, 2) - pow(x1, 2);
    double sy21 = pow(y2, 2) - pow(y1, 2);

    double f = ((sx13) * (x12)
        +(sy13) * (x12)
        +(sx21) * (x13)
        +(sy21) * (x13))
        / (2 * ((y31) * (x12)-(y21) * (x13)));
    double g = ((sx13) * (y12)
        +(sy13) * (y12)
        +(sx21) * (y13)
        +(sy21) * (y13))
        / (2 * ((x31) * (y12)-(x21) * (y13)));

    double c = -pow(x1, 2) - pow(y1, 2) - 2 * g * x1 - 2 * f * y1;

    // eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
    // where centre is (h = -g, k = -f) and radius r
    // as r^2 = h^2 + k^2 - c
    double h = -g;
    double k = -f;
    double sqr_of_r = h * h + k * k - c;

    // r is the radius
    double r1 = sqrt(sqr_of_r);

    cx = h;
    cy = k;
    r = r1;
    //cout << "Centre = (" << h << ", " << k << ")" << endl;
    //cout << "Radius = " << r;
}

// 我有两个点A和B，做A到B的向量C。给定平面为垂直于向量C的平面，且原点为A。这个坐标系设为D，XY平面为上述垂直于向量C的平面。 此时再给一个点E，把E转换为坐标系D上的坐标. 请用c++代码，不要用外部库
cb::Vector3D MathTool::TransformPoint(const cb::Vector3D& p, const cb::Vector3D& pFrom, const cb::Vector3D& pTo)
{
    Point point_e = { (double)p.x(), (double)p.y(), (double)p.z() };
    Point point_a = { (double)pFrom.x(), (double)pFrom.y(), (double)pFrom.z() };
    Point point_b = { (double)pTo.x(), (double)pTo.y(), (double)pTo.z() };
    Point r = transformPoint(point_a, point_b, point_e);
	return cb::Vector3D(r.x, r.y, r.z);
}


