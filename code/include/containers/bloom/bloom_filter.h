#pragma once

#include <cmath>
#include <functional>
#include <containers/bit/bits_field.h>
#include <containers/hash/murmur/murmur3.h>

namespace containers
{

	namespace detail
	{
		template <typename T> struct helper
		{
			static uint8_t* convert(T* value)
			{
				return reinterpret_cast<uint8_t*>(value);
			}

			static size_t len(T& value)
			{
				return sizeof(T);
			}
		};

		template <> struct helper<std::string>
		{
			static uint8_t* convert(std::string* value)
			{
				return reinterpret_cast<uint8_t*>(const_cast<char*>(value->c_str()));
			}

			static size_t len(std::string& value)
			{
				return value.length();
			}
		};

		template <> struct helper<const char*>
		{
			static uint8_t* convert(const char** value)
			{
				return reinterpret_cast<uint8_t*>(const_cast<char*>(*value));
			}

			static size_t len(const char*& value)
			{
				return strlen(value);
			}
		};
	}

	template <typename T> class bloom_filter // add hashing policy and make hashify template defined
	{
	private:
		bits_field<uint64_t> field;
		std::vector<std::function<uint64_t(uint64_t, uint64_t)>> hash_functions;
		uint64_t required_bits;
		uint64_t seed;
	public:
		bloom_filter(uint64_t capacity, double fp_rate, uint64_t s = 0)
		{
			double ln2 = std::log(2);
  			required_bits = (uint64_t) std::ceil(-(capacity * std::log(fp_rate) / ln2 / ln2));
  			double frac = static_cast<double>(required_bits) / static_cast<double>(capacity);
  			uint64_t k = (uint64_t) std::ceil(frac * ln2);
  			field = bits_field<uint64_t>(required_bits);
  			seed = s;
  			for (uint64_t i = 0; i < k; i++)
  			{
  				// https://www.eecs.harvard.edu/~michaelm/postscripts/rsa2008.pdf
  				hash_functions.push_back([=] (uint64_t h1, uint64_t h2) -> uint64_t { return (h1 + i * h2) % required_bits; });
  			}
		}


		void add(T key)
		{
			uint64_t tmp[2];
			hashify(key, tmp);
			for (auto hash : hash_functions)
			{
				field[hash(tmp[0], tmp[1])] = true;
			}
		}

		bool test(T key)
		{
			uint64_t tmp[2];
			hashify(key, tmp);
			for (auto hash : hash_functions)
			{
				if (!field[hash(tmp[0], tmp[1])])
					return false;
			}
			return true;
		}

	private:

		

		void hashify(T& key, uint64_t* out)
		{
			uint8_t* ptr = detail::helper<T>::convert(&key);
			MurmurHash3_x64_128(ptr, detail::helper<T>::len(key), seed, out);
		}
	};

}