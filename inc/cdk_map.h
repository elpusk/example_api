#pragma once

/**
* dual key map.
*/
#include <memory>
#include <string>
#include <list>
#include <map>
#include <mutex>

template <class V>
class cdk_map
{
public:
	typedef	std::shared_ptr< cdk_map<V> > type_ptr;
	typedef	std::pair< std::wstring, long > type_key;

private:
	typedef std::map<long, V > _type_map;

public:
	cdk_map() : m_n_auto_long_key(-1)
	{
	}
	virtual ~cdk_map()
	{
	}
	bool empty()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_map_long_value.empty();
	}
	bool size()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_map_long_value.size();
	}

	size_t max_size() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_map_long_value.max_size();
	}

	bool insert(const type_key & key, const V& value)
	{
		bool b_result(false);
		do {
			//std::pair<std::map<K1, _type_ptr_value>::iterator, bool> re_k1 = m_map_k1.insert(K1, _type_ptr_value(new V(value)));
			std::lock_guard<std::mutex> lock(m_mutex);

			std::map<std::wstring, long>::iterator it_sl = m_map_str_long.find(key.first);
			if (it_sl != std::end(m_map_str_long))
				continue;

			std::map<long, std::wstring>::iterator it_ls = m_map_long_str.find(key.second);
			if (it_ls != std::end(m_map_long_str))
				continue;
			//
			m_map_long_value.insert(std::make_pair(key.second, value));
			//
			m_map_str_long.insert(std::make_pair(key.first, key.second));
			m_map_long_str.insert(std::make_pair(key.second, key.first));

			b_result = true;
		} while (false);
		return b_result;
	}
	long insert(const std::wstring& s_key, const V& value)
	{
		long n_long_key(-1);
		do {
			std::lock_guard<std::mutex> lock(m_mutex);

			std::map<std::wstring, long>::iterator it_sl = m_map_str_long.find(s_key);
			if (it_sl != std::end(m_map_str_long))
				continue;
			long n_key = _generate_long_key();
			//
			m_map_long_value.insert(std::make_pair(n_key, value));
			//
			m_map_str_long.insert(std::make_pair(s_key, n_key));
			m_map_long_str.insert(std::make_pair(n_key, s_key));

			n_long_key = n_key;
		} while (false);
		return n_long_key;
	}

	void erase(const std::wstring &s_key)
	{
		do {
			std::lock_guard<std::mutex> lock(m_mutex);
			std::map<std::wstring, long>::iterator it_sl = m_map_str_long.find(s_key);
			if (it_sl == std::end(m_map_str_long))
				continue;

			m_map_long_value.erase(it_sl->second);
			m_map_long_str.erase(it_sl->second);
			m_map_str_long.erase(it_sl);
		} while (false);
	}
	void erase( long n_key)
	{
		do {
			std::lock_guard<std::mutex> lock(m_mutex);
			std::map<long, std::wstring>::iterator it_ls = m_map_long_str.find(n_key);
			if (it_ls == std::end(m_map_long_str))
				continue;

			m_map_long_value.erase(n_key);
			m_map_long_str.erase(n_key);
			m_map_str_long.erase(it_ls->second);
		} while (false);
	}

	void clear()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_map_long_value.clear();
		m_map_long_str.clear();
		m_map_str_long.clear();
	}

	V  get(const type_key& key)
	{
		V value;

		do {
			std::lock_guard<std::mutex> lock(m_mutex);

			typename cdk_map::_type_map::iterator it = m_map_long_value.find(key.second);
			if (it == std::end(m_map_long_value))
				continue;
			//
			value = it->second;
		} while (false);

		return value;
	}
	V  get(long n_key)
	{
		V value;

		do {
			std::lock_guard<std::mutex> lock(m_mutex);

			typename std::map<long, V>::iterator it = m_map_long_value.find(n_key);
			if (it == std::end(m_map_long_value))
				continue;
			//
			value = it->second;
		} while (false);

		return value;
	}
	V  get(const std::wstring s_key)
	{
		V value;

		do {
			std::lock_guard<std::mutex> lock(m_mutex);

			std::map<std::wstring, long>::iterator it_sl = m_map_str_long.find(s_key);
			if (it_sl == std::end(m_map_str_long))
				continue;
			typename cdk_map::_type_map::iterator it = m_map_long_value.find(it_sl->second);
			if (it == std::end(m_map_long_value))
				continue;
			//
			value = it->second;
		} while (false);

		return value;
	}

protected:
	long _generate_long_key()
	{
		bool b_run(true);
		std::map<long, std::wstring>::iterator it_ls;

		do {
			++m_n_auto_long_key;

			if (m_n_auto_long_key < 0)
				m_n_auto_long_key = 0;
			//
			it_ls = m_map_long_str.find(m_n_auto_long_key);
			if (it_ls == std::end(m_map_long_str)) {
				b_run = false;
				continue;
			}
				
		} while (b_run);


		return m_n_auto_long_key;
	}

protected:
	std::mutex m_mutex;

	cdk_map::_type_map m_map_long_value;

	std::map<std::wstring, long> m_map_str_long;
	std::map<long, std::wstring> m_map_long_str;

	long m_n_auto_long_key;
};