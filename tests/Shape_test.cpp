#include <gtest/gtest.h>

#include <vector>
#include <tuple>

#include "Shape_test.h"

#include "Common.h"
#include "Ray.h"
#include "Tuple.h"
#include "Intersection.h"
#include "Transformation.h"
#include "Group.h"

using namespace std;

namespace rt {
    std::unique_ptr<TestShape> test_shape() {
        return make_unique<TestShape>();
    }

    TEST(ShapeTest, DefaultTransformation) {
        const auto s = test_shape();
        EXPECT_EQ(s->transform(), Transformation::identity());
        EXPECT_EQ(s->inversed_transform(), Transformation::identity());
    }

    TEST(ShapeTest, ChangeTransformation) {
        const auto s = test_shape();
        constexpr auto t = translation(2, 3, 4);
        constexpr auto t_inv = translation(-2, -3, -4);

        s->set_transform(t);

        EXPECT_EQ(s->transform(), t);
        EXPECT_EQ(s->inversed_transform(), t_inv);
    }

    TEST(ShapeTest, HasMaterial) {
        const auto s = test_shape();

        const auto m = s->material();

        EXPECT_EQ(m, Material{});
    }

    TEST(ShapeTest, SetMaterial) {
        const auto s = test_shape();
        Material m{};
        m.ambient = 1;

        s->set_material(m);

        EXPECT_EQ(s->material(), m);
    }

    class RayTransformedShapeIntersectionTest
            : public testing::TestWithParam<std::tuple<Transformation, Ray> > {
    };

    TEST_P(RayTransformedShapeIntersectionTest, RayTransformedShapeIntersection) {
        constexpr auto r = Ray{point(0, 0, -5), vector(0, 0, 1)};
        auto [m, expected] = GetParam();
        const auto s = test_shape();

        s->set_transform(m);
        const auto xs = s->intersect(r);

        EXPECT_EQ(s->saved_ray.origin(), expected.origin());
        EXPECT_EQ(s->saved_ray.direction(), expected.direction());
    }

    INSTANTIATE_TEST_SUITE_P(
        RayTransformedShapeIntersectionSuite,
        RayTransformedShapeIntersectionTest,
        ::testing::Values(
            make_tuple(scaling(2, 2, 2), Ray(point(0, 0, -2.5), vector(0, 0, 0.5))),
            make_tuple(translation(5, 0, 0), Ray(point(-5, 0, -5), vector(0, 0, 1)))
        ));

    class ShapeTransformedNormalTest
            : public testing::TestWithParam<std::tuple<Transformation, Point, Vector> > {
    };

    TEST_P(ShapeTransformedNormalTest, ShapeTransformedNormal) {
        auto [transform, point, expected_normal] = GetParam();

        const auto s = test_shape();
        s->set_transform(transform);

        const auto n = s->normal_at(point);

        EXPECT_TRUE(approx_equals(n, expected_normal, 1e-5));
    }

    INSTANTIATE_TEST_SUITE_P(
        ShapeTransformedNormalSuite,
        ShapeTransformedNormalTest,
        ::testing::Values(
            make_tuple(
                translation(0, 1, 0),
                point(0, 1.70711, -0.70711),
                vector(0, 0.70711, -0.70711)),
            make_tuple(
                scaling(1, 0.5, 1) * rotation_z(numbers::pi / 5),
                point(0, numbers::sqrt2 / 2, - numbers::sqrt2 / 2),
                vector(0, 0.97014, -0.24254))
        ));

    TEST(ShapeTest, A_shape_has_a_parent_attribute) {
        const auto s = test_shape();

        EXPECT_FALSE(s->parent());
    }

    TEST(ShapeTest, Converting_a_point_from_world_to_object_space) {
        const auto g1 = group();
        g1->set_transform(rotation_y(numbers::pi / 2));
        auto g2 = group();
        g2->set_transform(scaling(2, 2, 2));
        auto s = sphere();
        const auto s_ptr = s.get();
        s->set_transform(translation(5, 0, 0));
        g2->add_child(std::move(s));
        g1->add_child(std::move(g2));

        const auto p = s_ptr->world_to_object(point(-2, 0, -10));

        EXPECT_TRUE(approx_equals(p, point(0, 0, -1), 1e-15));
    }

    TEST(ShapeTest, Converting_a_normal_from_object_to_world_space) {
        const auto g1 = group();
        g1->set_transform(rotation_y(numbers::pi / 2));
        auto g2 = group();
        g2->set_transform(scaling(1, 2, 3));
        auto s = sphere();
        const auto s_ptr = s.get();
        s->set_transform(translation(5, 0, 0));
        g2->add_child(std::move(s));
        g1->add_child(std::move(g2));

        const auto n = s_ptr->normal_to_world(vector(numbers::sqrt3 / 3, numbers::sqrt3 / 3, numbers::sqrt3 / 3));

        EXPECT_TRUE(approx_equals(n, vector(0.2857, 0.4286, -0.8571), 1e-4));
    }

    TEST(ShapeTest, Finding_the_normal_on_a_child_object) {
        const auto g1 = group();
        g1->set_transform(rotation_y(numbers::pi / 2));
        auto g2 = group();
        g2->set_transform(scaling(1, 2, 3));
        auto s = sphere();
        const auto s_ptr = s.get();
        s->set_transform(translation(5, 0, 0));
        g2->add_child(std::move(s));
        g1->add_child(std::move(g2));

        const auto n = s_ptr->normal_at(point(1.7321, 1.1547, -5.5774));

        EXPECT_TRUE(approx_equals(n, vector(0.2857, 0.4286, -0.8571), 1e-4));
    }

    class RaySphereIntersectionTest : public testing::TestWithParam<std::tuple<Ray, std::vector<double> > > {
    };

    TEST_P(RaySphereIntersectionTest, RaySphereIntersection) {
        auto [ray, expected] = GetParam();
        const auto s = sphere();

        const auto xs = s->local_intersect(ray);

        ASSERT_EQ(xs.size(), expected.size());
        for (auto i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(xs[i].t(), expected[i]);
            EXPECT_EQ(xs[i].object(), s.get());
        }
    }

    INSTANTIATE_TEST_SUITE_P(
        RaySphereIntersectionSuite,
        RaySphereIntersectionTest,
        ::testing::Values(
            make_tuple(Ray(point(0, 0, -5), vector(0, 0, 1)), std::vector{4.0, 6.0}),
            make_tuple(Ray(point(0, 1, -5), vector(0, 0, 1)), std::vector{5.0, 5.0}),
            make_tuple(Ray(point(0, 2, -5), vector(0, 0, 1)), std::vector<double>{}),
            make_tuple(Ray(point(0, 0, 0), vector(0, 0, 1)), std::vector{-1.0, 1.0}),
            make_tuple(Ray(point(0, 0, 5), vector(0, 0, 1)), std::vector{-6.0, -4.0})
        ));

    TEST(SphereTest, SetObjectInIntersection) {
        constexpr Ray ray{point(0, 0, -5), vector(0, 0, 1)};
        const auto s = sphere();

        const auto xs = s->local_intersect(ray);

        EXPECT_EQ(xs.size(), 2);
        EXPECT_EQ(xs[0].object(), s.get());
        EXPECT_EQ(xs[1].object(), s.get());
    }

    class SphereNormalTest : public testing::TestWithParam<std::tuple<Point, Vector> > {
    };

    TEST_P(SphereNormalTest, SphereNormal) {
        auto [input_point, expected_normal] = GetParam();

        const auto s = sphere();

        const auto n = s->local_normal_at(input_point);

        EXPECT_EQ(n, expected_normal);
    }

    INSTANTIATE_TEST_SUITE_P(
        SphereNormalSuite,
        SphereNormalTest,
        ::testing::Values(
            make_tuple(point(1, 0, 0), vector(1, 0, 0)),
            make_tuple(point(0, 1, 0), vector(0, 1, 0)),
            make_tuple(point(0, 0, 1), vector(0, 0, 1)),
            make_tuple(
                point(numbers::sqrt3 / 3, numbers::sqrt3 / 3, numbers::sqrt3 / 3),
                vector(numbers::sqrt3 / 3, numbers::sqrt3 / 3, numbers::sqrt3 / 3)
            )
        ));

    TEST(SphereTest, NormalizedNormal) {
        const auto s = sphere();

        const auto n = s->local_normal_at(
            point(numbers::sqrt3 / 3, numbers::sqrt3 / 3, numbers::sqrt3 / 3));

        EXPECT_EQ(n, n.normalize());
    }

    TEST(PlaneTest, PlaneNormalIsConstant) {
        const auto p = plane();

        const auto n1 = p->local_normal_at(point(0, 0, 0));
        const auto n2 = p->local_normal_at(point(10, 0, -10));
        const auto n3 = p->local_normal_at(point(-5, 0, 150));

        EXPECT_EQ(n1, vector(0, 1, 0));
        EXPECT_EQ(n2, vector(0, 1, 0));
        EXPECT_EQ(n3, vector(0, 1, 0));
    }

    class RayPlaneIntersectionTest : public testing::TestWithParam<std::tuple<Ray, std::vector<double> > > {
    };

    TEST_P(RayPlaneIntersectionTest, RaySphereIntersection) {
        auto [ray, expected] = GetParam();
        const auto p = plane();

        const auto xs = p->local_intersect(ray);

        ASSERT_EQ(xs.size(), expected.size());
        for (auto i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(xs[i].t(), expected[i]);
            EXPECT_EQ(xs[i].object(), p.get());
        }
    }

    INSTANTIATE_TEST_SUITE_P(
        RayPlaneIntersectionSuite,
        RayPlaneIntersectionTest,
        ::testing::Values(
            make_tuple(Ray(point(0, 10, 0), vector(0, 0, 1)), std::vector<double>{}),
            make_tuple(Ray(point(0, 0, 0), vector(0, 0, 1)), std::vector<double>{}),
            make_tuple(Ray(point(0, 1, 0), vector(0, -1, 0)), std::vector{1.0}),
            make_tuple(Ray(point(0, -1, 0), vector(0, 1, 0)), std::vector{1.0})
        ));

    std::unique_ptr<Shape> glass_sphere() {
        auto s = sphere();
        s->material().transparency = 1;
        s->material().refractive_index = 1.5;
        return s;
    }

    class RayCubeIntersectionTest : public testing::TestWithParam<std::tuple<Point, Vector, double, double> > {
    };

    TEST_P(RayCubeIntersectionTest, RayCubeIntersection) {
        auto [origin, direction, t1, t2] = GetParam();

        const auto c = cube();
        const Ray r{origin, direction};

        const auto xs = c->local_intersect(r);

        ASSERT_EQ(xs.size(), 2);
        EXPECT_EQ(xs[0].t(), t1);
        EXPECT_EQ(xs[1].t(), t2);
    }

    INSTANTIATE_TEST_SUITE_P(
        RayCubeIntersectionTestSuite,
        RayCubeIntersectionTest,
        ::testing::Values(
            make_tuple(point(5, 0.5, 0), vector(-1, 0, 0), 4, 6),
            make_tuple(point(-5, 0.5, 0), vector(1, 0, 0), 4, 6),
            make_tuple(point(0.5, 5, 0), vector(0, -1, 0), 4, 6),
            make_tuple(point(0.5, -5, 0), vector(0, 1, 0), 4, 6),
            make_tuple(point(0.5, 0, 5), vector(0, 0, -1), 4, 6),
            make_tuple(point(0.5, 0, -5), vector(0, 0, 1), 4, 6),
            make_tuple(point(0, 0.5, 0), vector(0, 0, 1), -1, 1)
        ));

    class RayMissesCubeTest : public testing::TestWithParam<std::tuple<Point, Vector> > {
    };

    TEST_P(RayMissesCubeTest, RayMissesCube) {
        auto [origin, direction] = GetParam();

        const auto c = cube();
        const Ray r{origin, direction};

        const auto xs = c->local_intersect(r);

        ASSERT_EQ(xs.size(), 0);
    }

    INSTANTIATE_TEST_SUITE_P(
        RayMissesCubeTestSuite,
        RayMissesCubeTest,
        ::testing::Values(
            make_tuple(point(-2, 0, 0), vector(0.2673, 0.5345, 0.8018)),
            make_tuple(point(0, -2, 0), vector(0.8018, 0.2673, 0.5345)),
            make_tuple(point(0, 0, -2), vector(0.5345, 0.8018, 0.2673)),
            make_tuple(point(2, 0, 2), vector(0, 0, -1)),
            make_tuple(point(0, 2, 2), vector(0, -1, 0)),
            make_tuple(point(2, 2, 0), vector(-1, 0, 0))
        ));

    class CubeNormalTest : public testing::TestWithParam<std::tuple<Point, Vector> > {
    };

    TEST_P(CubeNormalTest, CubeNormal) {
        auto [point, normal] = GetParam();

        const auto c = cube();

        const auto actual_normal = c->local_normal_at(point);

        ASSERT_EQ(actual_normal, normal);
    }

    INSTANTIATE_TEST_SUITE_P(
        CubeNormalTestSuite,
        CubeNormalTest,
        ::testing::Values(
            make_tuple(point(1, 0.5, -0.8), vector(1, 0, 0)),
            make_tuple(point(-1, -0.2, 0.9), vector(-1, 0, 0)),
            make_tuple(point(-0.4, 1, -0.1), vector(0, 1, 0)),
            make_tuple(point(0.3, -1, -0.7), vector(0, -1, 0)),
            make_tuple(point(-0.6, 0.3, 1), vector(0, 0, 1)),
            make_tuple(point(0.4, 0.4, -1), vector(0, 0, -1)),
            make_tuple(point(1, 1, 1), vector(1, 0, 0)),
            make_tuple(point(-1, -1, -1), vector(-1, 0, 0))
        ));

    class RayMissesCylinderTest : public testing::TestWithParam<std::tuple<Point, Vector> > {
    };

    TEST_P(RayMissesCylinderTest, Ray_misses_cylinder) {
        auto [origin, direction] = GetParam();

        const auto cyl = cylinder();
        direction = direction.normalize();
        const Ray r{origin, direction};

        const auto xs = cyl->local_intersect(r);

        EXPECT_EQ(xs.size(), 0);
    }

    INSTANTIATE_TEST_SUITE_P(
        RayMissesCylinderTestSuite,
        RayMissesCylinderTest,
        ::testing::Values(
            make_tuple(point(1, 0, 0), vector(0, 1, 0)),
            make_tuple(point(0, 0, 0), vector(0, 1, 0)),
            make_tuple(point(0, 0, -5), vector(1, 1, 1))
        ));

    class RayStrikesCylinderTest : public testing::TestWithParam<std::tuple<Point, Vector, double, double> > {
    };

    TEST_P(RayStrikesCylinderTest, A_ray_strikes_a_cylinder) {
        auto [origin, direction, t0, t1] = GetParam();

        const auto cyl = cylinder();
        direction = direction.normalize();
        const Ray r{origin, direction};

        const auto xs = cyl->local_intersect(r);

        ASSERT_EQ(xs.size(), 2);
        EXPECT_TRUE(approx_equals(xs[0].t(), t0, 1e-5));
        EXPECT_TRUE(approx_equals(xs[1].t(), t1, 1e-5));
    }

    INSTANTIATE_TEST_SUITE_P(
        RayStrikesCylinderTestSuite,
        RayStrikesCylinderTest,
        ::testing::Values(
            make_tuple(point(1, 0, -5), vector(0, 0, 1), 5, 5),
            make_tuple(point(0, 0, -5), vector(0, 0, 1), 4, 6),
            make_tuple(point(0.5, 0, -5), vector(0.1, 1, 1), 6.80798, 7.08872)
        ));

    class NormalVectorOnCylinderTest : public testing::TestWithParam<std::tuple<Point, Vector> > {
    };

    TEST_P(NormalVectorOnCylinderTest, Normal_vector_on_a_cylinder) {
        auto [point, normal] = GetParam();

        const auto cyl = cylinder();

        const auto n = cyl->local_normal_at(point);

        EXPECT_EQ(n, normal);
    }

    INSTANTIATE_TEST_SUITE_P(
        NormalVectorOnCylinderTestSuite,
        NormalVectorOnCylinderTest,
        ::testing::Values(
            make_tuple(point(1, 0, 0), vector(1, 0, 0)),
            make_tuple(point(0, 5, -1), vector(0, 0, -1)),
            make_tuple(point(0, -2, 1), vector(0, 0, 1)),
            make_tuple(point(-1, 1, 0), vector(-1, 0, 0))
        ));

    TEST(CylinderTest, The_default_minimum_and_maximum_for_a_cylinder) {
        const auto cyl = cylinder();

        EXPECT_EQ(dynamic_cast<Cylinder*>(cyl.get())->minimum(), -numeric_limits<double>::infinity());
        EXPECT_EQ(dynamic_cast<Cylinder*>(cyl.get())->maximum(), numeric_limits<double>::infinity());
    }

    class TruncatedCylinderTest : public testing::TestWithParam<std::tuple<Point, Vector, int> > {
    };

    TEST_P(TruncatedCylinderTest, Intersecting_a_constrained_cylinder) {
        auto [point, direction, count] = GetParam();

        const auto cyl = cylinder(1, 2);
        direction = direction.normalize();
        const Ray r{point, direction};

        const auto xs = cyl->local_intersect(r);

        EXPECT_EQ(xs.size(), count);
    }

    INSTANTIATE_TEST_SUITE_P(
        TruncatedCylinderTestSuite,
        TruncatedCylinderTest,
        ::testing::Values(
            make_tuple(point(0, 1.5, 0), vector(0.1, 1, 0), 0),
            make_tuple(point(0, 3, -5), vector(0.0, 0, 1), 0),
            make_tuple(point(0, 0, -5), vector(0, 0, 1), 0),
            make_tuple(point(0, 2, -5), vector(0, 0, 1), 0),
            make_tuple(point(0, 1, -5), vector(0, 0, 1), 0),
            make_tuple(point(0, 1.5, -2), vector(0, 0, 1), 2)
        ));

    TEST(CylinderTest, The_default_closed_value_for_a_cylinder) {
        const auto cyl = cylinder();

        EXPECT_FALSE(dynamic_cast<Cylinder*>(cyl.get())->closed());
    }

    class ClosedCylinderTest : public testing::TestWithParam<std::tuple<Point, Vector, int> > {
    };

    TEST_P(ClosedCylinderTest, Intersecting_the_caps_of_a_closed_cylinder) {
        auto [point, direction, count] = GetParam();

        const auto cyl = cylinder(1, 2, true);
        direction = direction.normalize();
        const Ray r{point, direction};

        const auto xs = cyl->local_intersect(r);

        EXPECT_EQ(xs.size(), count);
    }

    INSTANTIATE_TEST_SUITE_P(
        ClosedCylinderTestSuite,
        ClosedCylinderTest,
        ::testing::Values(
            make_tuple(point(0, 3, 0), vector(0, -1, 0), 2),
            make_tuple(point(0, 3, -2), vector(0, -1, 2), 2),
            make_tuple(point(0, 4, -2), vector(0, -1, 1), 2),
            make_tuple(point(0, 0, -2), vector(0, 1, 2), 2),
            make_tuple(point(0, -1, -2), vector(0, 1, 1), 2)
        ));

    class ClosedCylinderNormalTest : public testing::TestWithParam<std::tuple<Point, Vector> > {
    };

    TEST_P(ClosedCylinderNormalTest, The_normal_vector_on_a_cylinder_end_caps) {
        auto [point, normal] = GetParam();

        const auto cyl = cylinder(1, 2, true);

        const auto n = cyl->local_normal_at(point);

        EXPECT_EQ(n, normal);
    }

    INSTANTIATE_TEST_SUITE_P(
        ClosedCylinderNormalTestSuite,
        ClosedCylinderNormalTest,
        ::testing::Values(
            make_tuple(point(0, 1, 0), vector(0, -1, 0)),
            make_tuple(point(0.5, 1, 0), vector(0, -1, 0)),
            make_tuple(point(0, 1, 0.5), vector(0, -1, 0)),
            make_tuple(point(0, 2, 0), vector(0, 1, 0)),
            make_tuple(point(0.5, 2, 0), vector(0, 1, 0)),
            make_tuple(point(0, 2, 0.5), vector(0, 1, 0))
        ));

    class ConeIntersectionTest : public testing::TestWithParam<std::tuple<Point, Vector, double, double> > {
    };

    TEST_P(ConeIntersectionTest, Intersecting_cone_with_a_ray) {
        auto [origin, direction, t0, t1] = GetParam();

        const auto shape = cone();
        direction = direction.normalize();
        const Ray r{origin, direction};

        const auto xs = shape->local_intersect(r);

        ASSERT_EQ(xs.size(), 2);
        EXPECT_TRUE(approx_equals(xs[0].t(), t0, 1e-5));
        EXPECT_TRUE(approx_equals(xs[1].t(), t1, 1e-5));
    }

    INSTANTIATE_TEST_SUITE_P(
        ConeIntersectionTestSuite,
        ConeIntersectionTest,
        ::testing::Values(
            make_tuple(point(0, 0, -5), vector(0, 0, 1), 5, 5),
            make_tuple(point(0, 0, -5), vector(1, 1, 1), 8.66025, 8.66025),
            make_tuple(point(1, 1, -5), vector(-0.5, -1, 1), 4.55006, 49.44994)
        ));

    TEST(ConeTest, Intersecting_a_cone_with_a_ray_parallel_to_one_of_its_halves) {
        const auto shape = cone();
        const auto direction = vector(0, 1, 1).normalize();
        const Ray r{point(0, 0, -1), direction};

        const auto xs = shape->local_intersect(r);

        ASSERT_EQ(xs.size(), 1);
        EXPECT_TRUE(approx_equals(xs[0].t(), 0.35355, 1e-5));
    }

    class ConeCapIntersectionTest : public testing::TestWithParam<std::tuple<Point, Vector, int> > {
    };

    TEST_P(ConeCapIntersectionTest, Intersecting_a_cone_end_caps) {
        auto [origin, direction, count] = GetParam();

        const auto shape = cone(-0.5, 0.5, true);
        direction = direction.normalize();
        const Ray r{origin, direction};

        const auto xs = shape->local_intersect(r);

        ASSERT_EQ(xs.size(), count);
    }

    INSTANTIATE_TEST_SUITE_P(
        ConeCapIntersectionTestSuite,
        ConeCapIntersectionTest,
        ::testing::Values(
            make_tuple(point(0, 0, -5), vector(0, 1, 0), 0),
            make_tuple(point(0, 0, -0.25), vector(0, 1, 1), 2),
            make_tuple(point(0, 0, -0.25), vector(0, 1, 0), 4)
        ));

    class ConeBoundsTest : public testing::TestWithParam<tuple<double, double, Bounds> > {
    };

    TEST_P(ConeBoundsTest, ConeBounds) {
        auto [minimum, maximum, bounds] = GetParam();
        const auto shape = cone(minimum, maximum);

        const auto result = shape->bounds();

        EXPECT_EQ(result, bounds);
    }

    INSTANTIATE_TEST_SUITE_P(
        ConeBoundsTestSuite,
        ConeBoundsTest,
        ::testing::Values(
            make_tuple(-numeric_limits<double>::infinity(), numeric_limits<double>::infinity(),
                Bounds{
                point(-numeric_limits<double>::infinity(), -numeric_limits<double>::infinity(),
                    -numeric_limits<double>::infinity()),
                point(numeric_limits<double>::infinity(), numeric_limits<double>::infinity(),
                    numeric_limits<double>::infinity())
                }),
            make_tuple(2, 3,
                Bounds{
                point(-3, 2, -3),
                point(3, 3, 3)
                }),
            make_tuple(-3, -2,
                Bounds{
                point(-3, -3, -3),
                point(3, -2, 3)
                })
        ));
}
