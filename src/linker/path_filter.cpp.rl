#include <nonstd/optional.hpp>
#include <nonstd/string_view.hpp>
#include <parallel_hashmap/phmap.h>

#include <path.hpp>
#include <gcc/path.hpp>

namespace cc_wrapper {
namespace linker {
namespace path {

// clang-format off
%%{
  machine filter;

  action open { open_flag = arg; }
  action path { path = nonstd::string_view(p, pe-p); fbreak; }
  action ignore { check = false; }
  action skip { cannot_skip = false; }

  plugin = ( 'plugin' '-opt'? ) @ignore;
  short_long = '-'? ( 'dynamic-linker' | 'rpath' @skip | plugin );
  long = '-' ( 'library-path' @skip );
  long_combined = ( short_long | long ) %/open '=' any* >path;
  short = [IL] @skip %/open any* >path;
  flag = '-' ( long_combined | short );
  path = [^\-] @path;
  main := flag | path;

  write data;
}%%
// clang-format on

void appendGood(std::vector<nonstd::string_view> &new_args,
                nonstd::span<const nonstd::string_view> old_args,
                nonstd::span<const nonstd::string_view> pure_prefixes) {
  int cs;
  bool check, cannot_skip;
  nonstd::optional<nonstd::string_view> open_flag;
  for (const auto &arg : old_args) {
    if (open_flag) {
      if (!check || cc_wrapper::path::isPure(arg, pure_prefixes)) {
        new_args.push_back(*open_flag);
        new_args.push_back(arg);
      } else if (cannot_skip) {
        throw cc_wrapper::path::PurityError(arg);
      }
      open_flag = nonstd::nullopt;
      continue;
    }
    const char *p = arg.data();
    const char *pe = p + arg.size();
    const char *eof = pe;
    check = true;
    cannot_skip = true;
    nonstd::optional<nonstd::string_view> path;
    // clang-format off
    %% write init;
    %% write exec;
    // clang-format on
    if (open_flag)
      continue;
    if (path && check && !cc_wrapper::path::isPure(*path, pure_prefixes)) {
      if (cannot_skip)
        throw cc_wrapper::path::PurityError(*path);
      else
        continue;
    }
    new_args.push_back(arg);
  }
}

}  // namespace path
}  // namespace linker
}  // namespace cc_wrapper
