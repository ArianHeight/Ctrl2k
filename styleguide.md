# Style Guide and Conventions For Code

In this codebase, we have 2 different naming conventions depending on which level of code we are dealing with.
For anything in the library-level projects(AncestralHall, OracleBone, Monument, Petra), we use the [this convention](#library-level-naming-convention).
For code in any other project, we use the [standard convention](#standard-naming-convention).
Any terms and definitions that may appear in the codebase will be listed [here](#terms-and-definitions).

## Library Level Naming Convention
constants/constexpr variables:		SCREAMING_SNAKE_CASE
other variables:					snake_case
function names:						snake_case
class/struct/union/enum names:		snake_case
interface names:					interface_snake_case
abstract class/struct names:		abstract_snake_case
template type parameter names:		single capital letter, usually T, U, V, etc.
template paramter names:			_snake_case
enum values:						SCREAMING_SNAKE_CASE
file names:							snake_case(apart from files that match the project name)

## Standard Naming Convention
constants/constexpr variables:				SCREAMING_SNAKE_CASE
member variables:							m_camelCase
other variables:							camelCase
function names:								camelCase
class/struct/union/enum names:				PascalCase
interface class names:						IPascalCase
abstract class names:						APascalCase
template type parameter names:				single capital letter, usually T, U, V, etc.
template paramter names:					_underscore_snake_case
enum values:								SCREAMING_SNAKE_CASE
file names:									PascalCase

## Terms and Definitions
container: contains data in some form, in this codebase, this also means by default resizable in some way(unless it has the fixed_ prefix)
array: a container that holds data contiguously in memory
vector: indicates an array that can resize(but may be fixed in capacity), unfortunate C++ lingo, for our purposes, interchangeable with array
bitarray: a special array that only deals with bits(stored in bytes usually)
set: a container that only holds 1 of each element at most
map: a container that organizes data with key-value pairs where each key only has one value corresponding to it
multimap: a map where each key may have multiple values associated with it
kvp: short for key-value pair
stack_(container): a container that is created on the stack(or in the data section), and is therefore fixed in capacity
fixed_(container): a container that is fixed in size
