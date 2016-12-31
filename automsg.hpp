

#ifndef _automsg_hpp_
#define _automsg_hpp_

// AUTO_STRUCT_FIELD(foo);
// AUTO_STRUCT_FIELD(bar);

enum MsgPackTypes : uint8_t {
  msgpck_empty = 0xff,
  msgpck_nil = 0xc0,
  msgpck_bool = 0xc2,
  msgpck_uint = 0xcc,
  msgpck_sint = 0xd0,
  msgpck_float = 0xca,
  msgpck_string = 0xd9,
  msgpck_bin = 0xc4,
  msgpck_ext = 0xc7,
  msgpck_array = 0xdc,
  msgpck_map = 0xde,
  msgpck_unknown = 0x00,
};

template<typename T> struct field_trait;
template<> struct field_trait<int> { static int value() { return 1; } };
template<> struct field_trait<float> { static int value() { return 2; } };
template<> struct field_trait<char> { static int value() { return 3; } };
template<> struct field_trait<double> { static int value() { return 3; } };


#endif
