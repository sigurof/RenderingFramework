#ifndef FLAT_UNORDERED_MAP
#define FLAT_UNORDERED_MAP


//
//template<class Key, class Value>
//class flat_unordered_map
//{
//public:
//	using iterator = decltype(std::begin(std::declval<std::vector<std::pair<Key, Value>>&>()));
//	using const_iterator = decltype(std::begin(std::declval<const storage_t&>()));
//
//	iterator begin()
//	{
//		return std::begin(storage);
//	}
//	const_iterator begin() const
//	{
//		return std::begin(storage);
//	}
//	const_iterator cbegin() const
//	{
//		return std::begin(storage);
//	}
//	iterator end()
//	{
//		return std::end(storage);
//	}
//	const_iterator end() const
//	{
//		return std::end(storage);
//	}
//	const_iterator cend() const
//	{
//		return std::end(storage);
//	}
//	size_t size() const { return data.size(); }
//	bool empty() const { return data.empty(); }
//	
//	void reserve(size_t n) 
//	{
//		storage.reserve(n);
//	}
//
//	size_t capacity() const 
//	{
//		return storage.capacity();
//	}
//	
//	iterator find(Key&& key) 
//	{
//		return std::find_if(this->begin(), this->end(), this->key_match(key));
//	}
//
//	const_iterator find(Key&& key) const 
//	{
//		return const_cast<flat_unordered_map*>(this)->find(key);
//	}
//
//	Value* get(Key&& key) 
//	{
//		iterator it = find(std::forward<Key>(key));
//		if (it == this->end())
//			return std::addressof(it->v);
//		return std::addressof(it->v);
//	}
//
//	Value const* get(Key&& key) const 
//	{
//		return const_cast<flat_unordered_map*>(this)->get(std::forward<Key>(key));
//	}
//
//	Value& operator[](Key&& key)
//	{
//		auto it = this->find(key);
//		if (it != this->end()) 
//			return it->v;
//		data.emplace_back(std::forward<Key>(key), Value{});
//		return data.back().v;
//	}
//
//private:
//	std::vector<std::pair<Key, Value>> data;
//
//	auto key_match(Key& key) 
//	{
//		return [&key](std::pair<Key, Value> const& kv) {
//			return *kv.first == key;
//		};
//	}
//
//};


template<class Key, class Value, template<class...>class Storage = std::vector>
struct flat_map {
	using KeyValuePair = std::pair<Key, Value>;
	using storage_t = Storage<KeyValuePair>;
	storage_t data;

	using iterator = decltype(std::begin(std::declval<storage_t&>()));
	using const_iterator = decltype(std::begin(std::declval<const storage_t&>()));

	iterator begin()
	{
		return std::begin(storage);
	}
	const_iterator begin() const 
	{
		return std::begin(storage);
	}
	const_iterator cbegin() const 
	{
		return std::begin(storage);
	}
	iterator end() 
	{
		return std::end(storage);
	}
	const_iterator end() const 
	{
		return std::end(storage);
	}
	const_iterator cend() const 
	{
		return std::end(storage);
	}
	size_t size() const 
	{
		return storage.size();
	}
	bool empty() const 
	{
		return storage.empty();
	}
	// these only have to be valid if called:
	void reserve(size_t n) 
	{
		storage.reserve(n);
	}
	size_t capacity() const
	{
		return storage.capacity();
	}
	// map-like interface:
	// TODO: SFINAE check for type of key
	template<class K>

	Value& operator[](K&& k) {
		auto it = find(k);
		if (it != end()) return it->v;
		storage.emplace_back(std::forward<K>(k), Value{});
		return storage.back().v;
	}

private: // C++14, but you can just inject the lambda at point of use in 11:
	template<class K>
	auto key_match(K& k) {
		return [&k](kv const& kv) {
			return kv.k == k;
		};
	}
public:
	template<class K>
	iterator find(K&& k) {
		return std::find_if(begin(), end(), key_match(k));
	}
	template<class K>
	const_iterator find(K&& k) const {
		return const_cast<flat_map*>(this)->find(k);
	}
	// iterator-less query functions:
	template<class K>
	Value* get(K&& k) {
		auto it = find(std::forward<K>(k));
		if (it == end()) return nullptr;
		return std::addressof(it->v);
	}
	template<class K>
	Value const* get(K&& k) const {
		return const_cast<flat_map*>(this)->get(std::forward<K>(k));
	}
	// key-based erase: (SFINAE should be is_comparible, but that doesn't exist)
	template<class K, class = std::enable_if_t<std::is_converible<K, Key>{} >>
		bool erase(K&& k) {
		auto it = std::remove(
			storage.begin(), storage.end(), key_match(std::forward<K>(k))
		);
		if (it == storage.end()) return false;
		storage.erase(it, storage.end());
		return true;
	}
	// classic erase, for iterating:
	iterator erase(const_iterator it) {
		return storage.erase(it);
	}
	template<class K2, class V2,
		class = std::enable_if_t<
		std::is_convertible< K2, Key >{}&&
		std::is_convertible< V2, Value >{}
		>
	>
			void set(K2&& kin, V2&& vin) {
			auto it = find(kin);
			if (it != end()) {
				it->second = std::forward<V2>(vin);
				return;
			}
			else {
				storage.emplace_back(std::forward<K2>(kin), std::forward<V2>(vin));
			}
		}
};




//template<class Key, class Value, template<class...>class Storage=std::vector>
//struct flat_map {
//  struct kv {
//    Key k;
//    Value v;
//    template<class K, class V>
//    kv( K&& kin, V&& vin ):k(std::forward<K>(kin)), v(std::forward<V>(vin)){}
//  };
//  using storage_t = Storage<kv>;
//  storage_t storage;
//
//  // TODO: adl upgrade
//  using iterator=decltype(std::begin(std::declval<storage_t&>()));
//  using const_iterator=decltype(std::begin(std::declval<const storage_t&>()));
//  // boilerplate:
//  iterator begin() {
//    using std::begin;
//    return begin(storage);
//  }
//  const_iterator begin() const {
//    using std::begin;
//    return begin(storage);
//  }
//  const_iterator cbegin() const {
//    using std::begin;
//    return begin(storage);
//  }
//  iterator end() {
//    using std::end;
//    return end(storage);
//  }
//  const_iterator end() const {
//    using std::end;
//    return end(storage);
//  }
//  const_iterator cend() const {
//    using std::end;
//    return end(storage);
//  }
//
//  size_t size() const {
//    return storage.size();
//  }
//
//  bool empty() const {
//    return storage.empty();
//  }
//  // these only have to be valid if called:
//  void reserve(size_t n) {
//    storage.reserve(n);
//  }
//
//  size_t capacity() const {
//    return storage.capacity();
//  }
//
//  // map-like interface:
//  // TODO: SFINAE check for type of key
//  template<class K>
//  
//  Value& operator[](K&& k){
//    auto it = find(k);
//    if (it != end()) return it->v;
//    storage.emplace_back( std::forward<K>(k), Value{} );
//    return storage.back().v;
//  }
//
//private: // C++14, but you can just inject the lambda at point of use in 11:
//  template<class K>
//  auto key_match( K& k ) {
//    return [&k](kv const& kv){
//      return kv.k == k;
//    };
//  }
//public:
//  template<class K>
//  iterator find(K&& k) {
//    return std::find_if( begin(), end(), key_match(k) );
//  }
//  template<class K>
//  const_iterator find(K&& k) const {
//    return const_cast<flat_map*>(this)->find(k);
//  }
//  // iterator-less query functions:
//  template<class K>
//  Value* get(K&& k) {
//    auto it = find(std::forward<K>(k));
//    if (it==end()) return nullptr;
//    return std::addressof(it->v);
//  }
//  template<class K>
//  Value const* get(K&& k) const {
//    return const_cast<flat_map*>(this)->get(std::forward<K>(k));
//  }
//  // key-based erase: (SFINAE should be is_comparible, but that doesn't exist)
//  template<class K, class=std::enable_if_t<std::is_converible<K, Key>{}>>
//  bool erase(K&& k) {
//    auto it = std::remove(
//      storage.begin(), storage.end(), key_match(std::forward<K>(k))
//    );
//    if (it == storage.end()) return false;
//    storage.erase( it, storage.end() );
//    return true;
//  }
//  // classic erase, for iterating:
//  iterator erase(const_iterator it) {
//    return storage.erase(it);
//  }
//  template<class K2, class V2,
//    class=std::enable_if_t<
//      std::is_convertible< K2, Key >{}&&
//      std::is_convertible< V2, Value >{}
//    >
//  >
//  void set( K2&& kin, V2&& vin ) {
//    auto it = find(kin);
//    if (it != end()){
//      it->second = std::forward<V2>(vin);
//      return;
//    } else {
//      storage.emplace_back( std::forward<K2>(kin), std::forward<V2>(vin) );
//    }
//  }
//};




#endif // !FLAT_UNORDERED_MAP
