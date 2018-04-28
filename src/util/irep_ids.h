/*******************************************************************\

Module: util

Author: Reuben Thomas, reuben.thomas@me.com

\*******************************************************************/

/// \file
/// util

#ifndef CPROVER_UTIL_IREP_IDS_H
#define CPROVER_UTIL_IREP_IDS_H

#ifndef USE_STD_STRING
#define USE_DSTRING
#endif

#ifdef USE_DSTRING
#include "dstring.h"
#else
#include <string>
#endif

/// \file
/// The irep_ids are generated using a technique called
/// [X-macros](https://en.wikipedia.org/wiki/X_Macro).
/// The ids are defined in the file irep_ids.def, using a pair of macros
/// `IREP_ID_ONE` and `IREP_ID_TWO`.
/// Definitions of the form `IREP_ID_ONE(param)` will be converted into a
/// const extern irep_idt with the variable name `ID_param` and the string
/// value `"param"`.
/// Definitions of the form `IREP_ID_TWO(param, contents)` will be converted
/// into a const extern irep_idt with the variable name `ID_param` and the
/// string value `"contents"`.

#ifdef USE_DSTRING

enum class idt:unsigned
{
#define IREP_ID_ONE(the_id) id_##the_id,
#define IREP_ID_TWO(the_id, str) id_##the_id,

#include "irep_ids.def" // NOLINT(build/include)
};

#ifdef __GNUC__
#define IREP_ID_ONE(the_id)                                         \
  constexpr dstringt ID_##the_id=dstringt::make_from_table_index(   \
      static_cast<unsigned>(idt::id_##the_id));
#define IREP_ID_TWO(the_id, str)                                    \
  constexpr dstringt ID_##the_id=dstringt::make_from_table_index(   \
      static_cast<unsigned>(idt::id_##the_id));
#else
#define IREP_ID_ONE(the_id)                                         \
  const dstringt ID_##the_id=dstringt::make_from_table_index(       \
      static_cast<unsigned>(idt::id_##the_id));
#define IREP_ID_TWO(the_id, str)                                    \
  const const dstringt ID_##the_id=dstringt::make_from_table_index( \
      static_cast<unsigned>(idt::id_##the_id));
#endif

template <>
struct diagnostics_helpert<irep_idt>
{
  static std::string diagnostics_as_string(const irep_idt &irep_id)
  {
    return as_string(irep_id);
  }
};

#else

#define IREP_ID_ONE(the_id) extern const std::string ID_##the_id;
#define IREP_ID_TWO(the_id, str) extern const std::string ID_##the_id;

#endif

#include "irep_ids.def"

#endif
