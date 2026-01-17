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

		CHECK_EQ(t.template total_bytes<true>(), 32);
		CHECK_EQ(t.template total_bytes<false>(), 32);
		t.add('a');
		CHECK_EQ(t.template total_bytes<true>(), 33);
		CHECK_EQ(t.template total_bytes<false>(), 33);
		t.add('b');
		CHECK_EQ(t.template total_bytes<true>(), 34);
		CHECK_EQ(t.template total_bytes<false>(), 34);
		t.add('c');
		CHECK_EQ(t.template total_bytes<true>(), 35);
		CHECK_EQ(t.template total_bytes<false>(), 35);
	}

	{
		classtree::ctree<char, int> t;
		CHECK_EQ(t.template total_bytes<true>(), 32);
		CHECK_EQ(t.template total_bytes<false>(), 32);
		t.add({'a', 1});
		CHECK_EQ(t.template total_bytes<true>(), 40);
		CHECK_EQ(t.template total_bytes<false>(), 40);
		t.add({'b', 2});
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 48);
		t.add({'c', 3});
		CHECK_EQ(t.template total_bytes<true>(), 56);
		CHECK_EQ(t.template total_bytes<false>(), 56);
	}

	{
		classtree::ctree<int, void> t;
		CHECK_EQ(t.template total_bytes<true>(), 32);
		CHECK_EQ(t.template total_bytes<false>(), 32);
		t.add(1);
		CHECK_EQ(t.template total_bytes<true>(), 36);
		CHECK_EQ(t.template total_bytes<false>(), 36);
		t.add(2);
		CHECK_EQ(t.template total_bytes<true>(), 40);
		CHECK_EQ(t.template total_bytes<false>(), 40);
		t.add(3);
		CHECK_EQ(t.template total_bytes<true>(), 44);
		CHECK_EQ(t.template total_bytes<false>(), 44);
	}

	{
		classtree::ctree<int, int> t;
		CHECK_EQ(t.template total_bytes<true>(), 32);
		CHECK_EQ(t.template total_bytes<false>(), 32);
		t.add({1, 1});
		CHECK_EQ(t.template total_bytes<true>(), 40);
		CHECK_EQ(t.template total_bytes<false>(), 40);
		t.add({2, 2});
		CHECK_EQ(t.template total_bytes<true>(), 48);
		CHECK_EQ(t.template total_bytes<false>(), 48);
		t.add({3, 3});
		CHECK_EQ(t.template total_bytes<true>(), 56);
		CHECK_EQ(t.template total_bytes<false>(), 56);
	}
}

TEST_CASE("1-level t")
{
	SUBCASE("1 branch")
	{
		struct pair_key_subt {
			char key;
			classtree::ctree<char, void> subt;
		};
		static constexpr size_t subtree_s = sizeof(pair_key_subt);
		static_assert(subtree_s == 40);

		classtree::ctree<char, void, int> t;
		CHECK_EQ(t.template total_bytes<true>(), 40);
		CHECK_EQ(t.template total_bytes<false>(), 40);
		t.add('a', 1);
		CHECK_EQ(t.template total_bytes<true>(), 113);
		CHECK_EQ(t.template total_bytes<false>(), 113);
		t.add('b', 1);
		CHECK_EQ(t.template total_bytes<true>(), 114);
		CHECK_EQ(t.template total_bytes<false>(), 114);
		t.add('c', 1);
		CHECK_EQ(t.template total_bytes<true>(), 115);
		CHECK_EQ(t.template total_bytes<false>(), 115);
	}

	SUBCASE("2 branch")
	{
		struct pair_key_subt {
			char key;
			classtree::ctree<char, void> subt;
		};
		static constexpr size_t subtree_s = sizeof(pair_key_subt);
		static_assert(subtree_s == 40);

		classtree::ctree<char, void, int> t;
		CHECK_EQ(t.template total_bytes<true>(), 40);
		CHECK_EQ(t.template total_bytes<false>(), 40);
		t.add('a', 1);
		CHECK_EQ(t.template total_bytes<true>(), 113);
		CHECK_EQ(t.template total_bytes<false>(), 113);
		t.add('b', 1);
		CHECK_EQ(t.template total_bytes<true>(), 114);
		CHECK_EQ(t.template total_bytes<false>(), 114);
		t.add('c', 1);
		CHECK_EQ(t.template total_bytes<true>(), 115);
		CHECK_EQ(t.template total_bytes<false>(), 115);
		t.add('a', 2);
		CHECK_EQ(t.template total_bytes<true>(), 188);
		CHECK_EQ(t.template total_bytes<false>(), 188);

		t.print(std::cout);
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
