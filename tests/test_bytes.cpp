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

#undef DEBUG
#undef _GLIBCXX_DEBUG

#if defined DEBUG
#error("You should compile this only in release mode, otherwise the sizes will not match")
#endif

// C++ includes
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

// ct includes
#include <ctree/ctree.hpp>

template <typename data_t, typename metadata_t, typename... keys_t>
using ctree = classtree::ctree<data_t, metadata_t, keys_t...>;

using sstring = std::string;

TEST_CASE("Tree sizes")
{
	static_assert(sizeof(std::vector<char>) == 24);
	static_assert(sizeof(std::vector<int>) == 24);
	static_assert(sizeof(std::vector<std::pair<int, int>>) == 24);
	static_assert(sizeof(std::pmr::vector<char>) == 32);
	static_assert(sizeof(std::pmr::vector<int>) == 32);
	static_assert(sizeof(std::pmr::vector<std::pair<int, int>>) == 32);

	// leaf
	static_assert(sizeof(ctree<char, void>) == 32);
	static_assert(sizeof(ctree<char, int>) == 32);
	static_assert(sizeof(ctree<int, int>) == 32);

	// depth 1
	static_assert(sizeof(ctree<char, void, char>) == 40);
	static_assert(sizeof(ctree<char, int, int>) == 40);
	static_assert(sizeof(ctree<int, int, sstring>) == 40);

	// depth 2
	static_assert(sizeof(ctree<char, void, char, char>) == 40);
	static_assert(sizeof(ctree<char, int, char, int>) == 40);
	static_assert(sizeof(ctree<int, int, char, sstring>) == 40);

	static_assert(sizeof(ctree<char, void, int, char>) == 40);
	static_assert(sizeof(ctree<char, int, int, int>) == 40);
	static_assert(sizeof(ctree<int, int, int, sstring>) == 40);

	static_assert(sizeof(ctree<char, void, sstring, char>) == 40);
	static_assert(sizeof(ctree<char, int, sstring, int>) == 40);
	static_assert(sizeof(ctree<int, int, sstring, sstring>) == 40);

	// depth 3
	static_assert(sizeof(ctree<char, void, sstring, char, char>) == 40);
	static_assert(sizeof(ctree<char, int, sstring, char, int>) == 40);
	static_assert(sizeof(ctree<int, int, sstring, char, sstring>) == 40);

	static_assert(sizeof(ctree<sstring, void, sstring, char, char>) == 40);
	static_assert(sizeof(ctree<sstring, int, sstring, char, int>) == 40);
	static_assert(sizeof(ctree<sstring, int, sstring, int, sstring>) == 40);
}

TEST_CASE("Leaf t")
{
	{
		classtree::ctree<char, void> t;

		CHECK_EQ(t.template total_bytes<true>(), 0);
		CHECK_EQ(t.template total_bytes<false>(), 0);
		t.add('a');
		CHECK_EQ(t.template total_bytes<true>(), 1);
		CHECK_EQ(t.template total_bytes<false>(), 1);
		t.add('b');
		CHECK_EQ(t.template total_bytes<true>(), 2);
		CHECK_EQ(t.template total_bytes<false>(), 2);
		t.add('c');
		CHECK_EQ(t.template total_bytes<true>(), 3);
		CHECK_EQ(t.template total_bytes<false>(), 3);
	}

	{
		classtree::ctree<char, int> t;
		CHECK_EQ(t.template total_bytes<true>(), 0);
		CHECK_EQ(t.template total_bytes<false>(), 0);
		t.add({'a', 1});
		CHECK_EQ(t.template total_bytes<true>(), 8);
		CHECK_EQ(t.template total_bytes<false>(), 8);
		t.add({'b', 2});
		CHECK_EQ(t.template total_bytes<true>(), 16);
		CHECK_EQ(t.template total_bytes<false>(), 16);
		t.add({'c', 3});
		CHECK_EQ(t.template total_bytes<true>(), 24);
		CHECK_EQ(t.template total_bytes<false>(), 24);
	}

	{
		classtree::ctree<int, void> t;
		CHECK_EQ(t.template total_bytes<true>(), 0);
		CHECK_EQ(t.template total_bytes<false>(), 0);
		t.add(1);
		CHECK_EQ(t.template total_bytes<true>(), 4);
		CHECK_EQ(t.template total_bytes<false>(), 4);
		t.add(2);
		CHECK_EQ(t.template total_bytes<true>(), 8);
		CHECK_EQ(t.template total_bytes<false>(), 8);
		t.add(3);
		CHECK_EQ(t.template total_bytes<true>(), 12);
		CHECK_EQ(t.template total_bytes<false>(), 12);
	}

	{
		classtree::ctree<int, int> t;
		CHECK_EQ(t.template total_bytes<true>(), 0);
		CHECK_EQ(t.template total_bytes<false>(), 0);
		t.add({'a', 1});
		CHECK_EQ(t.template total_bytes<true>(), 8);
		CHECK_EQ(t.template total_bytes<false>(), 8);
		t.add({'b', 2});
		CHECK_EQ(t.template total_bytes<true>(), 16);
		CHECK_EQ(t.template total_bytes<false>(), 16);
		t.add({'c', 3});
		CHECK_EQ(t.template total_bytes<true>(), 24);
		CHECK_EQ(t.template total_bytes<false>(), 24);
	}
}

TEST_CASE("1-level t")
{
	struct pair_key_subt {
		char key;
		classtree::ctree<char, void> subt;
	};

	SUBCASE("First branching")
	{
		classtree::ctree<char, void, int> t;
		CHECK_EQ(t.template total_bytes<true>(), 0);
		CHECK_EQ(t.template total_bytes<false>(), 0);

		// 1 branch
		t.add('a', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 41);
		t.add('b', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 42);
		t.add('c', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 43);

		// 2 branches
		t.add('d', 2);
		CHECK_EQ(t.template total_bytes<true>(), 96);
		CHECK_EQ(t.template total_bytes<false>(), 84);
	}

	SUBCASE("Second branching")
	{
		classtree::ctree<char, void, int> t;
		CHECK_EQ(t.template total_bytes<true>(), 0);
		CHECK_EQ(t.template total_bytes<false>(), 0);

		// 1 branch
		t.add('1', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 41);
		t.add('2', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 42);
		t.add('3', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 43);
		t.add('4', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 44);
		t.add('5', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 45);
		t.add('6', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 46);
		t.add('7', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 47);
		t.add('8', 1);
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 48);
		t.add('9', 1);
		CHECK_EQ(t.template total_bytes<true>(), 56);
		CHECK_EQ(t.template total_bytes<false>(), 49);
		t.add('A', 1);
		CHECK_EQ(t.template total_bytes<true>(), 56);
		CHECK_EQ(t.template total_bytes<false>(), 50);
		t.add('B', 1);
		CHECK_EQ(t.template total_bytes<true>(), 56);
		CHECK_EQ(t.template total_bytes<false>(), 51);
		t.add('C', 1);
		CHECK_EQ(t.template total_bytes<true>(), 56);
		CHECK_EQ(t.template total_bytes<false>(), 52);

		// 2 branches
		t.add('d', 2);
		CHECK_EQ(t.template total_bytes<true>(), 104);
		CHECK_EQ(t.template total_bytes<false>(), 93);
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
