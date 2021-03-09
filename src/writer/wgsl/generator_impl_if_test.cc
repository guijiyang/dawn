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

#include "src/writer/wgsl/test_helper.h"

namespace tint {
namespace writer {
namespace wgsl {
namespace {

using WgslGeneratorImplTest = TestHelper;

TEST_F(WgslGeneratorImplTest, Emit_If) {
  auto* cond = Expr("cond");
  auto* body = create<ast::BlockStatement>(ast::StatementList{
      create<ast::DiscardStatement>(),
  });
  auto* i = create<ast::IfStatement>(cond, body, ast::ElseStatementList{});

  GeneratorImpl& gen = Build();

  gen.increment_indent();

  ASSERT_TRUE(gen.EmitStatement(i)) << gen.error();
  EXPECT_EQ(gen.result(), R"(  if (cond) {
    discard;
  }
)");
}

TEST_F(WgslGeneratorImplTest, Emit_IfWithElseIf) {
  auto* else_body = create<ast::BlockStatement>(ast::StatementList{
      create<ast::DiscardStatement>(),
  });

  auto* body = create<ast::BlockStatement>(ast::StatementList{
      create<ast::DiscardStatement>(),
  });
  auto* i = create<ast::IfStatement>(
      Expr("cond"), body,
      ast::ElseStatementList{
          create<ast::ElseStatement>(Expr("else_cond"), else_body)});

  GeneratorImpl& gen = Build();

  gen.increment_indent();

  ASSERT_TRUE(gen.EmitStatement(i)) << gen.error();
  EXPECT_EQ(gen.result(), R"(  if (cond) {
    discard;
  } elseif (else_cond) {
    discard;
  }
)");
}

TEST_F(WgslGeneratorImplTest, Emit_IfWithElse) {
  auto* else_body = create<ast::BlockStatement>(ast::StatementList{
      create<ast::DiscardStatement>(),
  });

  auto* cond = Expr("cond");
  auto* body = create<ast::BlockStatement>(ast::StatementList{
      create<ast::DiscardStatement>(),
  });
  auto* i = create<ast::IfStatement>(
      cond, body,
      ast::ElseStatementList{create<ast::ElseStatement>(nullptr, else_body)});

  GeneratorImpl& gen = Build();

  gen.increment_indent();

  ASSERT_TRUE(gen.EmitStatement(i)) << gen.error();
  EXPECT_EQ(gen.result(), R"(  if (cond) {
    discard;
  } else {
    discard;
  }
)");
}

TEST_F(WgslGeneratorImplTest, Emit_IfWithMultiple) {
  auto* else_body = create<ast::BlockStatement>(ast::StatementList{
      create<ast::DiscardStatement>(),
  });

  auto* else_body_2 = create<ast::BlockStatement>(ast::StatementList{
      create<ast::DiscardStatement>(),
  });

  auto* body = create<ast::BlockStatement>(ast::StatementList{
      create<ast::DiscardStatement>(),
  });
  auto* i = create<ast::IfStatement>(
      Expr("cond"), body,
      ast::ElseStatementList{
          create<ast::ElseStatement>(Expr("else_cond"), else_body),
          create<ast::ElseStatement>(nullptr, else_body_2),
      });

  GeneratorImpl& gen = Build();

  gen.increment_indent();

  ASSERT_TRUE(gen.EmitStatement(i)) << gen.error();
  EXPECT_EQ(gen.result(), R"(  if (cond) {
    discard;
  } elseif (else_cond) {
    discard;
  } else {
    discard;
  }
)");
}

}  // namespace
}  // namespace wgsl
}  // namespace writer
}  // namespace tint
