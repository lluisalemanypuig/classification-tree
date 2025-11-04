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

#define DOCTEST_CONFIG_IMPLEMENT

// C++ includes
#include <doctest/doctest.h>
#include <print>

// ctree includes
#include <ctree/search.hpp>

TEST_CASE("Empty vector")
{
	std::vector<std::pair<int, int>> v;
	std::vector<classtree::element_t<int, int>> v2;

	for (int i = 1; i <= 9; ++i) {
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_linear(v, i);
			CHECK(not found);
			CHECK_EQ(pos, 0);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_binary(v, i);
			CHECK(not found);
			CHECK_EQ(pos, 0);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_linear<int, int>(v2, i);
			CHECK(not found);
			CHECK_EQ(pos, 0);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_binary<int, int>(v2, i);
			CHECK(not found);
			CHECK_EQ(pos, 0);
		}
	}
}

TEST_CASE("Singleton vector")
{
	std::vector<std::pair<int, int>> v{{5, 5}};
	std::vector<classtree::element_t<int, int>> v2{{5, 5}};

	for (int i = 1; i <= 4; ++i) {
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_linear(v, i);
			CHECK(not found);
			CHECK_EQ(pos, 0);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_binary(v, i);
			CHECK(not found);
			CHECK_EQ(pos, 0);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_linear<int, int>(v2, i);
			CHECK(not found);
			CHECK_EQ(pos, 0);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_binary<int, int>(v2, i);
			CHECK(not found);
			CHECK_EQ(pos, 0);
		}
	}

	{
		const auto [pos, found] =
			classtree::detail::small_pair_search_linear(v, 5);
		CHECK(found);
		CHECK_EQ(pos, 0);
	}
	{
		const auto [pos, found] =
			classtree::detail::small_pair_search_binary(v, 5);
		CHECK(found);
		CHECK_EQ(pos, 0);
	}
	{
		const auto [pos, found] =
			classtree::detail::small_element_search_linear<int, int>(v2, 5);
		CHECK(found);
		CHECK_EQ(pos, 0);
	}
	{
		const auto [pos, found] =
			classtree::detail::small_element_search_binary<int, int>(v2, 5);
		CHECK(found);
		CHECK_EQ(pos, 0);
	}

	for (int i = 6; i <= 9; ++i) {
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_linear(v, i);
			CHECK(not found);
			CHECK_EQ(pos, 1);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_binary(v, i);
			CHECK(not found);
			CHECK_EQ(pos, 1);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_linear<int, int>(v2, i);
			CHECK(not found);
			CHECK_EQ(pos, 1);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_binary<int, int>(v2, i);
			CHECK(not found);
			CHECK_EQ(pos, 1);
		}
	}
}

TEST_CASE("Tiny vector")
{
	std::vector<std::pair<size_t, size_t>> v;
	std::vector<classtree::element_t<size_t, size_t>> v2;
	for (size_t i = 1; i <= 9; i += 2) {
		v.push_back({i, i});
		classtree::pair<size_t, size_t> p;
		p.data = p.metadata = i;
		v2.push_back(p);
	}

	for (size_t i = 1; i <= 9; i += 2) {
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_linear(v, i);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_binary(v, i);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_linear<size_t, size_t>(
					v2, i
				);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_binary<size_t, size_t>(
					v2, i
				);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
	}

	for (size_t i = 0; i <= 10; i += 2) {
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_linear(v, i);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_binary(v, i);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_linear<size_t, size_t>(
					v2, i
				);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_binary<size_t, size_t>(
					v2, i
				);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
	}
}

TEST_CASE("Small vector")
{
	std::vector<std::pair<size_t, size_t>> v;
	std::vector<classtree::element_t<size_t, size_t>> v2;
	for (size_t i = 1; i <= 17; i += 2) {
		v.push_back({i, i});
		classtree::pair<size_t, size_t> p;
		p.data = p.metadata = i;
		v2.push_back(p);
	}

	for (size_t i = 1; i <= 17; i += 2) {
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_linear(v, i);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_binary(v, i);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_linear<size_t, size_t>(
					v2, i
				);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_binary<size_t, size_t>(
					v2, i
				);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
	}

	for (size_t i = 0; i <= 18; i += 2) {
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_linear(v, i);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_binary(v, i);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_linear<size_t, size_t>(
					v2, i
				);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_binary<size_t, size_t>(
					v2, i
				);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
	}
}

TEST_CASE("Large vector")
{
	std::vector<std::pair<size_t, size_t>> v;
	std::vector<classtree::element_t<size_t, size_t>> v2;
	for (size_t i = 1; i <= 301; i += 2) {
		v.push_back({i, i});
		classtree::pair<size_t, size_t> p;
		p.data = p.metadata = i;
		v2.push_back(p);
	}

	for (size_t i = 1; i <= 301; i += 2) {
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_linear(v, i);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_binary(v, i);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_linear<size_t, size_t>(
					v2, i
				);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_binary<size_t, size_t>(
					v2, i
				);
			CHECK(found);
			CHECK_EQ(pos, (i - 1) / 2);
		}
	}

	for (size_t i = 0; i <= 302; i += 2) {
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_linear(v, i);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_pair_search_binary(v, i);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_linear<size_t, size_t>(
					v2, i
				);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
		{
			const auto [pos, found] =
				classtree::detail::small_element_search_binary<size_t, size_t>(
					v2, i
				);
			CHECK(not found);
			CHECK_EQ(pos, i / 2);
		}
	}
}

int main(int argc, char **argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	const int res = context.run(); // run doctest

	// important - query flags (and --exit) rely on the user doing this
	if (context.shouldExit()) {
		// propagate the result of the tests
		return res;
	}

	return res;
}
