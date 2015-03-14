## General ##

  * Line length - 75 chars + max 2 symbols if needed
  * Indent length - 4 spaces (never use tabs)

## Assembly language ##

  * Always use underscore to separate words in names
  * Use underscore in front of a name for global values

## C++ ##

  * Namespace, class, struct, typedef names in `UpperCamelCase`
  * Mehod, function, variable names in `lowerCaseCase`
  * Constants, enums, macros in `UPPER_CASE`

  * _g_ prefix for global variables (e.g. `gGlobal`)
  * _m_ prefix for class/struct members (e.g. `mMember`)

  * Curly brackets `{` and `}` always in new line at the same level as owning expression:
  * Round brackets `(` and `)` always separated by space after keyword (like `if`, `for`, etc.) and always intact with function, method name. No spaces after opening and before closing bracket
  * Square brackets `[` and `]`  always intact with owning expression. No spaces after opening and before closing bracket
  * All arithmetic, logic, bitwise operators always separated by one space around them except for unary operators that must be intact with affected expression

E.g.
```
if (foo < SIZE)
{
    Vga::put(mFoo[foo + 1]);
}
else
{
    Vga::put(gFail);
}
```