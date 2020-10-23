#ifndef hash_table_hpp
#define hash_table_hpp
// we are going to use an array to hold the buckets of our hash table
#include <array>
// we are going to use uint32_t to detect endian order
#include <cstdint>
// our buckets will be represented by linked lists
#include <linked_list.hpp>
// this enum is used to represent the endianness in use
enum class Endian {little_endian, big_endian};
// detects the endian order used, returning the appropriate Endian
// member
Endian endian_order(){
  uint32_t one_32 {1};
  uint8_t first_byte {*(uint8_t*) &one_32};

  if (first_byte == 1){
    return Endian::little_endian;
  }
  else{
    return Endian::big_endian;
  }
}
// this struct serves as a type for a simple, prefix based hash
// function. Notice that the notion of prefix is affected by the
// endian order in use
template<typename Key, typename HashKey>
struct SimpleHash{
  HashKey operator() (const Key& key) const{
    // if HashKey has a size smaller than the size of Key
    if constexpr (sizeof(HashKey) < sizeof(Key)){
      // if we are using little endian order
      if (endian_order() == Endian::little_endian){
        // sizeof(Key) == div * sizeof(HashKey) + rem
        constexpr auto div {sizeof(Key) / sizeof(HashKey)};
        constexpr auto rem {sizeof(Key) % sizeof(HashKey)};
        // we take key address skipping the first rem bytes
        uint8_t* raw_adress {((uint8_t*) &key) + rem};
        // we take raw_adress as a HashKey address
        HashKey* shallow_view {(HashKey*) raw_adress};
        // then we return the last "occurrence" of a HashKey inside
        // key
        return shallow_view[div - 1];
      }
      // here we work with big endian order, so we just need to take a
      // few of the first bytes of key
      else{
        // we take key address as a HashKey address
        HashKey* shallow_view {(HashKey*) &key};
        // then we return the first "occurrence" of a HashKey inside
        // key
        return shallow_view[0];
      }
    }
    // in case HashKey has at least the size of Key, SimpleHash
    // behaves like an identity function
    else{
      return key;
    }
  }
};
// returns 2 raised to n. Notice that this should be used with integer
// types, and n should be nonnegative
template<typename Integral>
constexpr auto pow2(Integral n){
  // simple left shift to calculate 2^n
  return 1 << n;
}
// a hash table implementation: maps Key to Val, using HashKey
// (defaults to uint8_t) as an internal hash index type and
// HashFunction (defaults to SimpleHash) to convert from Key to
// HashKey
template<typename Key, typename Val,
         typename HashKey = uint8_t,
         template<typename K, typename HK>
         typename HashFunction = SimpleHash>
class HashTable{
  // hash function object to perform Key -> HashKey transformation
  static constexpr const HashFunction<Key, HashKey> hash_function_ {};
  // number of buckets this hash table is going to use: if HashKey has
  // a n bytes representation, it has 2^(8n) possible values
  static constexpr const decltype(sizeof(HashKey)) bucket_number_ {pow2(sizeof(HashKey) * 8)};
  // we use a linked list as a bucket
  using Bucket      = LinkedList<Key, Val>;
  // BucketArray is an array of Buckets with one position for each
  // possible value of HashKey
  using BucketArray = std::array<Bucket, bucket_number_>;
  // our array of buckets
  BucketArray bucket_;
  // selects the bucket that might contain key
  const Bucket& select_bucket_(const Key& key) const{
    return bucket_[hash_function_(key)];
  }
  Bucket& select_bucket_(const Key& key){
    return bucket_[hash_function_(key)];
  }
public:
  // simple constructor
  HashTable() : bucket_{}
  {}
  // searches for key and returns its associated value. Returns
  // nothing in case key is not present
  std::optional<Val> search(const Key& key) const{
    // selects the bucket in which the search will be performed
    const Bucket& target_bucket {select_bucket_(key)};
    // then does the search
    return target_bucket.search(key);
  }
  // inserts key, associating it with value. In case key is already
  // present, does nothing and signals that to the caller
  bool insert(const Key& key, const Val& val){
    Bucket& target_bucket {select_bucket_(key)};

    return target_bucket.push(key, val);
  }
  // removes key, returning its associated value. In case key is not
  // present, returns nothing
  std::optional<Val> remove(const Key& key){
    Bucket& target_bucket {select_bucket_(key)};

    return target_bucket.remove(key);
  }
};

#endif
