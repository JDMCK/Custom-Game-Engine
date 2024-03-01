#include "Vec2.h"

const Vec2 Vec2::UP(0, -1);
const Vec2 Vec2::DOWN(0, 1);
const Vec2 Vec2::LEFT(-1, 0);
const Vec2 Vec2::RIGHT(1, 0);
const Vec2 Vec2::ZERO(0, 0);

bool Vec2::operator == (const Vec2& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

bool Vec2::operator != (const Vec2& rhs) const
{
    return x != rhs.x || y != rhs.y;
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * (const float value) const
{
    return Vec2(x * value, y * value);
}

Vec2 Vec2::operator / (const float value) const
{
    return Vec2(x / value, y / value);
}

void Vec2::operator += (const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator *= (const float value)
{
    x *= value;
    y *= value;
}

void Vec2::operator /= (const float value)
{
    x /= value;
    y /= value;
}


float Vec2::dist(const Vec2& rhs) const
{
    return sqrt((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
}

float Vec2::dot(const Vec2& rhs) const
{
    return x * rhs.x + y * rhs.y;
}

float Vec2::length() const
{
    return sqrt(x * x + y * y);
}

float Vec2::rotation() const
{
    return std::atan2(y, x) * 57.29578f;
}

Vec2& Vec2::normalize()
{   
    float length = this->length();
    if (length != 0)
    {
        x /= length;
        y /= length;
    }
    return *this;
}

Vec2& Vec2::scale(const float value)
{
    x *= value;
    y *= value;
    return *this;
}

Vec2& Vec2::rotate(const float angle)
{
    float radians = angle / 57.29578f;
    float cos_angle = cosf(radians);
    float sin_angle = sinf(radians);

    float x2 = x * cos_angle - y * sin_angle;
    float y2 = x * sin_angle + y * cos_angle;
    x = x2;
    y = y2;
    return *this;
}

Vec2& Vec2::flipX()
{
    x *= -1;
    return *this;
}

Vec2& Vec2::flipY()
{
    y *= -1;
    return *this;
}

std::ostream& operator << (std::ostream& os, const Vec2& v)
{
    return os << "(" << v.x << ", " << v.y << ")";
}