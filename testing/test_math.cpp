#include "pch.h"
#include "test.h"

#include "engine/system/math/vector2.h"
#include "engine/system/math/vector3.h"
#include "engine/system/math/random.h"

TEST(Vector2DTest, Magnitude) {
    raid::Vector2D vec(3.0, 4.0);
    ASSERT_DOUBLE_EQ(vec.Magnitude(), 5.0);
}

TEST(Vector2DTest, Normalize) {
    raid::Vector2D vec(3.0, 3.0);
    raid::Vector2D normalizedVec = vec.Normalize();
    ASSERT_DOUBLE_EQ(normalizedVec.Magnitude(), 1.0);
}

TEST(Vector2DTest, DotProduct) {
    raid::Vector2D vec1(1.0, 2.0);
    raid::Vector2D vec2(3.0, 4.0);
    ASSERT_DOUBLE_EQ(vec1.Dot(vec2), 11.0);
}

TEST(Vector3DTest, Magnitude) {
    raid::Vector3D vec(1.0, 2.0, 2.0);
    ASSERT_DOUBLE_EQ(vec.Magnitude(), 3.0);
}

TEST(Vector3DTest, Normalize) {
    raid::Vector3D vec(1.0, 2.0, 2.0);
    raid::Vector3D normalizedVec = vec.Normalize();
    ASSERT_DOUBLE_EQ(normalizedVec.Magnitude(), 1.0);
}

TEST(Vector3DTest, DotProduct) {
    raid::Vector3D vec1(1.0, 2.0, 3.0);
    raid::Vector3D vec2(4.0, 5.0, 6.0);
    ASSERT_DOUBLE_EQ(vec1.Dot(vec2), 32.0);
}

TEST(Vector3DTest, CrossProduct) {
    raid::Vector3D vec1(1.0, 0.0, 0.0);
    raid::Vector3D vec2(0.0, 1.0, 0.0);
    raid::Vector3D crossProduct = vec1.Cross(vec2);
    ASSERT_DOUBLE_EQ(crossProduct.GetX(), 0.0);
    ASSERT_DOUBLE_EQ(crossProduct.GetY(), 0.0);
    ASSERT_DOUBLE_EQ(crossProduct.GetZ(), 1.0);
}

TEST(RandomTest, Deterministic)
{
    raid::BasicRNG rng1, rng2;
    rng1.Reset(1337);
    rng2.Reset(1337);

    for (int i = 0; i < 100; i++)
    {
        EXPECT_EQ(rng1.GetInt(1, 100), rng2.GetInt(1, 100));
    }
}

TEST(RandomTest, TestRange)
{
    raid::BasicRNG rng1, rng2;
    rng1.Reset(time(nullptr));

    int count[10] = {0};
    for (int i = 0; i < 10000; i++)
    {
        int result = rng1.GetInt(0, 9);
        count[result] = count[result] + 1;
    }

    // for unit testing purposes, lets make sure its within 10% of average
    for (int i = 0; i < 10; i++)
    {
        EXPECT_TRUE(count[i] >= 900);
        EXPECT_TRUE(count[i] <= 1100);
    }
}