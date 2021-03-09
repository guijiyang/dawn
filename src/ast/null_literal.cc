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

#include "src/ast/null_literal.h"

#include "src/program_builder.h"

TINT_INSTANTIATE_TYPEINFO(tint::ast::NullLiteral);

namespace tint {
namespace ast {

NullLiteral::NullLiteral(const Source& source, type::Type* type)
    : Base(source, type) {}

NullLiteral::~NullLiteral() = default;

std::string NullLiteral::to_str(const semantic::Info&) const {
  return "null " + type()->type_name();
}

std::string NullLiteral::name() const {
  return "__null" + type()->type_name();
}

NullLiteral* NullLiteral::Clone(CloneContext* ctx) const {
  // Clone arguments outside of create() call to have deterministic ordering
  auto src = ctx->Clone(source());
  auto* ty = ctx->Clone(type());
  return ctx->dst->create<NullLiteral>(src, ty);
}

}  // namespace ast
}  // namespace tint
