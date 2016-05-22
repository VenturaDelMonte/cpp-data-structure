
#include <cmath>
#include <containers/proxy/proxy.h>
#include <cstring>

namespace containers
{
	template <typename T = uint32_t> class bits_field
	{
		typedef typename std::vector<T>::size_type size_type;
		typedef bits_field<T> self_type;

	public:

		class bits_proxy : public containers::proxy<T>
		{
		private:
			T index;

			struct helper
			{
				static constexpr T one()
				{
					return 1l;
				}

				static constexpr T zero()
				{
					return 0l;
				}
			};



		public:
			inline bits_proxy(typename proxy<T>::reference_type& r, T idx) : proxy<T>::proxy(r), index(idx) {}

			//bits_proxy(typename proxy<T>::self_type* r = nullptr, T idx) : proxy<T>::proxy(r), index(idx) {}

			inline bits_proxy& set(bool val)
			{
				T mask = ~(helper::one() << index);
				*this->holder = (*this->holder & mask) | ((val ? helper::one() : helper::zero()) << index);
				return *this;
			}

			inline bool get()
			{
				return (*this->holder & (helper::one() << index)) != 0;
			}

			inline bool operator! ()
			{
				return !get();
			}

			inline operator bool()
			{
				return get();
			}

			inline explicit operator T()
			{
				return *this->holder;
			}

			inline bits_proxy& operator= (bool val) 
			{
				set(val);
				return *this;
			}	

			


		};

	private:
		std::vector<T> _field;
		static const T bits_per_unit = sizeof(T) * 8;
		uint32_t _capacity; // capacity meant in bit

	public:
		explicit bits_field(uint32_t capacity = sizeof(T))
		{
			T real_size = capacity / bits_per_unit + (capacity % bits_per_unit > 0);
			_field.resize(real_size, 0);
			_capacity = capacity;
		}

		bits_field(const self_type& oth)
		{
			(*this) = oth;
		}

		bits_field(const self_type&& oth)
		{
			(*this) = oth;
		}

		inline self_type& operator= (const self_type& oth)
		{
			reset();
			std::copy(oth._field.begin(), oth._field.end(), _field.begin());
			_capacity = oth._capacity;
			return *this;
		}

		inline self_type& operator= (const self_type&& oth)
		{
			reset();
			_field = std::move(oth._field);
			_capacity = std::move(oth._capacity);
			return *this;
		}

		inline void reset()
		{
			_field.resize(_capacity / sizeof(T) + (_capacity % sizeof(T) > 0), 0);
		}

		inline bits_proxy operator[] (T bit_index)
		{
			size_type pos = bit_index / bits_per_unit;
			T idx = bit_index % bits_per_unit - 1;
			T& elem = _field[pos];
			return bits_proxy(elem, idx);
		}

		std::string dump()
		{
			std::string str = "";
			for (T i = 0; i < _capacity; i++)
			{
				str += ((*this)[_capacity - i - 1] ? "1" : "0");
			}
			return str;
		}
	};
}