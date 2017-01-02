

#ifndef _automsg_hpp_
#define _automsg_hpp_

// AUTO_STRUCT_FIELD(foo);
// AUTO_STRUCT_FIELD(bar);
#include "msgpck.h"
#include "autostructs.hpp"
#include <cstdint>

namespace autostruct {


  /* ========= Select Field ========== */

template <typename T> struct converter {
  template <typename U> T operator()(U &&x) { return T(x); }
};

template <typename R, typename Field, typename... Fields, typename Visitor,
          typename... AllFields>
R _select_field(Visitor &&v, char const *name,
                Struct<AllFields...> const &a_struct) {
  if (strcmp(name, Field::Q_name()) == 0)
    return v(a_struct.Field::Q_value());
  else
    return _select_field<R, Fields...>(static_cast<Visitor &&>(v), name,
                                       a_struct);
}

template <typename R, typename Visitor, typename... AllFields>
R _select_field(Visitor &&v, char const *name,
                Struct<AllFields...> const &a_struct) {
  // throw std::runtime_error("bad field name");
}

template <typename Visitor, typename Field, typename... AllFields>
auto select_field(Visitor &&v, char const *name,
                  Struct<Field, AllFields...> const &a_struct)
    -> decltype(v(a_struct.Field::Q_value())) {
  return _select_field<decltype(v(a_struct.Field::Q_value())), Field,
                       AllFields...>(static_cast<Visitor &&>(v), name,
                                     a_struct);
}

/* ========= Put Field ========== */

// template <typename T> struct setter {
//
//   T val;
//
//   setter(T v) { this->val = v; }
//
//   template <typename U> T operator()(U &&x) { x = val; }
// };


template <typename Field, typename... Fields, typename AV,
          typename... AllFields>
bool _put_field(AutoVisitor &&v, char const *name,
                Struct<AllFields...> &a_struct) {
  if (strcmp(name, Field::Q_name()) == 0) {
    std::cout << "put handler visitor!! found!!" << std::endl;
    return a_struct.Field::Q_visitor(v);
  }
  else {
    std::cout << "put handler visitor!! calling!!" << std::endl;
    return _put_field<AV, Fields...>(static_cast<AV &&>(v), name,
                                       a_struct);
                                     }
}

template <typename AV, typename... AllFields>
bool _put_field(AV &&v, char const *name,
                Struct<AllFields...> &a_struct) {
  // throw std::runtime_error("bad field name");
  std::cout << "put handler visitor!! error!! " << std::endl;
  return false;
}

template <typename AV, typename Field, typename... AllFields>
bool put_field(AV &&v, char const *name,
                  Struct<Field, AllFields...> &a_struct)
  {

  return _put_field<AV, Field,
                       AllFields...>(static_cast<AV &&>(v), name,
                                     a_struct);
}


/* ========= Examperimental ========== */

template <typename T> struct field_trait;

typedef class _Nill {
} mp_nil;
typedef class _ext {
} mp_ext;
typedef class _map {
} mp_map;
typedef class _arr {
} mp_arr;
typedef class _unknown {
} mp_unknown;

// template<> struct field_trait<empty> { static uint8_t value() { return
// msgpck_empty; } };
template <> struct field_trait<bool> {
  static uint8_t value() { return msgpck_bool; }
};

// template<> struct field_trait<uin8_t> { static uint8_t value() { return
// msgpck_uint; } };
// template<> struct field_trait<uin16_t> { static uint8_t value() { return
// msgpck_uint; } };
// template<> struct field_trait<uint32_t> { static uint8_t value() { return
// msgpck_uint; } };
template <> struct field_trait<unsigned int> {
  static uint8_t value() { return msgpck_uint; }
};
template <> struct field_trait<unsigned long> {
  static uint8_t value() { return msgpck_uint; }
};

// template<> struct field_trait<int8_t> { static uint8_t value() { return
// msgpck_sint; } };
// template<> struct field_trait<int16_t> { static uint8_t value() { return
// msgpck_sint; } };
// template<> struct field_trait<int32_t> { static uint8_t value() { return
// msgpck_sint; } };
template <> struct field_trait<int> {
  static uint8_t value() { return msgpck_sint; }
};
template <> struct field_trait<long> {
  static uint8_t value() { return msgpck_sint; }
};

template <> struct field_trait<float> {
  static uint8_t value() { return msgpck_float; }
};
template <> struct field_trait<char *> {
  static uint8_t value() { return msgpck_string; }
};
template <> struct field_trait<byte *> {
  static uint8_t value() { return msgpck_bin; }
};

template <> struct field_trait<mp_nil> {
  static uint8_t value() { return msgpck_nil; }
};

// TODO: Composite types?
// template<> struct field_trait<mp_ext> { static uint8_t value() { return
// msgpck_ext; } };
// template<> struct field_trait<mp_arr> { static uint8_t value() { return
// msgpck_array; } };
// template<> struct field_trait<mp_map> { static uint8_t value() { return
// msgpck_map; } };
// template<> struct field_trait<mp_unknown> { static uint8_t value() { return
// msgpck_unknown; } };

template <class AS>
bool automsg_deserialize_array(Stream *instream, AS *astruct) {

  if (astruct == NULL || instream == NULL)
    return false;

  bool result = msgpck_array_next(instream);

  if (!result)
    return false;

  uint32_t sz;
  result &= msgpck_read_array_size(instream, &sz);
  result &= sz == AS::size;

  if (!result)
    return false;

  // for (size_t idx = 0; idx < sz; ++idx) {
  //
  //
  //   float bar2 = autostruct::select_field(autostruct::converter<float>(), "bar", foobar);
  //
  //
  // }


  return true;

};

// template <class AS>
// bool automsg_serialize_array(Stream *outstream, AS *astruct) {
//
//   if (astruct == NULL || outstream == NULL)
//     return false;
//
//   uint32_t sz = AS::size;
//   msgpck_write_array_header(outstream, sz);
//
//   AS::template each_field<field_trait>([&outstream, &astruct](
//       char const *name, size_t offset, size_t size, int type) {
//     switch (type) {
//     case msgpck_nil:
//       msgpck_write_nil(outstream);
//       break;
//     case msgpck_bool:
//       msgpck_write_bool(outstream, *((bool *)astruct + offset));
//       break;
//     case msgpck_uint:
//       if (size == 1)
//         msgpck_write_integer(outstream, *((uint8_t *)astruct + offset));
//       if (size == 2)
//         msgpck_write_integer(outstream, *((uint16_t *)astruct + offset));
//       if (size == 4)
//         msgpck_write_integer(outstream, *((uint32_t *)astruct + offset));
//       break;
//     case msgpck_sint:
//       if (size == 1)
//         msgpck_write_integer(outstream, *((int8_t *)astruct + offset));
//       if (size == 2)
//         msgpck_write_integer(outstream, *((int16_t *)astruct + offset));
//       if (size == 4)
//         msgpck_write_integer(outstream, *((int32_t *)astruct + offset));
//       break;
//     case msgpck_float:
//       msgpck_write_float(outstream, *((float *)astruct + offset));
//       break;
//     case msgpck_string:
//       msgpck_write_string(outstream, (char *)(astruct + offset), size);
//       break;
//     case msgpck_bin:
//       msgpck_write_bin(outstream, (byte *)(astruct + offset), size);
//       break;
//       // case msgpck_ext:
//       //   msgpck_write_ext(outstream, (byte *) (astruct + offset), size);
//       //   break;
//       // case msgpck_array:
//       //     msgpck_read_array(instream, );
//       //   break;
//       // case msgpck_map:
//       //     msgpck_read_map(instream, );
//       //   break;
//     }
//     // Serial.print(":: field: ");
//     // Serial.print(" name: "); Serial.println(name);
//     // Serial.print(" offset: "); Serial.println(offset);
//     // Serial.print(" size: "); Serial.println(size);
//     // Serial.print(" type: "); Serial.println(type);
//     // Serial.println();
//
//   });
// }
};

#endif
