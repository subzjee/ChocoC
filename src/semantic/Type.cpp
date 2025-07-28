#include "semantic/Type.h"

namespace chocopy::sema {
[[nodiscard]] bool Type::isSubclass(const Type& type) const {
  if (*this == *Type::getObjectType()) {
    return false;
  }

  return *super_class == type;
}

[[nodiscard]] bool Type::conformsTo(const Type& type) const {
  return (*this == type) || isSubclass(type);
}

[[nodiscard]] bool
Type::isAssignmentCompatible(const Type& type) const {
  // 1. Ordinary subtyping.
  if (conformsTo(type)) {
    return true;
  }

  // 2. Can not assign None to a literal.
  if ((isNone() && !type.isInteger() && !type.isBoolean() &&
       !type.isString())) {
    return true;
  }

  // 3. Can assign empty list to any list type.
  if ((isEmpty() && type.isList())) {
    return true;
  }

  // 4. Allow assigning [None] to variables of type [T] where T is
  // assignment-compatible with <None>.
  if ((dimension == 1 && isNone() && type.dimension == 1 &&
       type.isAssignmentCompatible(*Type::getNoneType()))) {
    return true;
  }

  return false;
}
}