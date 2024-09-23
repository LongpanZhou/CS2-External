#pragma once
#include <math.h>
#include <string>
#include <Windows.h>

extern int width;
extern int height;
extern RECT rect;

constexpr auto PI = 3.1415927f;
class Vector3
{
public:

    float x;
    float y;
    float z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
    Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
    Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
    Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
    Vector3& operator += (const float rhs) { return *this = *this + Vector3(rhs, rhs, rhs); }
    Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
    Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
    Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
    float Length() const { return sqrtf(x * x + y * y + z * z); } //AKA Magnitude
    Vector3 Normalize() const { return *this * (1 / Length()); } // 1/ Length() 
    float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
    Vector3& abs() { x = fabs(x); y = fabs(y); z = fabs(z); return *this; }
    Vector3 NormalizeAngle(); // Wraps around x from 0-360 and y from -90 to 90
    void NormalizeAngle(Vector3 &angle); // Wraps around x from 0-360 and y from -90 to 90

    std::string ToString();

};

struct Vector4
{
    float x;
    float y;
    float z;
    float w;
};

struct Vector2
{
	float x;
	float y;
};

using Vec3 = Vector3;
using vec3 = Vector3;
using Vec2 = Vector2;
using vec2 = Vector2;
using Vec4 = Vector4;
using vec4 = Vector4;

bool WorldToScreen(vec3 pos, vec3& screen, float matrix[16], int windowWidth, int windowHeight);
bool T_WorldToScreen(vec3 pos, vec3& screen, float matrix[16], int windowWidth, int windowHeight);
Vec3 WorldToScreen(Vec3& pos, float matrix[16], int windowWidth, int windowHeight);
Vec3 CalcAngle(Vec3& origin, Vec3& target);
Vec4 CalcRect(Vec3& feet, Vec3& head);

Vec3 DegreesToRadians(Vec3& vec);
float DegreesToRadians(float num);
Vec3 RadiansToDegrees(const Vec3& vec);
float RadiansToDegrees(float num);
void getGameRect(HWND hwnd, RECT& rect);