/**
 * Tests for the Classification Tree
 * Copyright (C) 2025  Lluís Alemany Puig
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 * 		Lluís Alemany Puig
 * 		https://github.com/lluisalemanypuig
 */

// C++ includes
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

// ctree includes
#include <ctree/ctree.hpp>

/* 00 */

namespace e00 {
struct object { };
struct metadata { };

static_assert(not classtree::EqualityComparable<object>);
static_assert(not classtree::LessthanComparable<object>);
} // namespace e00

TEST_CASE("On non-comparable objects. On non-mergeable metadata.")
{
	classtree::ctree<e00::object, e00::metadata, int, float> kd;
	constexpr bool first = requires { kd.add<false>({}, {}, 1, 3.2f); };
	CHECK(first);
	constexpr bool second = not requires { kd.add<true>({}, {}, 1, 3.2f); };
	CHECK(second);
}

/* 01 */

namespace e01 {
struct object { };
struct metadata {
	metadata& operator+= (const metadata&) noexcept
	{
		return *this;
	}
};
} // namespace e01

/* 02 */

TEST_CASE("On non-comparable objects. On mergeable metadata")
{
	classtree::ctree<e01::object, e01::metadata, int, float> kd;
	constexpr bool first = requires { kd.add<false>({}, {}, 1, 3.2f); };
	CHECK(first);
	constexpr bool second = not requires { kd.add<true>({}, {}, 1, 3.2f); };
	CHECK(second);
}

namespace e02_lt {
struct object {
	[[nodiscard]] inline bool operator< (const object&) const noexcept
	{
		return true;
	}
};
struct metadata { };
} // namespace e02_lt

TEST_CASE("On < comparable objects. On non-mergeable metadata")
{
	classtree::ctree<e02_lt::object, e02_lt::metadata, int, float> kd;
	constexpr bool first = requires { kd.add<false>({}, {}, 1, 3.2f); };
	CHECK(first);
	constexpr bool second = not requires { kd.add<true>({}, {}, 1, 3.2f); };
	CHECK(second);
}

namespace e02_eq {
struct object {
	[[nodiscard]] inline bool operator== (const object&) const noexcept
	{
		return true;
	}
};
struct metadata { };
} // namespace e02_eq

TEST_CASE("On == comparable objects. On non-mergeable metadata")
{
	classtree::ctree<e02_eq::object, e02_eq::metadata, int, float> kd;
	constexpr bool first = requires { kd.add<false>({}, {}, 1, 3.2f); };
	CHECK(first);
	constexpr bool second = not requires { kd.add<true>({}, {}, 1, 3.2f); };
	CHECK(second);
}

/* 03 */

namespace e03_lt {
struct object {
	[[nodiscard]] inline bool operator< (const object&) const noexcept
	{
		return true;
	}
};
struct metadata {
	metadata& operator+= (const metadata&) noexcept
	{
		return *this;
	}
};
} // namespace e03_lt

TEST_CASE("On == comparable objects. On mergeable metadata")
{
	classtree::ctree<e03_lt::object, e03_lt::metadata, int, float> kd;
	constexpr bool first = requires { kd.add<false>({}, {}, 1, 3.2f); };
	CHECK(first);
	const bool second = requires { kd.add<true>({}, {}, 1, 3.2f); };
	CHECK(second);
}

namespace e03_eq {
struct object {
	[[nodiscard]] inline bool operator== (const object&) const noexcept
	{
		return true;
	}
};
struct metadata {
	metadata& operator+= (const metadata&) noexcept
	{
		return *this;
	}
};
} // namespace e03_eq

TEST_CASE("On == comparable objects. On mergeable metadata")
{
	classtree::ctree<e03_eq::object, e03_eq::metadata, int, float> kd;
	constexpr bool first = requires { kd.add<false>({}, {}, 1, 3.2f); };
	CHECK(first);
	constexpr bool second = requires { kd.add<true>({}, {}, 1, 3.2f); };
	CHECK(second);
}

int main(int argc, char **argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	int res = context.run(); // run doctest

	// important - query flags (and --exit) rely on the user doing this
	if (context.shouldExit()) {
		// propagate the result of the tests
		return res;
	}
}
