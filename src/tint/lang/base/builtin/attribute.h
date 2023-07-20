// Copyright 2023 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

////////////////////////////////////////////////////////////////////////////////
// File generated by tools/src/cmd/gen
// using the template:
//   src/tint/lang/base/builtin/attribute.h.tmpl
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_TINT_LANG_BASE_BUILTIN_ATTRIBUTE_H_
#define SRC_TINT_LANG_BASE_BUILTIN_ATTRIBUTE_H_

#include "src/tint/core/string_stream.h"

/// \cond DO_NOT_DOCUMENT
/// There is a bug in doxygen where this enum conflicts with the ast::Attribute
/// and generates invalid documentation errors.
namespace tint::builtin {

/// Address space of a given pointer.
enum class Attribute {
    kUndefined,
    kAlign,
    kBinding,
    kBuiltin,
    kCompute,
    kDiagnostic,
    kFragment,
    kGroup,
    kId,
    kIndex,
    kInterpolate,
    kInvariant,
    kLocation,
    kMustUse,
    kSize,
    kVertex,
    kWorkgroupSize,
};

/// @param out the stream to write to
/// @param value the Attribute
/// @returns `out` so calls can be chained
utils::StringStream& operator<<(utils::StringStream& out, Attribute value);

/// ParseAttribute parses a Attribute from a string.
/// @param str the string to parse
/// @returns the parsed enum, or Attribute::kUndefined if the string could not be parsed.
Attribute ParseAttribute(std::string_view str);

constexpr const char* kAttributeStrings[] = {
    "align",    "binding", "builtin", "compute",        "diagnostic", "fragment",
    "group",    "id",      "index",   "interpolate",    "invariant",  "location",
    "must_use", "size",    "vertex",  "workgroup_size",
};

}  // namespace tint::builtin
/// \endcond

#endif  // SRC_TINT_LANG_BASE_BUILTIN_ATTRIBUTE_H_