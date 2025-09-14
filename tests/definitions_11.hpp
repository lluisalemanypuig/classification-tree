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

#pragma once

// C++ includes
#include <ostream>

// ctree includes
#include <ctree/concepts.hpp>

struct d11 {
	int n_white_pawns;
	int n_black_pawns;
	int n_white_rooks;
	int n_black_rooks;
	int n_white_knights;
	int n_black_knights;
	int n_white_bishops;
	int n_black_bishops;
	int n_white_queens;
	int n_black_queens;

	int player_turn;

	d11(int wp,
		int bp,
		int wr,
		int br,
		int wn,
		int bn,
		int wb,
		int bb,
		int wq,
		int bq,
		int t)
	{
		n_white_pawns = wp;
		n_black_pawns = bp;
		n_white_rooks = wr;
		n_black_rooks = br;
		n_white_knights = wn;
		n_black_knights = bn;
		n_white_bishops = wb;
		n_black_bishops = bb;
		n_white_queens = wq;
		n_black_queens = bq;
		player_turn = t;
	}

	// this is needed for DOCTEST
	[[nodiscard]] bool operator== (const d11& o) const noexcept
	{
		return n_white_pawns == o.n_white_pawns and
			   n_black_pawns == o.n_black_pawns and
			   n_white_rooks == o.n_white_rooks and
			   n_black_rooks == o.n_black_rooks and
			   n_white_knights == o.n_white_knights and
			   n_black_knights == o.n_black_knights and
			   n_white_bishops == o.n_white_bishops and
			   n_black_bishops == o.n_black_bishops and
			   n_white_queens == o.n_white_queens and
			   n_black_queens == o.n_black_queens and
			   player_turn == o.player_turn;
	}

	[[nodiscard]] std::strong_ordering operator<=> (const d11& o) const noexcept
	{
		// clang-format off
		if (n_white_pawns == o.n_white_pawns) {
			if (n_black_pawns == o.n_black_pawns) {

				if (n_white_rooks == o.n_white_rooks) {
					if (n_black_rooks == o.n_black_rooks) {

						if (n_white_knights == o.n_white_knights) {
							if (n_black_knights == o.n_black_knights) {

								if (n_white_bishops == o.n_white_bishops) {
									if (n_black_bishops == o.n_black_bishops) {

										if (n_white_queens == o.n_white_queens) {
											if (n_black_queens == o.n_black_queens) {

												return player_turn <=> o.player_turn;

											}
											return n_black_queens <=> o.n_black_queens;
										}
										return n_white_queens <=> o.n_white_queens;
									}

									return n_black_bishops <=> o.n_black_bishops;
								}
								return n_white_bishops <=> o.n_white_bishops;
							}

							return n_black_knights <=> o.n_black_knights;
						}
						return n_white_knights <=> o.n_white_knights;
					}

					return n_black_rooks <=> o.n_black_rooks;
				}
				return n_white_rooks <=> o.n_white_rooks;
			}

			return n_black_pawns <=> o.n_black_pawns;
		}
		return n_white_pawns <=> o.n_white_pawns;
		// clang-format on
	}

	inline friend std::ostream&
	operator<< (std::ostream& os, const d11& c) noexcept
	{
		os << c.n_white_pawns << ' ' << c.n_black_pawns << ' '
		   << c.n_white_rooks << ' ' << c.n_black_rooks << ' '
		   << c.n_white_knights << ' ' << c.n_black_knights << ' '
		   << c.n_white_bishops << ' ' << c.n_black_bishops << ' '
		   << c.n_white_queens << ' ' << c.n_black_queens << ' '
		   << c.player_turn;
		return os;
	}
};

struct m11 {
	int num_occs = 0;

	m11& operator+= (const m11& m) noexcept
	{
		num_occs += m.num_occs;
		return *this;
	}

	[[nodiscard]] bool operator== (const m11& other) const noexcept
	{
		return num_occs == other.num_occs;
	}

	inline friend std::ostream&
	operator<< (std::ostream& os, const m11& c) noexcept
	{
		os << '{' << c.num_occs << '}';
		return os;
	}
};
