#pragma once
#include <algorithm>
#include <nonstd/optional.hpp>
#include <nonstd/span.hpp>
#include <nonstd/string_view.hpp>

namespace cc_wrapper {
namespace util {
namespace detail {

char *append(char *out, nonstd::string_view str);
nonstd::string_view removeTrailing(nonstd::string_view path);

}  // namespace detail

nonstd::optional<nonstd::string_view> getenv(nonstd::string_view var);
nonstd::string_view dirname(nonstd::string_view path);
nonstd::string_view basename(nonstd::string_view path);

void exec(nonstd::string_view bin,
          nonstd::span<const nonstd::string_view> args);

template <typename A, typename B, size_t N>
bool spanEqual(nonstd::span<A, N> a, nonstd::span<B, N> b) {
  return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
}

}  // namespace util
}  // namespace cc_wrapper
