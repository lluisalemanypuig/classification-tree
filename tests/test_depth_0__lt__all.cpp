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

TEST_CASE("All elements")
{
	classtree::ctree<data_lt, meta_incr> kd;

	kd.add<false>({{.i = 1, .j = 1, .k = 1, .z = 2}, {.num_occs = 1}});
	kd.add<false>({{.i = 1, .j = 1, .k = 1, .z = 1}, {.num_occs = 1}});
	kd.add<false>({{.i = 1, .j = 1, .k = 1, .z = 3}, {.num_occs = 1}});
	kd.add<false>({{.i = 1, .j = 1, .k = 1, .z = 4}, {.num_occs = 1}});
	kd.add<false>({{.i = 1, .j = 2, .k = 1, .z = 1}, {.num_occs = 1}});
	kd.add<false>({{.i = 1, .j = 2, .k = 2, .z = 1}, {.num_occs = 1}});
	kd.add<false>({{.i = 1, .j = 3, .k = 5, .z = 1}, {.num_occs = 1}});
	kd.add<false>({{.i = 1, .j = 1, .k = 1, .z = 2}, {.num_occs = 1}}); // *
	kd.add<false>({{.i = 2, .j = 2, .k = 2, .z = 1}, {.num_occs = 1}});
	kd.add<false>({{.i = 2, .j = 2, .k = 2, .z = 2}, {.num_occs = 1}});
	kd.add<false>({{.i = 2, .j = 2, .k = 3, .z = 2}, {.num_occs = 1}});

	CHECK_EQ(kd.size(), 11);

	SUBCASE("Print entire tree")
	{

		static constexpr std::string_view kd_str = "^ size: 11 11\n"
												   "├── (1 1 1 1) {1}\n"
												   "├── (1 1 1 2) {1}\n"
												   "├── (1 1 1 2) {1}\n"
												   "├── (1 1 1 3) {1}\n"
												   "├── (1 1 1 4) {1}\n"
												   "├── (1 2 1 1) {1}\n"
												   "├── (1 2 2 1) {1}\n"
												   "├── (1 3 5 1) {1}\n"
												   "├── (2 2 2 1) {1}\n"
												   "├── (2 2 2 2) {1}\n"
												   "└── (2 2 3 2) {1}\n";

		const std::string print_str_const = print_string(kd);
		CHECK_EQ(print_str_const, kd_str);
	}

	SUBCASE("Iterate entire tree forward")
	{

		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 1 1 1) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 1 1 3) {1}\n"
														"    (1 1 1 4) {1}\n"
														"    (1 2 1 1) {1}\n"
														"    (1 2 2 1) {1}\n"
														"    (1 3 5 1) {1}\n"
														"    (2 2 2 1) {1}\n"
														"    (2 2 2 2) {1}\n"
														"    (2 2 3 2) {1}\n";

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

		static constexpr std::string_view kd_iter_str_fb =
			"Iterate:\n"
			"    (2 2 3 2) {1}\n"
			"    (2 2 2 2) {1}\n"
			"    (2 2 2 1) {1}\n"
			"    (1 3 5 1) {1}\n"
			"    (1 2 2 1) {1}\n"
			"    (1 2 1 1) {1}\n"
			"    (1 1 1 4) {1}\n"
			"    (1 1 1 3) {1}\n"
			"    (1 1 1 2) {1}\n"
			"    (1 1 1 2) {1}\n"
			"    (1 1 1 1) {1}\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_iterator_end();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str_fb, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_iterator_end();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str_fb, iter_str);
	}

	SUBCASE("Manual iteration")
	{
		auto it = kd.get_const_iterator_begin();
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 1});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 1});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 1});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 3});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).data, data_lt{.i = 1, .j = 1, .k = 1, .z = 4});
		CHECK_EQ((*it).metadata, meta_incr{.num_occs = 1});
	}

	SUBCASE("Check iterator bounds")
	{
		auto it = kd.get_const_iterator_begin();

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		for (int i = 0; i < 3; ++i) {
			--it;
			CHECK_EQ(it.past_begin(), true);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), false);

			++it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), true);
			CHECK_EQ(it.end(), false);
		}

		size_t c = 0;
		for (size_t i = 0; i < kd.size(); ++i) {
			CHECK_EQ(it.past_begin(), false);
			if (i > 0) {
				CHECK_EQ(it.begin(), false);
			}
			CHECK_EQ(it.end(), false);

			++it;
			++c;
		}
		CHECK_EQ(kd.size(), c);

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);

		for (int i = 0; i < 3; ++i) {
			--it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), false);

			++it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), true);
		}
	}

	SUBCASE("Check range iterator bounds")
	{
		auto it = kd.get_const_range_iterator_begin();

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		for (int i = 0; i < 3; ++i) {
			--it;
			CHECK_EQ(it.past_begin(), true);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), false);

			++it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), true);
			CHECK_EQ(it.end(), false);
		}

		size_t c = 0;
		for (size_t i = 0; i < kd.size(); ++i) {
			CHECK_EQ(it.past_begin(), false);
			if (i > 0) {
				CHECK_EQ(it.begin(), false);
			}
			CHECK_EQ(it.end(), false);

			++it;
			++c;
		}
		CHECK_EQ(kd.size(), c);

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);

		for (int i = 0; i < 3; ++i) {
			--it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), false);

			++it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), true);
		}
	}
}

int main(int argc, char **argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	return context.run(); // run doctest
}
