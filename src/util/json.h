/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/


#ifndef CPROVER_UTIL_JSON_H
#define CPROVER_UTIL_JSON_H

#include <vector>
#include <map>
#include <iosfwd>
#include <string>

#include "irep.h"

class json_objectt;
class json_arrayt;

class jsont
{
public:
  enum class kindt
  {
    J_STRING,
    J_NUMBER,
    J_OBJECT,
    J_ARRAY,
    J_TRUE,
    J_FALSE,
    J_NULL
  };

  kindt kind;

  bool is_string() const
  {
    return kind==kindt::J_STRING;
  }

  bool is_number() const
  {
    return kind==kindt::J_NUMBER;
  }

  bool is_object() const
  {
    return kind==kindt::J_OBJECT;
  }

  bool is_array() const
  {
    return kind==kindt::J_ARRAY;
  }

  bool is_true() const
  {
    return kind==kindt::J_TRUE;
  }

  bool is_false() const
  {
    return kind==kindt::J_FALSE;
  }

  bool is_null() const
  {
    return kind==kindt::J_NULL;
  }

  jsont():kind(kindt::J_NULL)
  {
  }

  void output(std::ostream &out) const
  {
    output_rec(out, 0);
  }

  void swap(jsont &other);

  static jsont json_boolean(bool value)
  {
    return jsont(value?kindt::J_TRUE:kindt::J_FALSE);
  }

  void clear()
  {
    value.clear();
    kind=kindt::J_NULL;
    object.clear();
    array.clear();
  }

  json_arrayt &make_array();
  json_objectt &make_object();

  // this presumes that this is an object
  const jsont &operator[](const std::string &key) const
  {
    objectt::const_iterator it=object.find(key);
    if(it==object.end())
      return null_json_object;
    else
      return it->second;
  }

  void output_rec(std::ostream &, unsigned indent) const;

  static const jsont null_json_object;

protected:
  static void escape_string(const std::string &, std::ostream &);

  explicit jsont(kindt _kind):kind(_kind)
  {
  }

  jsont(kindt _kind, std::string _value) : kind(_kind), value(std::move(_value))
  {
  }

public:
  // should become protected
  typedef std::vector<jsont> arrayt;
  arrayt array;

  typedef std::map<std::string, jsont> objectt;
  objectt object;
  static void
  output_object(std::ostream &out, const objectt &object, unsigned indent);
  static void output_key(std::ostream &out, const std::string &key);

  std::string value;
};

inline std::ostream &operator<<(std::ostream &out, const jsont &src)
{
  src.output(out);
  return out;
}

class json_arrayt:public jsont
{
public:
  json_arrayt():jsont(kindt::J_ARRAY)
  {
  }

  void resize(std::size_t size)
  {
    array.resize(size);
  }

  std::size_t size() const
  {
    return array.size();
  }

  jsont &push_back(const jsont &json)
  {
    array.push_back(json);
    return array.back();
  }

  jsont &push_back()
  {
    array.push_back(jsont());
    return array.back();
  }

  template <typename... argumentst>
  void emplace_back(argumentst &&... arguments)
  {
    array.emplace_back(std::forward<argumentst>(arguments)...);
  }

  std::vector<jsont>::iterator begin()
  {
    return array.begin();
  }

  std::vector<jsont>::const_iterator begin() const
  {
    return array.begin();
  }

  std::vector<jsont>::const_iterator cbegin() const
  {
    return array.cbegin();
  }

  std::vector<jsont>::iterator end()
  {
    return array.end();
  }

  std::vector<jsont>::const_iterator end() const
  {
    return array.end();
  }

  std::vector<jsont>::const_iterator cend() const
  {
    return array.cend();
  }

  typedef jsont value_type; // NOLINT(readability/identifiers)
};

class json_stringt:public jsont
{
public:
  explicit json_stringt(std::string _value)
    : jsont(kindt::J_STRING, std::move(_value))
  {
  }

#ifndef USE_STD_STRING
  explicit json_stringt(const irep_idt &_value)
    : jsont(kindt::J_STRING, id2string(_value))
  {
  }
#endif

  /// Constructon from string literal.
  explicit json_stringt(const char *_value) : jsont(kindt::J_STRING, _value)
  {
  }
};

class json_numbert:public jsont
{
public:
  explicit json_numbert(const std::string &_value):
    jsont(kindt::J_NUMBER, _value)
  {
  }
};

class json_objectt:public jsont
{
public:
  json_objectt():jsont(kindt::J_OBJECT)
  {
  }

  jsont &operator[](const std::string &key)
  {
    return object[key];
  }

  const jsont &operator[](const std::string &key) const
  {
    objectt::const_iterator it=object.find(key);
    if(it==object.end())
      return null_json_object;
    else
      return it->second;
  }
};

class json_truet:public jsont
{
public:
  json_truet():jsont(kindt::J_TRUE) { }
};

class json_falset:public jsont
{
public:
  json_falset():jsont(kindt::J_FALSE) { }
};

class json_nullt:public jsont
{
public:
  json_nullt():jsont(kindt::J_NULL) { }
};

inline json_arrayt &jsont::make_array()
{
  kind=kindt::J_ARRAY;
  return static_cast<json_arrayt &>(*this);
}

inline json_objectt &jsont::make_object()
{
  kind=kindt::J_OBJECT;
  return static_cast<json_objectt &>(*this);
}

#endif // CPROVER_UTIL_JSON_H
