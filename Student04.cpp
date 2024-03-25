// @author ~ Marek Čupr (xcuprm01)
// @body ~ 0/3 (kod jsem doplnil mimo cvika, ale mel by byt dobre)

#include "Student.h"

#include <osg/Plane>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

osg::Vec3d projectOnPlane(const osg::Vec3d& vector, const osg::Vec3d& planeNormal)
{
    auto pp = vector - planeNormal * (vector * planeNormal);
    //get rid of round-off error
    pp[0] = std::abs(pp[0]) < 1e-6 ? 0 : pp[0];
    pp[1] = std::abs(pp[1]) < 1e-6 ? 0 : pp[1];
    pp[2] = std::abs(pp[2]) < 1e-6 ? 0 : pp[2];

    return pp;
}

double dot(const osg::Vec3d& u, const osg::Vec3d& v)
{
    auto d = u * v;

    //handle rounding errors
    d = std::clamp(d, -1.0, 1.0);

    return d;
}

osg::Vec3d cross(const osg::Vec3d& u, const osg::Vec3d& v)
{
    return u ^ v;
}

void normalize(osg::Vec3d& v)
{
    v.normalize();
}

osg::Vec3d normalized(const osg::Vec3d& v)
{
    osg::Vec3d u(v);
    u.normalize();

    return u;
}

osg::Matrix inverse(const osg::Matrix& m)
{
    return osg::Matrix::inverse(m);
}

#if USE_STUDENT_SOLUTION
osg::Matrix getScaleMatrix(const osg::Vec3d& scale)
{
    //Task 1 - fill in the scale matrix

    // vyplneni scale matice ---> jenom opiseme z tabule
    // NEZAPOMENOUT NA '()' za .x, .y a .z (jinak to nebude fungovat)
    osg::Matrix m(scale.x(), 0, 0, 0,
                  0, scale.y(), 0, 0, 
                  0, 0, scale.z(), 0,
                  0, 0, 0, 1);

    return m;
}

osg::Matrix getTranslationMatrix(const osg::Vec3d& translation)
{
    //Task 2 - fill in the translation matrix

    // vyplneni translate matice ---> jenom opiseme z tabule
    // NEZAPOMENOUT NA '()' za .x, .y a .z (jinak to nebude fungovat)
    osg::Matrix m(1, 0, 0, 0,
                  0, 1, 0, 0, 
                  0, 0, 1, 0,
                  translation.x(), translation.y(), translation.z(), 1);

    return m;
}

osg::Matrix rotateAroundX(double angle)
{

    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    //Task 3 - fill in the rotation matrix around X axis

    // vyplneni matice rotace kolem osy x ---> jenom opiseme z tabule

    osg::Matrix m(1, 0, 0, 0,
                  0, cosa, sina, 0, 
                  0, -sina, cosa, 0,
                  0, 0, 0, 1);

    return m;
}

osg::Matrix rotateAroundY(double angle)
{
    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    //Task 4 - fill in the rotation matrix around Y axis

    // vyplneni matice rotace kolem osy y ---> jenom opiseme z tabule

    osg::Matrix m(cosa, 0, -sina, 0,
                  0, 1, 0, 0, 
                  sina, 0, cosa, 0,
                  0, 0, 0, 1);

    return m;
}

osg::Matrix rotateAroundZ(double angle)
{
    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    //Task 5 - fill in the rotation matrix around Z axis

    // vyplneni matice rotace kolem osy z ---> jenom opiseme z tabule


    osg::Matrix m(cosa, sina, -sina, 0,
                  -sina, cosa, 0, 0, 
                  0, 0, 1, 0,
                  0, 0, 0, 1);

    return m;
}

double angleBetweenVectors(osg::Vec3d u, osg::Vec3d v)
{
    if (u.length2() == 0 || v.length2() == 0)
    {
        return 0.0;
    }

    //Task 6

    //Normalize both vectors - method normalize()
    
    normalize(u); // normalizace vektoru u
    normalize(v); // normalizace vektoru v

    //Compute cosine of the angle between the vectors using the dot product - function dot
    double angle_cos = dot(u, v); // cosinus uhlu

    //Return the angle using arcus cosine - function std::acos()
    double angle = std::acos(angle_cos); // uhel
    return angle;
}

osg::Matrix getRotationMatrix(const osg::Vec3d& fromVector, const osg::Vec3d& toVector)
{
    const osg::Vec3d xAxis(1, 0, 0);
    const osg::Vec3d yAxis(0, 1, 0);
    const osg::Vec3d zAxis(0, 0, 1);

    //Task 7 - compute the rotation matrix around arbitrary axis
    //Compute the angle between input vectors - function angleBetweenVectors
    double angle_vectors = angleBetweenVectors(fromVector, toVector);

    //7a - compute the rotation axis using the cross product - function cross
    osg::Vec3d rot_axis = cross(fromVector, toVector);
    
    //7b - project rotation axis into XY plane - you can use function projectOnPlane, or compute it by yourself
    osg::Vec3d project_rot_XY = projectOnPlane(rot_axis, zAxis);
     
    //7c - compute the angle between projected rotation axis and X axis
    double angle_axis_x = angleBetweenVectors(xAxis, project_rot_XY);
    
    //7d - compute rotation around Z axis
    osg::Matrix rotate_z = rotateAroundZ(angle_axis_x);

    //Rotate the rotation axis into XZ plane (vector-matrix multiplication)

    //7e - compute angle between rotation axis in XZ and X axis and compute rotation matrix around Y corrently, hint - for Pi use M_PI
    osg::Vec3d project_rot_XZ = projectOnPlane(rot_axis, yAxis);

    double angle_rot_XZ = angleBetweenVectors(xAxis, project_rot_XZ);

    if (project_rot_XZ.z() < 0){
        angle_rot_XZ = 2 * M_PI - angle_rot_XZ;
    }

    osg::Matrix rotate_y = rotateAroundY(angle_rot_XZ);

    //7f - compute the desired rotation around X axis
    osg::Matrix rotate_x = rotateAroundX(angle_vectors);

    //7g - compute and return the final rotation - for matrix inverse use function inverse
    osg::Matrix result;

    osg::Matrix inverz_z = inverse(rotate_z);
    osg::Matrix inverz_y = inverse(rotate_y);

    result = rotate_z * rotate_y * rotate_x * inverz_y * inverz_y;
    return result;
}

#else

osg::Matrix getScaleMatrix(const osg::Vec3d& scale)
{
    return osg::Matrix::scale(scale);
}

osg::Matrix getTranslationMatrix(const osg::Vec3d& translation)
{
    return osg::Matrix::translate(translation);
}

osg::Matrix getRotationMatrix(const osg::Vec3d& fromVector, const osg::Vec3d& toVector)
{
    osg::Quat q;
    q.makeRotate(fromVector, toVector);

    return osg::Matrix(q);
}

osg::Matrix rotateAroundX(double angle)
{
    return {};
}

osg::Matrix rotateAroundY(double angle)
{
    return {};
}

osg::Matrix rotateAroundZ(double angle)
{
    return {};
}

double angleBetweenVectors(osg::Vec3d u, osg::Vec3d v)
{
    return 0.0;
}
#endif
