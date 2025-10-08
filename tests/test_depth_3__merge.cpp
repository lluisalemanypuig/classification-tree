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
#include <ctree/iterator.hpp>
#include <ctree/range_iterator.hpp>

// custom includes
#include "definitions.hpp"
#include "functions.hpp"

TEST_CASE("Empty trees")
{
	classtree::ctree<data_eq, meta_incr, int, int, int> kd;

	{
		classtree::ctree<data_eq, meta_incr, int, int, int> kd2;
		const size_t r = kd.merge(std::move(kd2));
		CHECK_EQ(r, 0);
		CHECK_EQ(kd.size(), 0);
	}

	{
		CHECK_EQ(kd.size(), 0);
		classtree::ctree<data_eq, meta_incr, int, int, int> kd2;
		kd2.add({.i = 1, .j = 1, .k = 1, .z = 1}, {.num_occs = 1}, 1, 1, 1);
		kd2.add({.i = 1, .j = 1, .k = 1, .z = 2}, {.num_occs = 1}, 1, 1, 1);
		kd2.add({.i = 1, .j = 1, .k = 1, .z = 3}, {.num_occs = 1}, 1, 1, 1);
		kd2.add({.i = 1, .j = 2, .k = 1, .z = 1}, {.num_occs = 1}, 1, 2, 1);
		kd2.add({.i = 1, .j = 2, .k = 1, .z = 2}, {.num_occs = 1}, 1, 2, 1);
		kd2.add({.i = 1, .j = 2, .k = 1, .z = 3}, {.num_occs = 1}, 1, 2, 1);
		const size_t r = kd.merge(std::move(kd2));
		CHECK_EQ(r, 6);
		CHECK_EQ(kd.size(), 6);
	}
	{
		const std::string_view kd_str = "size: 6\n"
										"keys: 1\n"
										"└── 1\n"
										"    size: 6\n"
										"    keys: 2\n"
										"    ├── 1\n"
										"    │   size: 3\n"
										"    │   keys: 1\n"
										"    │   └── 1\n"
										"    │       ^ size: 3 3\n"
										"    │       ├── (1 1 1 1) {1}\n"
										"    │       ├── (1 1 1 2) {1}\n"
										"    │       └── (1 1 1 3) {1}\n"
										"    └── 2\n"
										"        size: 3\n"
										"        keys: 1\n"
										"        └── 1\n"
										"            ^ size: 3 3\n"
										"            ├── (1 2 1 1) {1}\n"
										"            ├── (1 2 1 2) {1}\n"
										"            └── (1 2 1 3) {1}\n";

		const std::string print_str_const = print_string(kd);
		CHECK_EQ(print_str_const, kd_str);
	}

	{
		CHECK_EQ(kd.size(), 6);
		classtree::ctree<data_eq, meta_incr, int, int, int> kd2;
		kd2.add({.i = 1, .j = 1, .k = 1, .z = 1}, {.num_occs = 1}, 1, 1, 1);
		kd2.add({.i = 1, .j = 1, .k = 1, .z = 2}, {.num_occs = 1}, 1, 1, 1);
		kd2.add({.i = 1, .j = 1, .k = 1, .z = 3}, {.num_occs = 1}, 1, 1, 1);
		kd2.add({.i = 1, .j = 2, .k = 1, .z = 1}, {.num_occs = 1}, 1, 2, 1);
		kd2.add({.i = 1, .j = 2, .k = 1, .z = 2}, {.num_occs = 1}, 1, 2, 1);
		kd2.add({.i = 1, .j = 2, .k = 1, .z = 3}, {.num_occs = 1}, 1, 2, 1);
		const size_t r = kd.merge(std::move(kd2));
		CHECK_EQ(r, 0);
		CHECK_EQ(kd.size(), 6);
	}
	{
		const std::string_view kd_str = "size: 6\n"
										"keys: 1\n"
										"└── 1\n"
										"    size: 6\n"
										"    keys: 2\n"
										"    ├── 1\n"
										"    │   size: 3\n"
										"    │   keys: 1\n"
										"    │   └── 1\n"
										"    │       ^ size: 3 3\n"
										"    │       ├── (1 1 1 1) {2}\n"
										"    │       ├── (1 1 1 2) {2}\n"
										"    │       └── (1 1 1 3) {2}\n"
										"    └── 2\n"
										"        size: 3\n"
										"        keys: 1\n"
										"        └── 1\n"
										"            ^ size: 3 3\n"
										"            ├── (1 2 1 1) {2}\n"
										"            ├── (1 2 1 2) {2}\n"
										"            └── (1 2 1 3) {2}\n";

		const std::string print_str_const = print_string(kd);
		CHECK_EQ(print_str_const, kd_str);
	}
}

int main(int argc, char **argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	return context.run(); // run doctest
}
