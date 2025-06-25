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
#include "test_definitions.hpp"

TEST_CASE("Empty tree")
{
	classtree::ctree<data_eq, meta_incr> kd;

	CHECK_EQ(kd.size(), 0);

	SUBCASE("Print entire tree")
	{
		static constexpr std::string_view kd_str = "^ size: 0 0\n";
		const std::string print_str_const = print_string(kd);
		CHECK_EQ(print_str_const, kd_str);
	}

	SUBCASE("Iterate entire tree forward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_iterator();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_iterator();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate entire tree backward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n";

		const std::string iter_str_fb_const = [&]()
		{
			auto it = kd.get_const_iterator();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_fb_const);

		const std::string iter_str_fb = [&]()
		{
			auto it = kd.get_iterator();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_fb);
	}

	SUBCASE("Check iterator bounds")
	{
		auto it = kd.get_const_iterator();
		CHECK_EQ(it.end(), true);
		CHECK_EQ(it.past_begin(), true);
	}

	SUBCASE("Check range iterator bounds")
	{
		auto it = kd.get_const_range_iterator();
		CHECK_EQ(it.end(), true);
		CHECK_EQ(it.past_begin(), true);
	}
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
