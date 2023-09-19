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
// File generated by 'tools/src/cmd/gen' using the template:
//   src/tint/lang/spirv/ir/intrinsic.cc.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "src/tint/lang/spirv/ir/intrinsic.h"

namespace tint::spirv::ir {

/// ParseIntrinsic parses a Intrinsic from a string.
/// @param str the string to parse
/// @returns the parsed enum, or Intrinsic::kUndefined if the string could not be parsed.
Intrinsic ParseIntrinsic(std::string_view str) {
    if (str == "image_query_size") {
        return Intrinsic::kImageQuerySize;
    }
    if (str == "image_query_size_lod") {
        return Intrinsic::kImageQuerySizeLod;
    }
    if (str == "image_sample_dref_explicit_lod") {
        return Intrinsic::kImageSampleDrefExplicitLod;
    }
    if (str == "image_sample_dref_implicit_lod") {
        return Intrinsic::kImageSampleDrefImplicitLod;
    }
    if (str == "image_sample_explicit_lod") {
        return Intrinsic::kImageSampleExplicitLod;
    }
    if (str == "image_sample_implicit_lod") {
        return Intrinsic::kImageSampleImplicitLod;
    }
    if (str == "image_write") {
        return Intrinsic::kImageWrite;
    }
    return Intrinsic::kUndefined;
}

std::string_view ToString(Intrinsic value) {
    switch (value) {
        case Intrinsic::kUndefined:
            return "undefined";
        case Intrinsic::kImageQuerySize:
            return "image_query_size";
        case Intrinsic::kImageQuerySizeLod:
            return "image_query_size_lod";
        case Intrinsic::kImageSampleDrefExplicitLod:
            return "image_sample_dref_explicit_lod";
        case Intrinsic::kImageSampleDrefImplicitLod:
            return "image_sample_dref_implicit_lod";
        case Intrinsic::kImageSampleExplicitLod:
            return "image_sample_explicit_lod";
        case Intrinsic::kImageSampleImplicitLod:
            return "image_sample_implicit_lod";
        case Intrinsic::kImageWrite:
            return "image_write";
    }
    return "<unknown>";
}

}  // namespace tint::spirv::ir
