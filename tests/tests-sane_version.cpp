#include "catch.hpp"
#include "sane.h"
#include <climits>

TEST_CASE("sane_version Operators"){
    sane_version sv1(0,0,0);
    sane_version sv2(0,0,1);
    sane_version sv3(0,1,0);
    sane_version sv4(1,0,0);

    std::function testOperators = [&]() {
        SECTION("Equals Operator") {
            REQUIRE(sv1 == sv1);
            REQUIRE(sv2 == sv2);
            REQUIRE(sv3 == sv3);
            REQUIRE(sv4 == sv4);
            REQUIRE_FALSE(sv1 == sv2);
            REQUIRE_FALSE(sv1 == sv3);
            REQUIRE_FALSE(sv1 == sv4);
        }

        SECTION("Unequal Operator"){
            REQUIRE(sv1 != sv2);
            REQUIRE(sv1 != sv3);
            REQUIRE(sv1 != sv4);
            REQUIRE(sv2 != sv1);
            REQUIRE(sv2 != sv3);
            REQUIRE(sv2 != sv4);
            REQUIRE(sv3 != sv1);
            REQUIRE(sv3 != sv2);
            REQUIRE(sv3 != sv4);
            REQUIRE(sv4 != sv1);
            REQUIRE(sv4 != sv2);
            REQUIRE(sv4 != sv3);
        }

        SECTION("Less Than Operator") {
            REQUIRE(sv1 < sv2);
            REQUIRE(sv1 < sv3);
            REQUIRE(sv1 < sv4);
            REQUIRE(sv2 < sv3);
            REQUIRE(sv2 < sv4);
            REQUIRE(sv3 < sv4);
        }

        SECTION("Greater Than Operator") {
            REQUIRE(sv2 > sv1);
            REQUIRE(sv3 > sv1);
            REQUIRE(sv4 > sv1);
            REQUIRE(sv3 > sv2);
            REQUIRE(sv4 > sv2);
            REQUIRE(sv4 > sv3);
        }

        SECTION("Less Than or Equals Operator") {
            REQUIRE(sv1 <= sv1);
            REQUIRE(sv2 <= sv2);
            REQUIRE(sv3 <= sv3);
            REQUIRE(sv4 <= sv4);
            REQUIRE(sv1 <= sv2);
            REQUIRE(sv1 <= sv3);
            REQUIRE(sv1 <= sv4);
            REQUIRE(sv2 <= sv3);
            REQUIRE(sv2 <= sv4);
            REQUIRE(sv3 <= sv4);
        }

        SECTION("Greater Than or Equals Operator") {
            REQUIRE(sv1 >= sv1);
            REQUIRE(sv2 >= sv2);
            REQUIRE(sv3 >= sv3);
            REQUIRE(sv4 >= sv4);
            REQUIRE(sv2 >= sv1);
            REQUIRE(sv3 >= sv1);
            REQUIRE(sv4 >= sv1);
            REQUIRE(sv3 >= sv2);
            REQUIRE(sv4 >= sv2);
            REQUIRE(sv4 >= sv3);
        }
    };

        SECTION("trivial"){
            testOperators();
        }

        SECTION("general"){
            sv1 = sane_version(0,0,0);
            sv2 = sane_version(13,0,0);
            sv3 = sane_version(13,42,0);
            sv4 = sane_version(13,42,69);
            testOperators();
        }

        SECTION("boundaries"){
            sv1 = sane_version(UCHAR_MAX-1,0,0);
            sv2 = sane_version(UCHAR_MAX,0,0);
            sv3 = sane_version(UCHAR_MAX,UCHAR_MAX,USHRT_MAX - 1);
            sv4 = sane_version(UCHAR_MAX,UCHAR_MAX,USHRT_MAX);
            testOperators();
        }
}