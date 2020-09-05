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

template<typename Key, typename HashKey>
struct SimpleHash{
  HashKey operator() (const Key& key) const{
    if constexpr (sizeof(HashKey) < sizeof(Key)){
      if constexpr (endian_order() == Endian::little_endian){
        constexpr auto div {sizeof(Key) / sizeof(HashKey)};
        constexpr auto rem {sizeof(Key) % sizeof(HashKey)};

        uint8_t* raw_adress {((uint8_t*) &key) + rem};

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

template<typename UnsignedIntegral>
constexpr auto pow2(UnsignedIntegral n){
  return 1 << n;
}

template<typename Key, typename Val,
         typename HashKey = uint8_t,
         template<typename K, typename HK>
         typename HashFunction = SimpleHash>
class HashTable{
  static constexpr const HashFunction<Key, HashKey> hash_function_ {};

  static constexpr const decltype(sizeof(HashKey)) bucket_number_ {pow2(sizeof(HashKey) * 8)};

  using Bucket      = LinkedList<Key, Val>;
  using BucketArray = std::array<Bucket, bucket_number_>;

  BucketArray bucket_;

  const Bucket& select_bucket_(const Key& key) const{
    return bucket_[hash_function_(key)];
  }
  Bucket& select_bucket_(const Key& key){
    return bucket_[hash_function_(key)];
  }
public:
  HashTable() : bucket_{}
  {}

  std::optional<Val> search(const Key& key) const{
    const Bucket& target_bucket {select_bucket_(key)};

    return target_bucket.search(key);
  }

  bool insert(const Key& key, const Val& val){
    Bucket& target_bucket {select_bucket_(key)};

    return target_bucket.push(key, val);
  }

  std::optional<Val> remove(const Key& key){
    Bucket& target_bucket {select_bucket_(key)};

    return target_bucket.remove(key);
  }
};

#endif
