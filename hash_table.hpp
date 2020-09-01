#ifndef hash_table_hpp
#define hash_table_hpp

#include <array>
#include <cmath>
#include <cstdint>

#include <linked_list.hpp>

enum Endian : uint32_t {
  little_endian = 0x00000001,
  big_endian    = 0x01000000
};

constexpr Endian endian_order(){
  constexpr uint32_t all_set_mask {0xffffffff};
  
  if constexpr ((all_set_mask & 1 ) == Endian::little_endian){
    return Endian::little_endian;
  }
  else{
    return Endian::big_endian;
  }
}

template<typename HashKey, typename Key>
struct SimpleHash{
  HashKey operator() (const Key& key) const{
    if constexpr (sizeof(HashKey) < sizeof(Key)){
      if constexpr (endian_order() == Endian::little_endian){
        constexpr auto div {sizeof(Key) / sizeof(HashKey)};
        constexpr auto rem {sizeof(Key) % sizeof(HashKey)};

        char* raw_adress {((char*) &key) + rem};

        HashKey* shallow_view {raw_adress};

        return shallow_view[div - 1];
      }
      else{
        HashKey* shallow_view {&key};

        return shallow_view[0];
      }
    }
    else{
      return key;
    }
  }
};

template<typename Key, typename Val, typename HashKey = uint8_t,
         template<typename HK, typename K>
         typename HashFunction = SimpleHash>
class HashTable{
  static constexpr const HashFunction<HashKey, Key> hash_function_ {};

  static constexpr const decltype(sizeof(HashKey)) bucket_number_ {std::pow(2, sizeof(HashKey))};

  using Bucket = LinkedList<Key, Val>;
  using BucketArray = std::array<Bucket, bucket_number_>;
};

#endif
