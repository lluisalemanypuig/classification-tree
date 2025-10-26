// Google Benchmark includes
#include <benchmark/benchmark.h>

// ctree includes
#include <ctree/search.hpp>

#define ARGUMENT_LIST                                                          \
	->Arg(1)                                                                   \
		->Arg(2)                                                               \
		->Arg(3)                                                               \
		->Arg(4)                                                               \
		->Arg(5)                                                               \
		->Arg(6)                                                               \
		->Arg(7)                                                               \
		->Arg(8)                                                               \
		->Arg(9)                                                               \
		->Arg(10)                                                              \
		->Arg(11)                                                              \
		->Arg(12)                                                              \
		->Arg(13)                                                              \
		->Arg(14)                                                              \
		->Arg(15)                                                              \
		->Arg(16)                                                              \
		->Arg(17)                                                              \
		->Arg(18)                                                              \
		->Arg(19)                                                              \
		->Arg(20)                                                              \
		->Arg(21)                                                              \
		->Arg(22)                                                              \
		->Arg(23)                                                              \
		->Arg(24)                                                              \
		->Arg(25)                                                              \
		->Arg(26)                                                              \
		->Arg(27)                                                              \
		->Arg(28)                                                              \
		->Arg(29)                                                              \
		->Arg(30)                                                              \
		->Arg(31)                                                              \
		->Arg(32)                                                              \
		->Arg(33)                                                              \
		->Arg(34)                                                              \
		->Arg(35)                                                              \
		->Arg(36)                                                              \
		->Arg(37)                                                              \
		->Arg(38)                                                              \
		->Arg(39)                                                              \
		->Arg(40)                                                              \
		->Arg(41)                                                              \
		->Arg(42)                                                              \
		->Arg(43)                                                              \
		->Arg(44)                                                              \
		->Arg(45)                                                              \
		->Arg(46)                                                              \
		->Arg(47)                                                              \
		->Arg(48)                                                              \
		->Arg(49)                                                              \
		->Arg(50)                                                              \
		->Arg(51)                                                              \
		->Arg(52)                                                              \
		->Arg(53)                                                              \
		->Arg(54)                                                              \
		->Arg(55)                                                              \
		->Arg(56)                                                              \
		->Arg(57)                                                              \
		->Arg(58)                                                              \
		->Arg(59)                                                              \
		->Arg(60)                                                              \
		->Arg(61)                                                              \
		->Arg(62)                                                              \
		->Arg(63)                                                              \
		->Arg(64)                                                              \
		->Arg(65)                                                              \
		->Arg(66)                                                              \
		->Arg(67)                                                              \
		->Arg(68)                                                              \
		->Arg(69)                                                              \
		->Arg(70)                                                              \
		->Arg(71)                                                              \
		->Arg(72)                                                              \
		->Arg(73)                                                              \
		->Arg(74)                                                              \
		->Arg(75)                                                              \
		->Arg(76)                                                              \
		->Arg(77)                                                              \
		->Arg(78)                                                              \
		->Arg(79)                                                              \
		->Arg(80)                                                              \
		->Arg(81)                                                              \
		->Arg(82)                                                              \
		->Arg(83)                                                              \
		->Arg(84)                                                              \
		->Arg(85)                                                              \
		->Arg(86)                                                              \
		->Arg(87)                                                              \
		->Arg(88)                                                              \
		->Arg(89)                                                              \
		->Arg(90)                                                              \
		->Arg(91)                                                              \
		->Arg(92)                                                              \
		->Arg(93)                                                              \
		->Arg(94)                                                              \
		->Arg(95)                                                              \
		->Arg(96)                                                              \
		->Arg(97)                                                              \
		->Arg(98)                                                              \
		->Arg(99)                                                              \
		->Arg(100)                                                             \
		->Arg(110)                                                             \
		->Arg(120)                                                             \
		->Arg(130)                                                             \
		->Arg(140)                                                             \
		->Arg(150)                                                             \
		->Arg(160)                                                             \
		->Arg(170)                                                             \
		->Arg(180)                                                             \
		->Arg(190)                                                             \
		->Arg(200)

template <typename Contained, typename Callable>
void custom_search(const std::vector<Contained>& v, const Callable& f)
{
	for (size_t j = 0; j < 2 * v.size(); ++j) {
		f(v, j);
	}
}

static void pair_search_linear(benchmark::State& state)
{
	const size_t n = static_cast<size_t>(state.range(0));

	size_t j = 2;
	std::vector<std::pair<size_t, size_t>> v(n);
	for (size_t i = 0; i < n; ++i) {
		v.push_back({j, j});
		j += 2;
	}

	for (auto _ : state) {
		custom_search(
			v,
			[]<typename Vec, typename Value>(const Vec& vv, Value val)
			{
				auto asdf = classtree::detail::pair_search_linear(vv, val);
				benchmark::DoNotOptimize(asdf);
			}
		);
	}
}
BENCHMARK(pair_search_linear) ARGUMENT_LIST;

static void pair_search_binary(benchmark::State& state)
{
	const size_t n = static_cast<size_t>(state.range(0));

	size_t j = 2;
	std::vector<std::pair<size_t, size_t>> v(n);
	for (size_t i = 0; i < n; ++i) {
		v.push_back({j, j});
		j += 2;
	}

	for (auto _ : state) {
		custom_search(
			v,
			[]<typename Vec, typename Value>(const Vec& vv, Value val)
			{
				auto asdf = classtree::detail::pair_search_binary(vv, val);
				benchmark::DoNotOptimize(asdf);
			}
		);
	}
}
BENCHMARK(pair_search_binary) ARGUMENT_LIST;

static void element_search_linear(benchmark::State& state)
{
	const size_t n = static_cast<size_t>(state.range(0));

	size_t j = 2;
	std::vector<classtree::element_t<size_t, size_t>> v(n);
	for (size_t i = 0; i < n; ++i) {
		v.push_back({j, j});
		j += 2;
	}

	for (auto _ : state) {
		custom_search(
			v,
			[]<typename Vec, typename Value>(const Vec& vv, Value val)
			{
				auto asdf =
					classtree::detail::element_search_linear<size_t, size_t>(
						vv, val
					);
				benchmark::DoNotOptimize(asdf);
			}
		);
	}
}
BENCHMARK(element_search_linear) ARGUMENT_LIST;

static void element_search_binary(benchmark::State& state)
{
	const size_t n = static_cast<size_t>(state.range(0));

	size_t j = 2;
	std::vector<classtree::element_t<size_t, size_t>> v(n);
	for (size_t i = 0; i < n; ++i) {
		v.push_back({j, j});
		j += 2;
	}

	for (auto _ : state) {
		custom_search(
			v,
			[]<typename Vec, typename Value>(const Vec& vv, Value val)
			{
				auto asdf =
					classtree::detail::element_search_binary<size_t, size_t>(
						vv, val
					);
				benchmark::DoNotOptimize(asdf);
			}
		);
	}
}
BENCHMARK(element_search_binary) ARGUMENT_LIST;

BENCHMARK_MAIN();
