/*
 * Copyright 2017 cc-wrapper authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include <string_view>

#include "string-util.h"

namespace {

using std::string_view;

TEST(StringUtilTest, StringClone) {
  const char *expected = "this test str";
  char *clone = string_clone(expected);
  EXPECT_NE(expected, clone);
  EXPECT_EQ(string_view(expected), string_view(clone));
  free(clone);
}

}  // namespace