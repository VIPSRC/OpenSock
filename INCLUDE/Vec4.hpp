#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#define SMALL_float 0.0000000001


/**
 * Attempt to include a header file if the file exists.
 * If the file does not exist, create a dummy data structure for that type.
 * If it cannot be determined if it exists, just attempt to include it.
 */
#ifdef __has_include
#   if __has_include("Vec3.hpp")
#       include "Vec3.hpp"
#   elif !defined(GMATH_Vec3)
#define GMATH_Vec3
        struct Vec3
        {
            union
            {
                struct
                {
                    float X;
                    float Y;
                    float Z;
                };
                float data[3];
            };

            inline Vec3() : X(0), Y(0), Z(0) {}
            inline Vec3(float data[]) : X(data[0]), Y(data[1]), Z(data[2])
                {}
            inline Vec3(float value) : X(value), Y(value), Z(value) {}
            inline Vec3(float x, float y) : X(x), Y(y), Z(0) {}
            inline Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}

            static inline Vec3 Cross(Vec3 lhs, Vec3 rhs)
            {
                float x = lhs.Y * rhs.Z - lhs.Z * rhs.Y;
                float y = lhs.Z * rhs.X - lhs.X * rhs.Z;
                float z = lhs.X * rhs.Y - lhs.Y * rhs.X;
                return Vec3(x, y, z);
            }

            static inline float Dot(Vec3 lhs, Vec3 rhs)
            {
                return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
            }

            static inline Vec3 Normalized(Vec3 v)
            {
                float mag = sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
                if (mag == 0)
                    return Vec3::Zero();
                return v / mag;
            }

            static inline Vec3 Orthogonal(Vec3 v)
            {
                return v.Z < v.X ?
                    Vec3(v.Y, -v.X, 0) : Vec3(0, -v.Z, v.Y);
            }

            static inline float SqrMagnitude(Vec3 v)
            {
                return v.X * v.X + v.Y * v.Y + v.Z * v.Z;
            }
        };


        inline Vec3 operator+(Vec3 lhs, const Vec3 rhs)
        {
            return Vec3(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
        }

        inline Vec3 operator*(Vec3 lhs, const float rhs)
        {
            return Vec3(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs);
        }
#   endif
#else
#   include "Vec3.hpp"
#endif


struct Vec4
{
    union
    {
        struct
        {
            float X;
            float Y;
            float Z;
            float W;
        };
        float data[4];
    };


    /**
     * Constructors.
     */
    inline Vec4();
    inline Vec4(float data[]);
    inline Vec4(Vec3 Vec, float scalar);
    inline Vec4(float x, float y, float z, float w);


    /**
     * Constants for common Vec4s.
     */
    static inline Vec4 Identity();


    /**
     * Returns the angle between two Vec4s.
     * The Vec4s must be normalized.
     * @param a: The first Vec4.
     * @param b: The second Vec4.
     * @return: A scalar value.
     */
    static inline float Angle(Vec4 a, Vec4 b);

    /**
     * Returns the conjugate of a Vec4.
     * @param rotation: The Vec4 in question.
     * @return: A new Vec4.
     */
    static inline Vec4 Conjugate(Vec4 rotation);

    /**
     * Returns the dot product of two Vec4s.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static inline float Dot(Vec4 lhs, Vec4 rhs);

    /**
     * Creates a new Vec4 from the angle-axis representation of
     * a rotation.
     * @param angle: The rotation angle in radians.
     * @param axis: The Vec about which the rotation occurs.
     * @return: A new Vec4.
     */
    static inline Vec4 FromAngleAxis(float angle, Vec3 axis);

    /**
     * Create a new Vec4 from the euler angle representation of
     * a rotation. The z, x and y values represent rotations about those
     * axis in that respective order.
     * @param rotation: The x, y and z rotations.
     * @return: A new Vec4.
     */
    static inline Vec4 FromEuler(Vec3 rotation);

    /**
     * Create a new Vec4 from the euler angle representation of
     * a rotation. The z, x and y values represent rotations about those
     * axis in that respective order.
     * @param x: The rotation about the x-axis in radians.
     * @param y: The rotation about the y-axis in radians.
     * @param z: The rotation about the z-axis in radians.
     * @return: A new Vec4.
     */
    static inline Vec4 FromEuler(float x, float y, float z);

    /**
     * Create a Vec4 rotation which rotates "fromVec" to "toVec".
     * @param fromVec: The Vec from which to start the rotation.
     * @param toVec: The Vec at which to end the rotation.
     * @return: A new Vec4.
     */
    static inline Vec4 FromToRotation(Vec3 fromVec,
                                            Vec3 toVec);

    /**
     * Returns the inverse of a rotation.
     * @param rotation: The Vec4 in question.
     * @return: A new Vec4.
     */
    static inline Vec4 Inverse(Vec4 rotation);

    /**
     * Interpolates between a and b by t, which is clamped to the range [0-1].
     * The result is normalized before being returned.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @return: A new Vec4.
     */
    static inline Vec4 Lerp(Vec4 a, Vec4 b, float t);

    /**
     * Interpolates between a and b by t. This normalizes the result when
     * complete.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new Vec4.
     */
    static inline Vec4 LerpUnclamped(Vec4 a, Vec4 b,
                                           float t);

    /**
     * Creates a rotation with the specified forward direction. This is the
     * same as calling LookRotation with (0, 1, 0) as the upwards Vec.
     * The output is undefined for parallel Vecs.
     * @param forward: The forward direction to look toward.
     * @return: A new Vec4.
     */
    static inline Vec4 LookRotation(Vec3 forward);

    /**
     * Creates a rotation with the specified forward and upwards directions.
     * The output is undefined for parallel Vecs.
     * @param forward: The forward direction to look toward.
     * @param upwards: The direction to treat as up.
     * @return: A new Vec4.
     */
    static inline Vec4 LookRotation(Vec3 forward, Vec3 upwards);

    /**
     * Returns the norm of a Vec4.
     * @param rotation: The Vec4 in question.
     * @return: A scalar value.
     */
    static inline float Norm(Vec4 rotation);

    /**
     * Returns a Vec4 with identical rotation and a norm of one.
     * @param rotation: The Vec4 in question.
     * @return: A new Vec4.
     */
    static inline Vec4 Normalized(Vec4 rotation);

    /**
     * Returns a new Vec4 created by rotating "from" towards "to" by
     * "maxRadiansDelta". This will not overshoot, and if a negative delta is
     * applied, it will rotate till completely opposite "to" and then stop.
     * @param from: The rotation at which to start.
     * @param to: The rotation at which to end.
     # @param maxRadiansDelta: The maximum number of radians to rotate.
     * @return: A new Vec4.
     */
    static inline Vec4 RotateTowards(Vec4 from, Vec4 to,
                                           float maxRadiansDelta);

    /**
     * Returns a new Vec4 interpolated between a and b, using spherical
     * linear interpolation. The variable t is clamped to the range [0-1]. The
     * resulting Vec4 will be normalized.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new Vec4.
     */
    static inline Vec4 Slerp(Vec4 a, Vec4 b, float t);

    /**
     * Returns a new Vec4 interpolated between a and b, using spherical
     * linear interpolation. The resulting Vec4 will be normalized.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new Vec4.
     */
    static inline Vec4 SlerpUnclamped(Vec4 a, Vec4 b,
                                            float t);

    /**
     * Outputs the angle axis representation of the provided Vec4.
     * @param rotation: The input Vec4.
     * @param angle: The output angle.
     * @param axis: The output axis.
     */
    static inline void ToAngleAxis(Vec4 rotation, float &angle,
                                   Vec3 &axis);

    /**
     * Returns the Euler angle representation of a rotation. The resulting
     * Vec contains the rotations about the z, x and y axis, in that order.
     * @param rotation: The Vec4 to convert.
     * @return: A new Vec.
     */
    static inline Vec3 ToEuler(Vec4 rotation);

    /**
     * Operator overloading.
     */
    inline struct Vec4& operator+=(const float rhs);
    inline struct Vec4& operator-=(const float rhs);
    inline struct Vec4& operator*=(const float rhs);
    inline struct Vec4& operator/=(const float rhs);
    inline struct Vec4& operator+=(const Vec4 rhs);
    inline struct Vec4& operator-=(const Vec4 rhs);
    inline struct Vec4& operator*=(const Vec4 rhs);
};

inline Vec4 operator-(Vec4 rhs);
inline Vec4 operator+(Vec4 lhs, const float rhs);
inline Vec4 operator-(Vec4 lhs, const float rhs);
inline Vec4 operator*(Vec4 lhs, const float rhs);
inline Vec4 operator/(Vec4 lhs, const float rhs);
inline Vec4 operator+(const float lhs, Vec4 rhs);
inline Vec4 operator-(const float lhs, Vec4 rhs);
inline Vec4 operator*(const float lhs, Vec4 rhs);
inline Vec4 operator/(const float lhs, Vec4 rhs);
inline Vec4 operator+(Vec4 lhs, const Vec4 rhs);
inline Vec4 operator-(Vec4 lhs, const Vec4 rhs);
inline Vec4 operator*(Vec4 lhs, const Vec4 rhs);
inline Vec3 operator*(Vec4 lhs, const Vec3 rhs);
inline bool operator==(const Vec4 lhs, const Vec4 rhs);
inline bool operator!=(const Vec4 lhs, const Vec4 rhs);



/*******************************************************************************
 * Implementation
 */

Vec4::Vec4() : X(0), Y(0), Z(0), W(1) {}
Vec4::Vec4(float data[]) : X(data[0]), Y(data[1]), Z(data[2]),
                                       W(data[3]) {}
Vec4::Vec4(Vec3 Vec, float scalar) : X(Vec.X),
                                                       Y(Vec.Y), Z(Vec.Z), W(scalar) {}
Vec4::Vec4(float x, float y, float z, float w) : X(x), Y(y),
                                                             Z(z), W(w) {}


Vec4 Vec4::Identity() { return Vec4(0, 0, 0, 1); }


float Vec4::Angle(Vec4 a, Vec4 b)
{
    float dot = Dot(a, b);
    return acos(fmin(fabs(dot), 1)) * 2;
}

Vec4 Vec4::Conjugate(Vec4 rotation)
{
    return Vec4(-rotation.X, -rotation.Y, -rotation.Z, rotation.W);
}

float Vec4::Dot(Vec4 lhs, Vec4 rhs)
{
    return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z + lhs.W * rhs.W;
}

Vec4 Vec4::FromAngleAxis(float angle, Vec3 axis)
{
    Vec4 q;
    float m = sqrt(axis.X * axis.X + axis.Y * axis.Y + axis.Z * axis.Z);
    float s = sin(angle / 2) / m;
    q.X = axis.X * s;
    q.Y = axis.Y * s;
    q.Z = axis.Z * s;
    q.W = cos(angle / 2);
    return q;
}

Vec4 Vec4::FromEuler(Vec3 rotation)
{
    return FromEuler(rotation.X, rotation.Y, rotation.Z);
}

Vec4 Vec4::FromEuler(float x, float y, float z)
{
    float cx = cos(x * 0.5);
    float cy = cos(y * 0.5);
    float cz = cos(z * 0.5);
    float sx = sin(x * 0.5);
    float sy = sin(y * 0.5);
    float sz = sin(z * 0.5);
    Vec4 q;
    q.X = cx * sy * sz + cy * cz * sx;
    q.Y = cx * cz * sy - cy * sx * sz;
    q.Z = cx * cy * sz - cz * sx * sy;
    q.W = sx * sy * sz + cx * cy * cz;
    return q;
}

Vec4 Vec4::FromToRotation(Vec3 fromVec, Vec3 toVec)
{
    float dot = Vec3::Dot(fromVec, toVec);
    float k = sqrt(Vec3::SqrMagnitude(fromVec) *
                   Vec3::SqrMagnitude(toVec));
    if (fabs(dot / k + 1) < 0.00001)
    {
        Vec3 ortho = Vec3::Orthogonal(fromVec);
        return Vec4(Vec3::Normalized(ortho), 0);
    }
    Vec3 cross = Vec3::Cross(fromVec, toVec);
    return Normalized(Vec4(cross, dot + k));
}

Vec4 Vec4::Inverse(Vec4 rotation)
{
    float n = Norm(rotation);
    return Conjugate(rotation) / (n * n);
}

Vec4 Vec4::Lerp(Vec4 a, Vec4 b, float t)
{
    if (t < 0) return Normalized(a);
    else if (t > 1) return Normalized(b);
    return LerpUnclamped(a, b, t);
}

Vec4 Vec4::LerpUnclamped(Vec4 a, Vec4 b, float t)
{
    Vec4 Vec4;
    if (Dot(a, b) >= 0)
        Vec4 = a * (1 - t) + b * t;
    else
        Vec4 = a * (1 - t) - b * t;
    return Normalized(Vec4);
}

Vec4 Vec4::LookRotation(Vec3 forward)
{
    return LookRotation(forward, Vec3(0, 1, 0));
}

Vec4 Vec4::LookRotation(Vec3 forward, Vec3 upwards)
{
    // Normalize inputs
    forward = Vec3::Normalized(forward);
    upwards = Vec3::Normalized(upwards);
    // Don't allow zero Vecs
    if (Vec3::SqrMagnitude(forward) < SMALL_float || Vec3::SqrMagnitude(upwards) < SMALL_float)
        return Vec4::Identity();
    // Handle alignment with up direction
    if (1 - fabs(Vec3::Dot(forward, upwards)) < SMALL_float)
        return FromToRotation(Vec3::Forward(), forward);
    // Get orthogonal Vecs
    Vec3 right = Vec3::Normalized(Vec3::Cross(upwards, forward));
    upwards = Vec3::Cross(forward, right);
    // Calculate rotation
    Vec4 Vec4;
    float radicand = right.X + upwards.Y + forward.Z;
    if (radicand > 0)
    {
        Vec4.W = sqrt(1.0 + radicand) * 0.5;
        float recip = 1.0 / (4.0 * Vec4.W);
        Vec4.X = (upwards.Z - forward.Y) * recip;
        Vec4.Y = (forward.X - right.Z) * recip;
        Vec4.Z = (right.Y - upwards.X) * recip;
    }
    else if (right.X >= upwards.Y && right.X >= forward.Z)
    {
        Vec4.X = sqrt(1.0 + right.X - upwards.Y - forward.Z) * 0.5;
        float recip = 1.0 / (4.0 * Vec4.X);
        Vec4.W = (upwards.Z - forward.Y) * recip;
        Vec4.Z = (forward.X + right.Z) * recip;
        Vec4.Y = (right.Y + upwards.X) * recip;
    }
    else if (upwards.Y > forward.Z)
    {
        Vec4.Y = sqrt(1.0 - right.X + upwards.Y - forward.Z) * 0.5;
        float recip = 1.0 / (4.0 * Vec4.Y);
        Vec4.Z = (upwards.Z + forward.Y) * recip;
        Vec4.W = (forward.X - right.Z) * recip;
        Vec4.X = (right.Y + upwards.X) * recip;
    }
    else
    {
        Vec4.Z = sqrt(1.0 - right.X - upwards.Y + forward.Z) * 0.5;
        float recip = 1.0 / (4.0 * Vec4.Z);
        Vec4.Y = (upwards.Z + forward.Y) * recip;
        Vec4.X = (forward.X + right.Z) * recip;
        Vec4.W = (right.Y - upwards.X) * recip;
    }
    return Vec4;
}

float Vec4::Norm(Vec4 rotation)
{
    return sqrt(rotation.X * rotation.X +
                rotation.Y * rotation.Y +
                rotation.Z * rotation.Z +
                rotation.W * rotation.W);
}

Vec4 Vec4::Normalized(Vec4 rotation)
{
    return rotation / Norm(rotation);
}

Vec4 Vec4::RotateTowards(Vec4 from, Vec4 to,
                                     float maxRadiansDelta)
{
    float angle = Vec4::Angle(from, to);
    if (angle == 0)
        return to;
    maxRadiansDelta = fmax(maxRadiansDelta, angle - M_PI);
    float t = fmin(1, maxRadiansDelta / angle);
    return Vec4::SlerpUnclamped(from, to, t);
}

Vec4 Vec4::Slerp(Vec4 a, Vec4 b, float t)
{
    if (t < 0) return Normalized(a);
    else if (t > 1) return Normalized(b);
    return SlerpUnclamped(a, b, t);
}

Vec4 Vec4::SlerpUnclamped(Vec4 a, Vec4 b, float t)
{
    float n1;
    float n2;
    float n3 = Dot(a, b);
    bool flag = false;
    if (n3 < 0)
    {
        flag = true;
        n3 = -n3;
    }
    if (n3 > 0.999999)
    {
        n2 = 1 - t;
        n1 = flag ? -t : t;
    }
    else
    {
        float n4 = acos(n3);
        float n5 = 1 / sin(n4);
        n2 = sin((1 - t) * n4) * n5;
        n1 = flag ? -sin(t * n4) * n5 : sin(t * n4) * n5;
    }
    Vec4 Vec4;
    Vec4.X = (n2 * a.X) + (n1 * b.X);
    Vec4.Y = (n2 * a.Y) + (n1 * b.Y);
    Vec4.Z = (n2 * a.Z) + (n1 * b.Z);
    Vec4.W = (n2 * a.W) + (n1 * b.W);
    return Normalized(Vec4);
}

void Vec4::ToAngleAxis(Vec4 rotation, float &angle, Vec3 &axis)
{
    if (rotation.W > 1)
        rotation = Normalized(rotation);
    angle = 2 * acos(rotation.W);
    float s = sqrt(1 - rotation.W * rotation.W);
    if (s < 0.00001) {
        axis.X = 1;
        axis.Y = 0;
        axis.Z = 0;
    } else {
        axis.X = rotation.X / s;
        axis.Y = rotation.Y / s;
        axis.Z = rotation.Z / s;
    }
}

Vec3 Vec4::ToEuler(Vec4 rotation)
{
    float sqw = rotation.W * rotation.W;
    float sqx = rotation.X * rotation.X;
    float sqy = rotation.Y * rotation.Y;
    float sqz = rotation.Z * rotation.Z;
    // If normalized is one, otherwise is correction factor
    float unit = sqx + sqy + sqz + sqw;
    float test = rotation.X * rotation.W - rotation.Y * rotation.Z;
    Vec3 v;
    // Singularity at north pole
    if (test > 0.4995f * unit)
    {
        v.Y = 2 * atan2(rotation.Y, rotation.X);
        v.X = M_PI_2;
        v.Z = 0;
        return v;
    }
    // Singularity at south pole
    if (test < -0.4995f * unit)
    {
        v.Y = -2 * atan2(rotation.Y, rotation.X);
        v.X = -M_PI_2;
        v.Z = 0;
        return v;
    }
    // Yaw
    v.Y = atan2(2 * rotation.W * rotation.Y + 2 * rotation.Z * rotation.X,
                1 - 2 * (rotation.X * rotation.X + rotation.Y * rotation.Y));
    // Pitch
    v.X = asin(2 * (rotation.W * rotation.X - rotation.Y * rotation.Z));
    // Roll
    v.Z = atan2(2 * rotation.W * rotation.Z + 2 * rotation.X * rotation.Y,
                1 - 2 * (rotation.Z * rotation.Z + rotation.X * rotation.X));
    return v;
}

struct Vec4& Vec4::operator+=(const float rhs)
{
    X += rhs;
    Y += rhs;
    Z += rhs;
    W += rhs;
    return *this;
}

struct Vec4& Vec4::operator-=(const float rhs)
{
    X -= rhs;
    Y -= rhs;
    Z -= rhs;
    W -= rhs;
    return *this;
}

struct Vec4& Vec4::operator*=(const float rhs)
{
    X *= rhs;
    Y *= rhs;
    Z *= rhs;
    W *= rhs;
    return *this;
}

struct Vec4& Vec4::operator/=(const float rhs)
{
    X /= rhs;
    Y /= rhs;
    Z /= rhs;
    W /= rhs;
    return *this;
}

struct Vec4& Vec4::operator+=(const Vec4 rhs)
{
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    W += rhs.W;
    return *this;
}

struct Vec4& Vec4::operator-=(const Vec4 rhs)
{
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    W -= rhs.W;
    return *this;
}

struct Vec4& Vec4::operator*=(const Vec4 rhs)
{
    Vec4 q;
    q.W = W * rhs.W - X * rhs.X - Y * rhs.Y - Z * rhs.Z;
    q.X = X * rhs.W + W * rhs.X + Y * rhs.Z - Z * rhs.Y;
    q.Y = W * rhs.Y - X * rhs.Z + Y * rhs.W + Z * rhs.X;
    q.Z = W * rhs.Z + X * rhs.Y - Y * rhs.X + Z * rhs.W;
    *this = q;
    return *this;
}

Vec4 operator-(Vec4 rhs) { return rhs * -1; }
Vec4 operator+(Vec4 lhs, const float rhs) { return lhs += rhs; }
Vec4 operator-(Vec4 lhs, const float rhs) { return lhs -= rhs; }
Vec4 operator*(Vec4 lhs, const float rhs) { return lhs *= rhs; }
Vec4 operator/(Vec4 lhs, const float rhs) { return lhs /= rhs; }
Vec4 operator+(const float lhs, Vec4 rhs) { return rhs += lhs; }
Vec4 operator-(const float lhs, Vec4 rhs) { return rhs -= lhs; }
Vec4 operator*(const float lhs, Vec4 rhs) { return rhs *= lhs; }
Vec4 operator/(const float lhs, Vec4 rhs) { return rhs /= lhs; }
Vec4 operator+(Vec4 lhs, const Vec4 rhs)
{
    return lhs += rhs;
}
Vec4 operator-(Vec4 lhs, const Vec4 rhs)
{
    return lhs -= rhs;
}
Vec4 operator*(Vec4 lhs, const Vec4 rhs)
{
    return lhs *= rhs;
}

Vec3 operator*(Vec4 lhs, const Vec3 rhs)
{
    Vec3 u = Vec3(lhs.X, lhs.Y, lhs.Z);
    float s = lhs.W;
    return u * (Vec3::Dot(u, rhs) * 2)
           + rhs * (s * s - Vec3::Dot(u, u))
           + Vec3::Cross(u, rhs) * (2.0 * s);
}

bool operator==(const Vec4 lhs, const Vec4 rhs)
{
    return lhs.X == rhs.X &&
           lhs.Y == rhs.Y &&
           lhs.Z == rhs.Z &&
           lhs.W == rhs.W;
}

bool operator!=(const Vec4 lhs, const Vec4 rhs)
{
    return !(lhs == rhs);
}