

#ifndef _automsg_hpp_
#define _automsg_hpp_

// AUTO_STRUCT_FIELD(foo);
// AUTO_STRUCT_FIELD(bar);
#include "msgpck.h"
#include <cstdint>

namespace autostruct {

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

template <class AS> bool automsg_deserialize_array(Stream *instream, AS *astruct) {

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

  AS::template each_field<field_trait>([&instream, &astruct, &result](
      char const *name, size_t offset, size_t size, int type) {
    switch (type) {
    case msgpck_nil:
      msgpck_read_nil(instream);
      break;
    case msgpck_bool:
      msgpck_read_bool(instream, astruct + offset);
      break;
    case msgpck_uint:
    case msgpck_sint:
      msgpck_read_integer(instream, astruct + offset, size);
      break;
    case msgpck_float:
      msgpck_read_float(instream, astruct + offset);
      break;
    case msgpck_string:
      msgpck_read_string(instream, astruct + offset, size);
      break;
    case msgpck_bin:
      msgpck_read_bin(instream, astruct + offset, size);
      break;
    case msgpck_ext:
      msgpck_read_ext(instream, astruct + offset, size);
      break;
    // case msgpck_array:
    //     msgpck_read_array(instream, );
    //   break;
    // case msgpck_map:
    //     msgpck_read_map(instream, );
    //   break;

    case msgpck_unknown:
      result = false;
      break;
    }
    // Serial.print(":: field: ");
    // Serial.print(" name: "); Serial.println(name);
    // Serial.print(" offset: "); Serial.println(offset);
    // Serial.print(" size: "); Serial.println(size);
    // Serial.print(" type: "); Serial.println(type);
    // Serial.println();

  });
}

template <class AS> bool automsg_serialize_array(Stream *outstream, AS *astruct) {

  if (astruct == NULL || outstream == NULL)
    return false;

  uint32_t sz = AS::size;
  msgpck_write_array_header(outstream, sz);

  AS::template each_field<field_trait>([&outstream, &astruct](
      char const *name, size_t offset, size_t size, int type) {
    switch (type) {
    case msgpck_nil:
      msgpck_write_nil(outstream);
      break;
    case msgpck_bool:
      msgpck_write_bool(outstream, *((bool*)astruct + offset));
      break;
    case msgpck_uint:
      if (size == 1) msgpck_write_integer(outstream,  *((uint8_t*)astruct+offset));
      if (size == 2) msgpck_write_integer(outstream, *((uint16_t*)astruct+offset));
      if (size == 4) msgpck_write_integer(outstream, *((uint32_t*)astruct+offset));
      break;
    case msgpck_sint:
      if (size == 1) msgpck_write_integer(outstream,  *((int8_t*)astruct+offset));
      if (size == 2) msgpck_write_integer(outstream, *((int16_t*)astruct+offset));
      if (size == 4) msgpck_write_integer(outstream, *((int32_t*)astruct+offset));
      break;
    case msgpck_float:
      msgpck_write_float(outstream, *((float*)astruct + offset));
      break;
    case msgpck_string:
      msgpck_write_string(outstream, (char *) (astruct + offset), size);
      break;
    case msgpck_bin:
      msgpck_write_bin(outstream, (byte *) (astruct + offset), size);
      break;
    // case msgpck_ext:
    //   msgpck_write_ext(outstream, (byte *) (astruct + offset), size);
    //   break;
    // case msgpck_array:
    //     msgpck_read_array(instream, );
    //   break;
    // case msgpck_map:
    //     msgpck_read_map(instream, );
    //   break;
    }
    // Serial.print(":: field: ");
    // Serial.print(" name: "); Serial.println(name);
    // Serial.print(" offset: "); Serial.println(offset);
    // Serial.print(" size: "); Serial.println(size);
    // Serial.print(" type: "); Serial.println(type);
    // Serial.println();

  });
}

};

#endif
