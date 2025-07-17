#pragma once

#define _USE_MATH_DEFINES
#include <math.h>


struct Vec2
{
    union
    {
        struct
        {
            float X;
            float Y;
        };
        float data[2];
    };


    /**
     * Constructors.
     */
    inline Vec2();
    inline Vec2(float data[]);
    inline Vec2(float value);
    inline Vec2(float x, float y);


    /**
     * Constants for common vectors.
     */
    static inline Vec2 Zero();
    static inline Vec2 One();
    static inline Vec2 Right();
    static inline Vec2 Left();
    static inline Vec2 Up();
    static inline Vec2 Down();


    /**
     * Returns the angle between two vectors in radians.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A scalar value.
     */
    static inline float Angle(Vec2 a, Vec2 b);

    /**
     * Returns a vector with its magnitude clamped to maxLength.
     * @param vector: The target vector.
     * @param maxLength: The maximum length of the return vector.
     * @return: A new vector.
     */
    static inline Vec2 ClampMagnitude(Vec2 vector, float maxLength);

    /**
     * Returns the component of a in the direction of b (scalar projection).
     * @param a: The target vector.
     * @param b: The vector being compared against.
     * @return: A scalar value.
     */
    static inline float Component(Vec2 a, Vec2 b);

    /**
     * Returns the distance between a and b.
     * @param a: The first point.
     * @param b: The second point.
     * @return: A scalar value.
     */
    static inline float Distance(Vec2 a, Vec2 b);

    /**
     * Returns the dot product of two vectors.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static inline float Dot(Vec2 lhs, Vec2 rhs);

    /**
     * Converts a polar representation of a vector into cartesian
     * coordinates.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle from the X axis.
     * @return: A new vector.
     */
    static inline Vec2 FromPolar(float rad, float theta);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line. The vector is clamped to never go beyond the end points.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1].
     * @return: A new vector.
     */
    static inline Vec2 Lerp(Vec2 a, Vec2 b, float t);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1] (no actual bounds).
     * @return: A new vector.
     */
    static inline Vec2 LerpUnclamped(Vec2 a, Vec2 b, float t);

    /**
     * Returns the magnitude of a vector.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float Magnitude(Vec2 v);

    /**
     * Returns a vector made from the largest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vec2 Max(Vec2 a, Vec2 b);

    /**
     * Returns a vector made from the smallest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vec2 Min(Vec2 a, Vec2 b);

    /**
     * Returns a vector "maxDistanceDelta" units closer to the target. This
     * interpolation is in a straight line, and will not overshoot.
     * @param current: The current position.
     * @param target: The destination position.
     * @param maxDistanceDelta: The maximum distance to move.
     * @return: A new vector.
     */
    static inline Vec2 MoveTowards(Vec2 current, Vec2 target,
                               float maxDistanceDelta);

    /**
     * Returns a new vector with magnitude of one.
     * @param v: The vector in question.
     * @return: A new vector.
     */
    static inline Vec2 Normalized(Vec2 v);

    /**
     * Creates a new coordinate system out of the two vectors.
     * Normalizes "normal" and normalizes "tangent" and makes it orthogonal to
     * "normal"..
     * @param normal: A reference to the first axis vector.
     * @param tangent: A reference to the second axis vector.
     */
    static inline void OrthoNormalize(Vec2 &normal, Vec2 &tangent);

    /**
     * Returns the vector projection of a onto b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vec2 Project(Vec2 a, Vec2 b);

    /**
     * Returns a vector reflected about the provided line.
     * This behaves as if there is a plane with the line as its normal, and the
     * vector comes in and bounces off this plane.
     * @param vector: The vector traveling inward at the imaginary plane.
     * @param line: The line about which to reflect.
     * @return: A new vector pointing outward from the imaginary plane.
     */
    static inline Vec2 Reflect(Vec2 vector, Vec2 line);

    /**
     * Returns the vector rejection of a on b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vec2 Reject(Vec2 a, Vec2 b);

    /**
     * Rotates vector "current" towards vector "target" by "maxRadiansDelta".
     * This treats the vectors as directions and will linearly interpolate
     * between their magnitudes by "maxMagnitudeDelta". This function does not
     * overshoot. If a negative delta is supplied, it will rotate away from
     * "target" until it is pointing the opposite direction, but will not
     * overshoot that either.
     * @param current: The starting direction.
     * @param target: The destination direction.
     * @param maxRadiansDelta: The maximum number of radians to rotate.
     * @param maxMagnitudeDelta: The maximum delta for magnitude interpolation.
     * @return: A new vector.
     */
    static inline Vec2 RotateTowards(Vec2 current, Vec2 target,
                                 float maxRadiansDelta,
                                 float maxMagnitudeDelta);

    /**
     * Multiplies two vectors component-wise.
     * @param a: The lhs of the multiplication.
     * @param b: The rhs of the multiplication.
     * @return: A new vector.
     */
    static inline Vec2 Scale(Vec2 a, Vec2 b);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is clamped to 0 <= t <= 1.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vec2 Slerp(Vec2 a, Vec2 b, float t);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is unclamped.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vec2 SlerpUnclamped(Vec2 a, Vec2 b, float t);

    /**
     * Returns the squared magnitude of a vector.
     * This is useful when comparing relative lengths, where the exact length
     * is not important, and much time can be saved by not calculating the
     * square root.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float SqrMagnitude(Vec2 v);

    /**
     * Calculates the polar coordinate space representation of a vector.
     * @param vector: The vector to convert.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle from the X axis.
     */
    static inline void ToPolar(Vec2 vector, float &rad, float &theta);


    /**
     * Operator overloading.
     */
    inline struct Vec2& operator+=(const float rhs);
    inline struct Vec2& operator-=(const float rhs);
    inline struct Vec2& operator*=(const float rhs);
    inline struct Vec2& operator/=(const float rhs);
    inline struct Vec2& operator+=(const Vec2 rhs);
    inline struct Vec2& operator-=(const Vec2 rhs);
};

inline Vec2 operator-(Vec2 rhs);
inline Vec2 operator+(Vec2 lhs, const float rhs);
inline Vec2 operator-(Vec2 lhs, const float rhs);
inline Vec2 operator*(Vec2 lhs, const float rhs);
inline Vec2 operator/(Vec2 lhs, const float rhs);
inline Vec2 operator+(const float lhs, Vec2 rhs);
inline Vec2 operator-(const float lhs, Vec2 rhs);
inline Vec2 operator*(const float lhs, Vec2 rhs);
inline Vec2 operator/(const float lhs, Vec2 rhs);
inline Vec2 operator+(Vec2 lhs, const Vec2 rhs);
inline Vec2 operator-(Vec2 lhs, const Vec2 rhs);
inline bool operator==(const Vec2 lhs, const Vec2 rhs);
inline bool operator!=(const Vec2 lhs, const Vec2 rhs);



/*******************************************************************************
 * Implementation
 */

Vec2::Vec2() : X(0), Y(0) {}
Vec2::Vec2(float data[]) : X(data[0]), Y(data[1]) {}
Vec2::Vec2(float value) : X(value), Y(value) {}
Vec2::Vec2(float x, float y) : X(x), Y(y) {}


Vec2 Vec2::Zero() { return Vec2(0, 0); }
Vec2 Vec2::One() { return Vec2(1, 1); }
Vec2 Vec2::Right() { return Vec2(1, 0); }
Vec2 Vec2::Left() { return Vec2(-1, 0); }
Vec2 Vec2::Up() { return Vec2(0, 1); }
Vec2 Vec2::Down() { return Vec2(0, -1); }


float Vec2::Angle(Vec2 a, Vec2 b)
{
    float v = Dot(a, b) / (Magnitude(a) * Magnitude(b));
    v = fmax(v, -1.0);
    v = fmin(v, 1.0);
    return acos(v);
}

Vec2 Vec2::ClampMagnitude(Vec2 vector, float maxLength)
{
    float length = Magnitude(vector);
    if (length > maxLength)
        vector *= maxLength / length;
    return vector;
}

float Vec2::Component(Vec2 a, Vec2 b)
{
    return Dot(a, b) / Magnitude(b);
}

float Vec2::Distance(Vec2 a, Vec2 b)
{
    return Vec2::Magnitude(a - b);
}

float Vec2::Dot(Vec2 lhs, Vec2 rhs)
{
    return lhs.X * rhs.X + lhs.Y * rhs.Y;
}

Vec2 Vec2::FromPolar(float rad, float theta)
{
    Vec2 v = Vec2();
    v.X = rad * cos(theta);
    v.Y = rad * sin(theta);
    return v;
}

Vec2 Vec2::Lerp(Vec2 a, Vec2 b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return LerpUnclamped(a, b, t);
}

Vec2 Vec2::LerpUnclamped(Vec2 a, Vec2 b, float t)
{
    return (b - a) * t + a;
}

float Vec2::Magnitude(Vec2 v)
{
    return sqrt(SqrMagnitude(v));
}

Vec2 Vec2::Max(Vec2 a, Vec2 b)
{
    float x = a.X > b.X ? a.X : b.X;
    float y = a.Y > b.Y ? a.Y : b.Y;
    return Vec2(x, y);
}

Vec2 Vec2::Min(Vec2 a, Vec2 b)
{
    float x = a.X > b.X ? b.X : a.X;
    float y = a.Y > b.Y ? b.Y : a.Y;
    return Vec2(x, y);
}

Vec2 Vec2::MoveTowards(Vec2 current, Vec2 target,
                             float maxDistanceDelta)
{
    Vec2 d = target - current;
    float m = Magnitude(d);
    if (m < maxDistanceDelta || m == 0)
        return target;
    return current + (d * maxDistanceDelta / m);
}

Vec2 Vec2::Normalized(Vec2 v)
{
    float mag = Magnitude(v);
    if (mag == 0)
        return Vec2::Zero();
    return v / mag;
}

void Vec2::OrthoNormalize(Vec2 &normal, Vec2 &tangent)
{
    normal = Normalized(normal);
    tangent = Reject(tangent, normal);
    tangent = Normalized(tangent);
}

Vec2 Vec2::Project(Vec2 a, Vec2 b)
{
    float m = Magnitude(b);
    return Dot(a, b) / (m * m) * b;
}

Vec2 Vec2::Reflect(Vec2 vector, Vec2 planeNormal)
{
    return vector - 2 * Project(vector, planeNormal);
}

Vec2 Vec2::Reject(Vec2 a, Vec2 b)
{
    return a - Project(a, b);
}

Vec2 Vec2::RotateTowards(Vec2 current, Vec2 target,
                               float maxRadiansDelta,
                               float maxMagnitudeDelta)
{
    float magCur = Magnitude(current);
    float magTar = Magnitude(target);
    float newMag = magCur + maxMagnitudeDelta *
        ((magTar > magCur) - (magCur > magTar));
    newMag = fmin(newMag, fmax(magCur, magTar));
    newMag = fmax(newMag, fmin(magCur, magTar));

    float totalAngle = Angle(current, target) - maxRadiansDelta;
    if (totalAngle <= 0)
        return Normalized(target) * newMag;
    else if (totalAngle >= M_PI)
        return Normalized(-target) * newMag;

    float axis = current.X * target.Y - current.Y * target.X;
    axis = axis / fabs(axis);
    if (!(1 - fabs(axis) < 0.00001))
        axis = 1;
    current = Normalized(current);
    Vec2 newVector = current * cos(maxRadiansDelta) +
        Vec2(-current.Y, current.X) * sin(maxRadiansDelta) * axis;
    return newVector * newMag;
}

Vec2 Vec2::Scale(Vec2 a, Vec2 b)
{
    return Vec2(a.X * b.X, a.Y * b.Y);
}

Vec2 Vec2::Slerp(Vec2 a, Vec2 b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return SlerpUnclamped(a, b, t);
}

Vec2 Vec2::SlerpUnclamped(Vec2 a, Vec2 b, float t)
{
    float magA = Magnitude(a);
    float magB = Magnitude(b);
    a /= magA;
    b /= magB;
    float dot = Dot(a, b);
    dot = fmax(dot, -1.0);
    dot = fmin(dot, 1.0);
    float theta = acos(dot) * t;
    Vec2 relativeVec = Normalized(b - a * dot);
    Vec2 newVec = a * cos(theta) + relativeVec * sin(theta);
    return newVec * (magA + (magB - magA) * t);
}

float Vec2::SqrMagnitude(Vec2 v)
{
    return v.X * v.X + v.Y * v.Y;
}

void Vec2::ToPolar(Vec2 vector, float &rad, float &theta)
{
    rad = Magnitude(vector);
    theta = atan2(vector.Y, vector.X);
}


struct Vec2& Vec2::operator+=(const float rhs)
{
    X += rhs;
    Y += rhs;
    return *this;
}

struct Vec2& Vec2::operator-=(const float rhs)
{
    X -= rhs;
    Y -= rhs;
    return *this;
}

struct Vec2& Vec2::operator*=(const float rhs)
{
    X *= rhs;
    Y *= rhs;
    return *this;
}

struct Vec2& Vec2::operator/=(const float rhs)
{
    X /= rhs;
    Y /= rhs;
    return *this;
}

struct Vec2& Vec2::operator+=(const Vec2 rhs)
{
    X += rhs.X;
    Y += rhs.Y;
    return *this;
}

struct Vec2& Vec2::operator-=(const Vec2 rhs)
{
    X -= rhs.X;
    Y -= rhs.Y;
    return *this;
}

Vec2 operator-(Vec2 rhs) { return rhs * -1; }
Vec2 operator+(Vec2 lhs, const float rhs) { return lhs += rhs; }
Vec2 operator-(Vec2 lhs, const float rhs) { return lhs -= rhs; }
Vec2 operator*(Vec2 lhs, const float rhs) { return lhs *= rhs; }
Vec2 operator/(Vec2 lhs, const float rhs) { return lhs /= rhs; }
Vec2 operator+(const float lhs, Vec2 rhs) { return rhs += lhs; }
Vec2 operator-(const float lhs, Vec2 rhs) { return rhs -= lhs; }
Vec2 operator*(const float lhs, Vec2 rhs) { return rhs *= lhs; }
Vec2 operator/(const float lhs, Vec2 rhs) { return rhs /= lhs; }
Vec2 operator+(Vec2 lhs, const Vec2 rhs) { return lhs += rhs; }
Vec2 operator-(Vec2 lhs, const Vec2 rhs) { return lhs -= rhs; }

bool operator==(const Vec2 lhs, const Vec2 rhs)
{
    return lhs.X == rhs.X && lhs.Y == rhs.Y;
}

bool operator!=(const Vec2 lhs, const Vec2 rhs)
{
    return !(lhs == rhs);
}