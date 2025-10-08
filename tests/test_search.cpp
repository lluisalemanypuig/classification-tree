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

// ctree includes
#include <ctree/search.hpp>

TEST_CASE("Small vector")
{
	std::vector<std::pair<int, int>> v;
	for (size_t i = 1; i <= 17; i += 2) {
		v.push_back({i, i});
	}

	for (int i = 1; i <= 17; i += 2) {
		const auto [pos, found] = classtree::search(v, i);
		CHECK(found);
		CHECK_EQ(pos, (i - 1) / 2);
	}

	for (int i = 0; i <= 18; i += 2) {
		const auto [pos, found] = classtree::search(v, i);
		CHECK(not found);
		CHECK_EQ(pos, i / 2);
	}
}

TEST_CASE("Large vector")
{
	std::vector<std::pair<int, int>> v;
	for (size_t i = 1; i <= 301; i += 2) {
		v.push_back({i, i});
	}

	for (int i = 1; i <= 301; i += 2) {
		const auto [pos, found] = classtree::search(v, i);
		CHECK(found);
		CHECK_EQ(pos, (i - 1) / 2);
	}

	for (int i = 0; i <= 302; i += 2) {
		const auto [pos, found] = classtree::search(v, i);
		CHECK(not found);
		CHECK_EQ(pos, i / 2);
	}
}

int main(int argc, char **argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	return context.run(); // run doctest
}
