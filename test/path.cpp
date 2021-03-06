#include <catch2/catch.hpp>
#include <vector>

#include <env.hpp>
#include <path.hpp>

namespace cc_wrapper {
namespace path {

TEST_CASE("Impure", "[isPure]") {
  env::detail::enforcingPurity() = false;
  CHECK(isPure("/dev/null", {}));
  CHECK(isPure("./local/file", {}));
  CHECK(isPure("local", {}));
  CHECK(!isPure("/no-such-path", {}));
  CHECK(!isPure("/no-such-path/file", {}));
  CHECK(isPure("/usr/bin/bash", {}));
}

TEST_CASE("Pure", "[isPure]") {
  env::detail::enforcingPurity() = true;
  CHECK(isPure("/dev/null", {}));
  CHECK(isPure("./local/file", {}));
  CHECK(isPure("local", {}));
  CHECK(!isPure("/no-such-path", {}));
  CHECK(!isPure("/no-such-path/file", {}));
  CHECK(!isPure("/usr/bin/bash", {}));
  const std::vector<nonstd::string_view> prefixes = {"/usr", "/build"};
  CHECK(isPure("/usr/bin/bash", prefixes));
  CHECK(isPure("/build/bash", prefixes));
  CHECK(!isPure("/tmp/bash", prefixes));
}

TEST_CASE("Absolute", "[isAbsolute]") {
  CHECK(!isAbsolute(""));
  CHECK(isAbsolute("/no-such-path/hi"));
  CHECK(!isAbsolute("./local"));
  CHECK(!isAbsolute("local"));
}

TEST_CASE("Canonicalize Path", "[canonicalize]") {
  CHECK(canonicalize("/") == "/");
  CHECK(canonicalize(".") == ".");
  CHECK(canonicalize("..") == "..");
  CHECK(canonicalize("abc") == "abc");
  CHECK(canonicalize("/abc") == "/abc");

  CHECK(canonicalize("///") == "/");
  CHECK(canonicalize("//././") == "/");
  CHECK(canonicalize("/../..") == "/");

  CHECK(canonicalize("") == ".");
  CHECK(canonicalize("./") == ".");
  CHECK(canonicalize("./.") == ".");

  CHECK(canonicalize("./..") == "..");
  CHECK(canonicalize("../..") == "../..");

  CHECK(canonicalize("./abc") == "abc");
  CHECK(canonicalize("./abc/def") == "abc/def");
  CHECK(canonicalize("./abc/..") == ".");

  CHECK(canonicalize("/abc/./.././lib") == "/lib");
  CHECK(canonicalize("../abc/../lib") == "../lib");
  CHECK(canonicalize("abc/..") == ".");
  CHECK(canonicalize("abc/../") == ".");
  CHECK(canonicalize("abc/def/../..") == ".");
  CHECK(canonicalize("abc/../def/..") == ".");
  CHECK(canonicalize("lib/abc/../def/..") == "lib");
  CHECK(canonicalize("lib/abc/../def/../z") == "lib/z");
}

TEST_CASE("Has Prefix", "[startsWith]") {
  CHECK(startsWith("/usr2", "/"));
  CHECK(!startsWith("/usr2", "/usr"));
  CHECK(startsWith("/usr/include", "/usr"));
  CHECK(!startsWith("/data", "/usr"));
  CHECK(!startsWith("/", "/data"));
  CHECK(!startsWith("/usr", "/usr///"));
  CHECK(!startsWith("abc", "/"));
}

}  // namespace path
}  // namespace cc_wrapper
