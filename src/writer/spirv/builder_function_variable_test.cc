// Copyright 2020 The Tint Authors.
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

#include <memory>

#include "gtest/gtest.h"
#include "src/ast/binary_expression.h"
#include "src/ast/binding_decoration.h"
#include "src/ast/builtin.h"
#include "src/ast/builtin_decoration.h"
#include "src/ast/decorated_variable.h"
#include "src/ast/float_literal.h"
#include "src/ast/location_decoration.h"
#include "src/ast/scalar_constructor_expression.h"
#include "src/ast/set_decoration.h"
#include "src/ast/storage_class.h"
#include "src/ast/struct.h"
#include "src/ast/type/access_control_type.h"
#include "src/ast/type/f32_type.h"
#include "src/ast/type/struct_type.h"
#include "src/ast/type/vector_type.h"
#include "src/ast/type_constructor_expression.h"
#include "src/ast/variable.h"
#include "src/ast/variable_decoration.h"
#include "src/context.h"
#include "src/type_determiner.h"
#include "src/writer/spirv/builder.h"
#include "src/writer/spirv/spv_dump.h"
#include "src/writer/spirv/test_helper.h"

namespace tint {
namespace writer {
namespace spirv {
namespace {

using BuilderTest = TestHelper;

TEST_F(BuilderTest, FunctionVar_NoStorageClass) {
  ast::type::F32Type f32;
  ast::Variable v("var", ast::StorageClass::kNone, &f32);

  b.push_function(Function{});
  EXPECT_TRUE(b.GenerateFunctionVariable(&v)) << b.error();
  EXPECT_EQ(DumpInstructions(b.debug()), R"(OpName %1 "tint_766172"
)");
  EXPECT_EQ(DumpInstructions(b.types()), R"(%3 = OpTypeFloat 32
%2 = OpTypePointer Function %3
%4 = OpConstantNull %3
)");

  const auto& func = b.functions()[0];
  EXPECT_EQ(DumpInstructions(func.variables()),
            R"(%1 = OpVariable %2 Function %4
)");
}

TEST_F(BuilderTest, FunctionVar_WithConstantConstructor) {
  ast::type::F32Type f32;
  ast::type::VectorType vec(&f32, 3);

  ast::ExpressionList vals;
  vals.push_back(create<ast::ScalarConstructorExpression>(
      create<ast::FloatLiteral>(&f32, 1.0f)));
  vals.push_back(create<ast::ScalarConstructorExpression>(
      create<ast::FloatLiteral>(&f32, 1.0f)));
  vals.push_back(create<ast::ScalarConstructorExpression>(
      create<ast::FloatLiteral>(&f32, 3.0f)));

  auto init = create<ast::TypeConstructorExpression>(&vec, std::move(vals));

  EXPECT_TRUE(td.DetermineResultType(init.get())) << td.error();

  ast::Variable v("var", ast::StorageClass::kOutput, &f32);
  v.set_constructor(std::move(init));

  td.RegisterVariableForTesting(&v);

  b.push_function(Function{});
  EXPECT_TRUE(b.GenerateFunctionVariable(&v)) << b.error();
  ASSERT_FALSE(b.has_error()) << b.error();

  EXPECT_EQ(DumpInstructions(b.debug()), R"(OpName %6 "tint_766172"
)");
  EXPECT_EQ(DumpInstructions(b.types()), R"(%2 = OpTypeFloat 32
%1 = OpTypeVector %2 3
%3 = OpConstant %2 1
%4 = OpConstant %2 3
%5 = OpConstantComposite %1 %3 %3 %4
%7 = OpTypePointer Function %2
%8 = OpConstantNull %2
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].variables()),
            R"(%6 = OpVariable %7 Function %8
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].instructions()), R"(OpStore %6 %5
)");
}

TEST_F(BuilderTest, FunctionVar_WithNonConstantConstructor) {
  ast::type::F32Type f32;
  ast::type::VectorType vec(&f32, 2);

  auto rel =
      create<ast::BinaryExpression>(ast::BinaryOp::kAdd,
                                    create<ast::ScalarConstructorExpression>(
                                        create<ast::FloatLiteral>(&f32, 3.0f)),
                                    create<ast::ScalarConstructorExpression>(
                                        create<ast::FloatLiteral>(&f32, 3.0f)));

  ast::ExpressionList vals;
  vals.push_back(create<ast::ScalarConstructorExpression>(
      create<ast::FloatLiteral>(&f32, 1.0f)));
  vals.push_back(std::move(rel));

  auto init = create<ast::TypeConstructorExpression>(&vec, std::move(vals));

  EXPECT_TRUE(td.DetermineResultType(init.get())) << td.error();

  ast::Variable v("var", ast::StorageClass::kFunction, &vec);
  v.set_constructor(std::move(init));

  td.RegisterVariableForTesting(&v);
  b.push_function(Function{});
  EXPECT_TRUE(b.GenerateFunctionVariable(&v)) << b.error();
  ASSERT_FALSE(b.has_error()) << b.error();

  EXPECT_EQ(DumpInstructions(b.debug()), R"(OpName %7 "tint_766172"
)");
  EXPECT_EQ(DumpInstructions(b.types()), R"(%2 = OpTypeFloat 32
%1 = OpTypeVector %2 2
%3 = OpConstant %2 1
%4 = OpConstant %2 3
%8 = OpTypePointer Function %1
%9 = OpConstantNull %1
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].variables()),
            R"(%7 = OpVariable %8 Function %9
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].instructions()),
            R"(%5 = OpFAdd %2 %4 %4
%6 = OpCompositeConstruct %1 %3 %5
OpStore %7 %6
)");
}

TEST_F(BuilderTest, FunctionVar_WithNonConstantConstructorLoadedFromVar) {
  // var v : f32 = 1.0;
  // var v2 : f32 = v; // Should generate the load and store automatically.

  ast::type::F32Type f32;

  auto init = create<ast::ScalarConstructorExpression>(
      create<ast::FloatLiteral>(&f32, 1.0f));

  ASSERT_TRUE(td.DetermineResultType(init.get())) << td.error();

  ast::Variable v("v", ast::StorageClass::kFunction, &f32);
  v.set_constructor(std::move(init));
  td.RegisterVariableForTesting(&v);

  ast::Variable v2("v2", ast::StorageClass::kFunction, &f32);
  v2.set_constructor(create<ast::IdentifierExpression>("v"));
  td.RegisterVariableForTesting(&v2);

  ASSERT_TRUE(td.DetermineResultType(v2.constructor())) << td.error();

  b.push_function(Function{});
  EXPECT_TRUE(b.GenerateFunctionVariable(&v)) << b.error();
  EXPECT_TRUE(b.GenerateFunctionVariable(&v2)) << b.error();
  ASSERT_FALSE(b.has_error()) << b.error();

  EXPECT_EQ(DumpInstructions(b.debug()), R"(OpName %3 "tint_76"
OpName %7 "tint_7632"
)");
  EXPECT_EQ(DumpInstructions(b.types()), R"(%1 = OpTypeFloat 32
%2 = OpConstant %1 1
%4 = OpTypePointer Function %1
%5 = OpConstantNull %1
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].variables()),
            R"(%3 = OpVariable %4 Function %5
%7 = OpVariable %4 Function %5
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].instructions()),
            R"(OpStore %3 %2
%6 = OpLoad %1 %3
OpStore %7 %6
)");
}

TEST_F(BuilderTest, FunctionVar_ConstWithVarInitializer) {
  // var v : f32 = 1.0;
  // const v2 : f32 = v; // Should generate the load

  ast::type::F32Type f32;

  auto init = create<ast::ScalarConstructorExpression>(
      create<ast::FloatLiteral>(&f32, 1.0f));

  EXPECT_TRUE(td.DetermineResultType(init.get())) << td.error();

  ast::Variable v("v", ast::StorageClass::kFunction, &f32);
  v.set_constructor(std::move(init));
  td.RegisterVariableForTesting(&v);

  ast::Variable v2("v2", ast::StorageClass::kFunction, &f32);
  v2.set_is_const(true);
  v2.set_constructor(create<ast::IdentifierExpression>("v"));
  td.RegisterVariableForTesting(&v2);

  ASSERT_TRUE(td.DetermineResultType(v2.constructor())) << td.error();

  b.push_function(Function{});
  EXPECT_TRUE(b.GenerateFunctionVariable(&v)) << b.error();
  EXPECT_TRUE(b.GenerateFunctionVariable(&v2)) << b.error();
  ASSERT_FALSE(b.has_error()) << b.error();

  EXPECT_EQ(DumpInstructions(b.debug()), R"(OpName %3 "tint_76"
)");
  EXPECT_EQ(DumpInstructions(b.types()), R"(%1 = OpTypeFloat 32
%2 = OpConstant %1 1
%4 = OpTypePointer Function %1
%5 = OpConstantNull %1
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].variables()),
            R"(%3 = OpVariable %4 Function %5
)");
  EXPECT_EQ(DumpInstructions(b.functions()[0].instructions()),
            R"(OpStore %3 %2
%6 = OpLoad %1 %3
)");
}

TEST_F(BuilderTest, FunctionVar_Const) {
  ast::type::F32Type f32;
  ast::type::VectorType vec(&f32, 3);

  ast::ExpressionList vals;
  vals.push_back(create<ast::ScalarConstructorExpression>(
      create<ast::FloatLiteral>(&f32, 1.0f)));
  vals.push_back(create<ast::ScalarConstructorExpression>(
      create<ast::FloatLiteral>(&f32, 1.0f)));
  vals.push_back(create<ast::ScalarConstructorExpression>(
      create<ast::FloatLiteral>(&f32, 3.0f)));

  auto init = create<ast::TypeConstructorExpression>(&vec, std::move(vals));

  EXPECT_TRUE(td.DetermineResultType(init.get())) << td.error();

  ast::Variable v("var", ast::StorageClass::kOutput, &f32);
  v.set_constructor(std::move(init));
  v.set_is_const(true);

  td.RegisterVariableForTesting(&v);

  EXPECT_TRUE(b.GenerateFunctionVariable(&v)) << b.error();
  ASSERT_FALSE(b.has_error()) << b.error();

  EXPECT_EQ(DumpInstructions(b.types()), R"(%2 = OpTypeFloat 32
%1 = OpTypeVector %2 3
%3 = OpConstant %2 1
%4 = OpConstant %2 3
%5 = OpConstantComposite %1 %3 %3 %4
)");
}

}  // namespace
}  // namespace spirv
}  // namespace writer
}  // namespace tint
