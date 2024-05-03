// Petra.cpp : Defines the functions for the static library.
#include "rqm.h"
#include "Core/Monument/Monument.h"

compile_assert(sizeof(float) == sizeof(rqm::vec1));
compile_assert(sizeof(float) * 2 == sizeof(rqm::vec2));
compile_assert(sizeof(float) * 3 == sizeof(rqm::vec3));
compile_assert(sizeof(float) * 4 == sizeof(rqm::vec4));

compile_assert(sizeof(float) * 4 == sizeof(rqm::mat22));
compile_assert(sizeof(float) * 9 == sizeof(rqm::mat33));
compile_assert(sizeof(float) * 16 == sizeof(rqm::mat44));

compile_assert(sizeof(float) * 4 == sizeof(rqm::quat));