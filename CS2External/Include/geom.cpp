#pragma once
#include "geom.h"

extern int width = 1920;
extern int height = 1080;

bool WorldToScreen(vec3 pos, vec3& screen, float matrix[16], int windowWidth, int windowHeight)
{
    Vec4 clipCoords;
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
    if (clipCoords.w < 0.1f)
        return false;

    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];

    screen.x = (windowWidth / 2.0f) * (clipCoords.x / clipCoords.w + 1.0f);
    screen.y = (windowHeight / 2.0f) * (1.0f - clipCoords.y / clipCoords.w);

    return true;
}

bool T_WorldToScreen(vec3 pos, vec3& screen, float matrix[16], int windowWidth, int windowHeight)
{
	Vec4 clipCoords;
	clipCoords.w = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	if (clipCoords.w < 0.1f)
		return false;

	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clipCoords.z = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	screen.x = (windowWidth / 2.0f) * (clipCoords.x / clipCoords.w + 1.0f);
	screen.y = (windowHeight / 2.0f) * (1.0f - clipCoords.y / clipCoords.w);

    if (screen.x < 0 || screen.x > windowWidth || screen.y < 0 || screen.y > windowHeight)
        return false;

	return true;
}   

Vec3 WorldToScreen(Vec3& pos, float matrix[16], int windowWidth, int windowHeight)
{
    Vec4 clipCoords;

    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
    if (clipCoords.w < 0.1f)
        return Vec3{ 0.0f, 0.0f, 0.0f };

    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];

    float camX = windowWidth / 2.0;
    float camY = windowHeight / 2.0;

     vec3{ clipCoords.x / clipCoords.w, clipCoords.y / clipCoords.w, clipCoords.z / clipCoords.w };
}

Vec3 CalcAngle(Vec3& origin, Vec3& target)
{
    Vec3 results{ 0.0f, 0.0f, 0.0f };
    results.y = RadiansToDegrees(-atan2f(target.x - origin.x, target.y - origin.y));
    if (results.y <= 90.0f)
    {
        results.y += 360.0f;
    }
	results.y -= 270.0f;

    results.x = -RadiansToDegrees(asinf((target.z - origin.z) / origin.Distance(target)));
    return results;
}

Vec4 CalcRect(Vec3& feet, Vec3& head)
{
    return Vec4{feet.x - (head.y - feet.y) / 4, feet.y, head.x + (head.y - feet.y) / 4, head.y}; 
}

Vec3 DegreesToRadians(Vec3& vec)
{
    return Vec3{ DegreesToRadians(vec.x), DegreesToRadians(vec.y), DegreesToRadians(vec.z) };
}

float DegreesToRadians(float num)
{
    return num / 180.0f * PI;
}

Vec3 RadiansToDegrees(const Vec3& vec)
{
    return Vec3{ vec.x / PI * 180.0f, vec.y / PI * 180.0f, vec.z / PI * 180.0f };
}

float RadiansToDegrees(float num)
{
    return num / PI * 180.0f;
}

Vector3 Vector3::NormalizeAngle()
{
    if (x > 360)
        x -= 360;
    if (x < 0)
        x += 360;
    if (y > 90)
        y -= 90;
    if (y < -90)
        y += 90;
    return Vector3{ x,y,z };
}

std::string Vector3::ToString()
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

void getGameRect(HWND hwnd, RECT& rect)
{
    RECT windowRect, clientRect;

    GetWindowRect(hwnd, &windowRect);
    GetClientRect(hwnd, &clientRect);

    int borderWidth = (windowRect.right - windowRect.left - (clientRect.right - clientRect.left)) / 2;
    int titleBarHeight = (windowRect.bottom - windowRect.top - (clientRect.bottom - clientRect.top)) - borderWidth;

    width = clientRect.right - clientRect.left;
    height = clientRect.bottom - clientRect.top;

    rect.left = windowRect.left + borderWidth;
    rect.top = windowRect.top + titleBarHeight;
}