#pragma once
#include <nonstd/span.hpp>
#include <nonstd/string_view.hpp>
#include <stdexcept>
#include <string>

namespace cc_wrapper {
namespace path {

class PurityError : public std::runtime_error {
public:
  PurityError(nonstd::string_view path);
};

bool isPure(nonstd::string_view path,
            nonstd::span<const nonstd::string_view> pure_prefixes);

bool isAbsolute(nonstd::string_view path);
std::string canonicalize(nonstd::string_view path);
void canonicalizeInPlace(std::string &path);

bool startsWith(nonstd::string_view path, nonstd::string_view prefix);

}  // namespace path
}  // namespace cc_wrapper
