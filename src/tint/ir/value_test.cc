// Copyright 2022 The Tint Authors.
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

#include <sstream>

#include "src/tint/ir/test_helper.h"
#include "src/tint/ir/value.h"

namespace tint::ir {
namespace {

using namespace tint::number_suffixes;  // NOLINT

using IR_ValueTest = TestHelper;

TEST_F(IR_ValueTest, f32) {
    std::stringstream str;

    Value val(1.2_f);
    EXPECT_EQ(1.2_f, val.AsF32());

    str << val;
    EXPECT_EQ("1.200000", str.str());

    EXPECT_TRUE(val.IsF32());
    EXPECT_FALSE(val.IsF16());
    EXPECT_FALSE(val.IsI32());
    EXPECT_FALSE(val.IsU32());
    EXPECT_FALSE(val.IsTemp());
    EXPECT_FALSE(val.IsBool());
}

TEST_F(IR_ValueTest, f16) {
    std::stringstream str;

    Value val(1.1_h);
    EXPECT_EQ(1.1_h, val.AsF16());

    str << val;
    EXPECT_EQ("1.099609", str.str());

    EXPECT_FALSE(val.IsF32());
    EXPECT_TRUE(val.IsF16());
    EXPECT_FALSE(val.IsI32());
    EXPECT_FALSE(val.IsU32());
    EXPECT_FALSE(val.IsTemp());
    EXPECT_FALSE(val.IsBool());
}

TEST_F(IR_ValueTest, i32) {
    std::stringstream str;

    Value val(1_i);
    EXPECT_EQ(1_i, val.AsI32());

    str << val;
    EXPECT_EQ("1", str.str());

    EXPECT_FALSE(val.IsF32());
    EXPECT_FALSE(val.IsF16());
    EXPECT_TRUE(val.IsI32());
    EXPECT_FALSE(val.IsU32());
    EXPECT_FALSE(val.IsTemp());
    EXPECT_FALSE(val.IsBool());
}

TEST_F(IR_ValueTest, u32) {
    std::stringstream str;

    Value val(2_u);
    EXPECT_EQ(2_u, val.AsU32());

    str << val;
    EXPECT_EQ("2", str.str());

    EXPECT_FALSE(val.IsF32());
    EXPECT_FALSE(val.IsF16());
    EXPECT_FALSE(val.IsI32());
    EXPECT_TRUE(val.IsU32());
    EXPECT_FALSE(val.IsTemp());
    EXPECT_FALSE(val.IsBool());
}

TEST_F(IR_ValueTest, id) {
    std::stringstream str;

    Value val(Value::Id(4));
    EXPECT_EQ(4u, val.AsId());

    str << val;
    EXPECT_EQ("%4", str.str());

    EXPECT_FALSE(val.IsF32());
    EXPECT_FALSE(val.IsF16());
    EXPECT_FALSE(val.IsI32());
    EXPECT_FALSE(val.IsU32());
    EXPECT_TRUE(val.IsTemp());
    EXPECT_FALSE(val.IsBool());
}

TEST_F(IR_ValueTest, bool) {
    std::stringstream str;

    Value val(false);
    EXPECT_FALSE(val.AsBool());

    str << val;
    EXPECT_EQ("false", str.str());

    str.str("");
    val = Value(true);
    EXPECT_TRUE(val.AsBool());

    str << val;
    EXPECT_EQ("true", str.str());

    EXPECT_FALSE(val.IsF32());
    EXPECT_FALSE(val.IsF16());
    EXPECT_FALSE(val.IsI32());
    EXPECT_FALSE(val.IsU32());
    EXPECT_FALSE(val.IsTemp());
    EXPECT_TRUE(val.IsBool());
}

TEST_F(IR_ValueTest, uninitialized) {
    Value val;

    EXPECT_FALSE(val.IsF32());
    EXPECT_FALSE(val.IsF16());
    EXPECT_FALSE(val.IsI32());
    EXPECT_FALSE(val.IsU32());
    EXPECT_FALSE(val.IsTemp());
    EXPECT_FALSE(val.IsBool());
}

}  // namespace
}  // namespace tint::ir
