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

TEST_CASE("Empty tree")
{
	classtree::ctree<data_eq, meta_incr, int, int, int> kd;

	CHECK_EQ(kd.size(), 0);

	SUBCASE("Print entire tree")
	{
		static constexpr std::string_view kd_str = "size: 0\n"
												   "keys: 0\n";
		const std::string print_str_const = print_string(kd);
		CHECK_EQ(print_str_const, kd_str);
	}

	SUBCASE("Iterate entire tree forward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_iterator_begin();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_iterator_begin();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate entire tree backward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n";

		const std::string iter_str_fb_const = [&]()
		{
			auto it = kd.get_const_iterator_end();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_fb_const);

		const std::string iter_str_fb = [&]()
		{
			auto it = kd.get_iterator_end();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_fb);
	}

	SUBCASE("Iterate over a range forward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n";

		const auto f1 = [](const int v) -> bool
		{
			return v == 1;
		};
		const auto f2 = [](const int v) -> bool
		{
			return 1 <= v and v <= 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator_begin(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator_begin(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate over a range backward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n";

		const auto f1 = [](const int v) -> bool
		{
			return v == 1;
		};
		const auto f2 = [](const int v) -> bool
		{
			return 1 <= v and v <= 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator_end(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator_end(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Count elements")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 1;
		};
		const auto f2 = [](const int v) -> bool
		{
			return 1 <= v and v <= 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		auto it = kd.get_range_iterator(f1, f2, f3);
		const std::size_t c = it.count();
		CHECK_EQ(c, 0);
	}

	SUBCASE("Check iterator bounds")
	{
		auto it = kd.get_const_iterator_begin();
		CHECK_EQ(it.end(), true);
		CHECK_EQ(it.past_begin(), true);
	}

	SUBCASE("Check range iterator bounds")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 1;
		};
		const auto f2 = [](const int v) -> bool
		{
			return 1 <= v and v <= 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);
		CHECK_EQ(it.end(), true);
		CHECK_EQ(it.past_begin(), true);
	}
}

int main(int argc, char **argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	return context.run(); // run doctest
}
