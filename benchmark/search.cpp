// Google Benchmark includes
#include <benchmark/benchmark.h>

// ctree includes
#include <ctree/search.hpp>

#define ARGUMENT_LIST                                                          \
	->Arg(2)                                                                   \
		->Arg(3)                                                               \
		->Arg(4)                                                               \
		->Arg(5)                                                               \
		->Arg(6)                                                               \
		->Arg(7)                                                               \
		->Arg(8)                                                               \
		->Arg(9)                                                               \
		->Arg(10)                                                              \
		->Arg(11)                                                              \
		->Arg(12)

template <typename Contained, typename Value, typename Callable>
void custom_search(
	const std::vector<Contained>& v, Value max_val, const Callable& f
)
{
	for (size_t j = 0; j < max_val + 150; ++j) {
		f(v, j);
	}
}

static void pair_search_linear(benchmark::State& state)
{
	const size_t n = static_cast<size_t>(state.range(0));

	size_t j = 150;
	std::vector<std::pair<size_t, size_t>> v(n);
	for (size_t i = 0; i < n; ++i) {
		v.push_back({j, j});
		j += 2;
	}

	for (auto _ : state) {
		custom_search(
			v,
			v.back().first,
			[](const auto& vv, auto val)
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

	size_t j = 150;
	std::vector<std::pair<size_t, size_t>> v(n);
	for (size_t i = 0; i < n; ++i) {
		v.push_back({j, j});
		j += 2;
	}

	for (auto _ : state) {
		custom_search(
			v,
			v.back().first,
			[](const auto& vv, auto val)
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

	size_t j = 150;
	std::vector<classtree::element_t<size_t, size_t>> v(n);
	for (size_t i = 0; i < n; ++i) {
		v.push_back({j, j});
		j += 2;
	}

	for (auto _ : state) {
		custom_search(
			v,
			v.back().data,
			[](const auto& vv, auto val)
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

	size_t j = 150;
	std::vector<classtree::element_t<size_t, size_t>> v(n);
	for (size_t i = 0; i < n; ++i) {
		v.push_back({j, j});
		j += 2;
	}

	for (auto _ : state) {
		custom_search(
			v,
			v.back().data,
			[](const auto& vv, auto val)
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
