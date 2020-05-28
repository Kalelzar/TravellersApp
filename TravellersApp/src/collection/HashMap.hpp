#ifndef TRAVELLERAPP_HASHMAP_HPP
#define TRAVELLERAPP_HASHMAP_HPP

#include "ArrayList.hpp"
#include "../debug.hpp"
#include <cstdlib>
#include <type_traits>

/**
 * An Entry describes a key-value pair.
 * @tparam Key the key
 * @tparam Value the value
 */
template<class Key, class Value>
struct Entry {
    Key key;
    Value value;

    bool operator==(Entry<Key, Value> const &other) const {
        return key == other.key && value == other.value;
    }

    bool operator!=(Entry<Key, Value> const &other) const {
        return !(*this == other);
    }
};

/**
 * A generic implementation of a HashMap.
 * This particular implementation uses the 32-bit FNV-1a hash.
 *
 * Due to the way it is implemented there are several requirements for the
 * Key type.
 *
 * Firstly the Key type needs to overload the == and != operators.
 *
 * Secondly the Key type cannot be a pointer of any kind.
 *
 * Thirdly, if the Key is a class then it must not contain
 * any pointers as fields.
 * The third requirement can be circumvented by overloading the operator&
 * of the class so that it returns the stored pointer instead.
 * Doing this is NOT recommended and will probably break interoperability with
 * STL methods.
 *
 * @tparam Key the key
 * @tparam Value the value
 */
template<class Key, class Value>
class HashMap {
private:
    /** Shorthand for the Specialized Entry */
    typedef Entry<Key, Value>* SpecEntry;
    /** The default size of the HashMap */
    static const unsigned DEFAULT_SIZE = 16;
    /** The load factor threshold for expanding the underlying array */
    static constexpr double EXPAND_FACTOR = 0.8;
    SpecEntry* array;
    unsigned reserved{};
    unsigned elemCount{};

    /**
     * An implementation of the 32-bit FNV-1a hash.
     * This implementation is only available if T is not a pointer type
     * @tparam T the type of the object to be hashed
     * @param key the object to be hashed
     * @return the hash
     */
    template<typename T, typename std::enable_if_t<!std::is_pointer<T>::value> * = nullptr>
    uint32_t hash(const T *key) const {

        unsigned char *bytes = (unsigned char *) key;
        uint32_t hash = 2166136261u;
        uint32_t elems = sizeof(*key);
        for (int i = 0; i < elems; i++) {
            hash ^= bytes[i];
            hash *= 16777619;
        }

        return hash;
    };

    /**
     * An implementation of the 32-bit FNV-1a hash.
     * This implementation is only available if T is a pointer type
     * @tparam T the type of the object to be hashed
     * @param key the object to be hashed
     * @return the hash
     */
    template<typename T, typename std::enable_if_t<std::is_pointer<T>::value> * = nullptr>
    uint32_t hash(const T *key) const {

        unsigned char *bytes = (unsigned char *) *key;
        uint32_t hash = 2166136261u;
        uint32_t elems = strlen(*key); //This assumes T is char*.
        for (int i = 0; i < elems; i++) {
            hash ^= bytes[i];
            hash *= 16777619;
        }

        return hash;
    };

    /**
     * Returns the load factor of the HashMap.
     * The load factor is the ratio of the total allocated
     * capacity that is filled.
     * @return the load factor
     */
    double loadFactor() const {
        return length() / (double) capacity();
    }


    /**
     * Convenience method for copying another HashMap.
     * @param other the HashMap to copy
     */
    void copy(HashMap<Key, Value> const &other) {
        LOG(INFO, "Copy HashMap");
        array = new SpecEntry[other.capacity()];
        reserved = other.capacity();
        elemCount = 0;

        putAll(other);
    }

    /**
     * Finds hash associated with the given key.
     *
     * This is also the function that deals with hash collisions.
     * The way hash collision resolution is accomplished is simply
     * by hashing the key and incrementing it by one until a
     * free hash is reached or the hash where that key is stored is found.
     * @param k the key, whose hash to find
     * @returns the position of the key in the underlying array
     */
    unsigned findEntry(Key const &k) const {
        uint32_t hsh = hash(&k) % capacity();

        while (array[hsh] && array[hsh]->key != k) {
            hsh++;
            if (hsh >= capacity()) {
                hsh = 0;
            }
        }
        return hsh;
    }

    /**
     * Initializes the hash map with a provided size.
     * Assumes that the previous array, if such, has already been freed,
     * otherwise it leaks memory.
     * @param size the size to initialize the hash map with
     */
    void init(unsigned size) {
        LOG(INFO, "Initialize HashMap");
        array = new SpecEntry[size];
        reserved = size;
        elemCount = 0;
        for (int i = 0; i < size; i++) {
            array[i] = nullptr;
        }
    }

    /**
     * Expand the hash map by doubling the current capacity.
     * This operation requires that all elements are rehashed
     * since the hash relies on the capacity, as such it can
     * be a bit slow for a large number of elements
     */
    void expand() {
        LOG(INFO, "Expand HashMap");
        SpecEntry *const newarray = new SpecEntry[capacity() * 2];

        unsigned oldCapacity = capacity();

        for (int i = 0; i < capacity() * 2; i++) {
            newarray[i] = nullptr;
        }

        const SpecEntry *const oldarray = array;

        array = newarray;
        reserved = capacity() * 2;
        elemCount = 0;

        putAll(oldarray, oldCapacity);

        //delete[] newarray;
        delete[] oldarray;
    }

    /**
     * Puts all key-value pairs in the given list in this Hash Map.
     * @param other an array of Entries
     * @param the size of the array
     */
    void putAll(const SpecEntry *other, unsigned size) {
        for (unsigned i = 0; i < size; i++) {
            if (!other[i]) continue;//Skip if the entry is not initialized
            put(other[i]->key, other[i]->value);
        }
    }

    /**
     * Free all dynamically allocated memory associated with the Hash Map.
     */
    void free() {
        LOG(INFO, "Free HashMap");
        for(unsigned i = 0; i < capacity(); i++){
            if(array[i]) delete array[i];
        }
        delete[] array;
        reserved = 0;
        elemCount = 0;
    }

public:

    /**
     * Puts all elements contained in a Hash Map in this one.
     * @param other the HashMap whose element to add to this one
     */
    void putAll(HashMap<Key, Value> const &other) {
        putAll(other.array, other.capacity());
    }

    /**
     * Initialize the HashMap to {@link DEFAULT_SIZE}
     */
    HashMap() {
        init(DEFAULT_SIZE);
    }

    ~HashMap() {
        LOG(INFO, "Destructor called.");
        free();
    }

    HashMap(HashMap<Key, Value> const &other) {
        if (this != &other)
            copy(other);
    }

    HashMap<Key, Value> &operator=(HashMap<Key, Value> const &other) {
        if (this != &other) {
            free();
            copy(other);
        }
        return *this;
    }

    /**
     * The amount of elements in this HashMap.
     * @return the length
     */
    unsigned length() const {
        return elemCount;
    }

    /**
     * Returns the Value associated with the given Key if any.
     * This is done by returning a pointer to a {@link Nullable} instance.
     * It is an instance of {@link Null} if the key doesn't exist
     * or an instance of {@link NotNull} wrapped around the Value.
     * @param k the key, whose value to get
     * @returns the value, if any
     */
    std::unique_ptr<Nullable<Value>> get(Key const &k) {
        unsigned hsh = findEntry(k);

        if (!array[hsh]) {
            return std::make_unique<Null<Value>>();
        } else return std::make_unique<NotNull<Value>>(array[hsh]->value);
    }


    /**
     * Returns true if the key is contained in the HashMap.
     * @param k the key
     * @return is the key in the HashMap
     */
    bool contains(Key const &k) {
        unsigned hsh = findEntry(k);
        return array[hsh] ? true : false;
    }

    /**
     * The capacity of the HashMap
     * @return the capacity
     */
    unsigned capacity() const {
        return reserved;
    }

    /**
     * Put the given key-value pair in the HashMap.
     * If the key is already in the HashMap, its value will be updated.
     * @param k the key
     * @param v the value
     */
    void put(Key const &k, Value const &v) {
        if (loadFactor() >= EXPAND_FACTOR) {
            expand();
        }
        unsigned hsh = findEntry(k);
        if (!array[hsh]) {
            array[hsh] = new Entry<Key,Value>();
            array[hsh]->key = k;
            elemCount++;
        }
        array[hsh]->value = v;
    }

    /**
     * Removes the given key from the hash map by setting it's corresponding entry's
     * as uninitialized. This will probably not free any memory associated with the Key, Value pair
     * Such a thing should be handled by the caller.
     *
     * @param k the key to remove
     */
    void remove(Key const &k) {
        unsigned hsh = findEntry(k);
        if (!array[hsh]) return;
        delete array[hsh];
        array[hsh] = nullptr;
        elemCount--;
    }

    /**
     * Returns a constant pointer to the underlying entry list
     * for the purposes of iteration
     * @return the entries
     */
    const SpecEntry *underlying() const {
        return array;
    }

    /**
     * Returns all contained values in a list.
     * The order in which the values appear in the list is arbitrary.
     * @return a pointer to the list of values
     */
    std::unique_ptr<ArrayList<Value>> values() const {
        std::unique_ptr<ArrayList<Value>> list =
            std::make_unique<ArrayList<Value>>(length());
        for (int i = 0; i < capacity(); i++) {
            if (array[i]) {
                list->append(array[i]->value);
            }
        }

        return list;
    }

    /**
     * Returns all contained keys in a list.
     * The order in which the keys appear in the list is arbitrary.
     * @return a pointer to the list of keys
     */
    std::unique_ptr<ArrayList<Key>> keys() const {
        std::unique_ptr<ArrayList<Key>> list =
            std::make_unique<ArrayList<Key>>(length());
        for (int i = 0; i < capacity(); i++) {
            if (array[i]) {
                list->append(array[i]->key);
            }
        }

        return list;
    }

    /**
     * Clears the contents of the hash map.
     */
    void clear() {
        free();
        init(DEFAULT_SIZE);
    }
};


#endif //TRAVELLERAPP_HASHMAP_HPP
