#pragma once

#include <initializer_list>
#include <vector>
#include <exception>
#include <memory>
#include <containers/proxy/tensor_proxy.h>


namespace containers
{
	class index_out_of_bounds : public std::exception
	{
	private:
		size_t _index, _size;
	public:
		index_out_of_bounds(size_t index, size_t size) : _index(index), _size(size) { }
		virtual const char* what() const throw()
		{
			return "My exception happened";
		}
	};


	// https://en.wikipedia.org/wiki/Tensor
	template <typename T, class Allocator = std::allocator<T>> class _tensor
	{
	public:
		typedef _tensor<T> self_type;

		enum tensor_type
		{
			exclusive,
			shared
		};

		class tensor_proxy
		{

		public:

			tensor_proxy(self_type* holder = nullptr, size_t index = 0, size_t level = 0, tensor_proxy* parent = nullptr)
				: _holder(holder), _index(index), _level(level), _parent(parent)
			{
			}

			inline void operator= (T& val)
			{
				if (_level + 1 == _holder->_shape.size() && _index < _holder->_shape[_level])
				{
					T* buffer = unroll();
					*buffer = val;
				}
			}

			inline void operator= (T&& val)
			{
				if (_level + 1 == _holder->_shape.size() && _index < _holder->_shape[_level])
				{
					T* buffer = unroll();
					*buffer = std::move(val);
				}
			}

			inline operator T()
			{
				if (_level + 1 == _holder->_shape.size() && _index < _holder->_shape[_level])
				{
					T* buffer = unroll();
					return *buffer;
				}
				throw index_out_of_bounds(_index, _holder->_shape[_level]);
			}

			inline operator self_type()
			{
				T* buffer = unroll();
				std::vector<size_t> shape;
				for (size_t i = _level + 1; i < _holder->_shape.size(); i++)
				{
					shape.push_back(_holder->_shape[i]);
				}
				return self_type(buffer, shape, self_type::tensor_type::shared);
			}

			inline tensor_proxy operator[](size_t index)
			{
				if (index < _holder->_shape[_level])
				{
					return tensor_proxy(_holder, index, _level + 1, this);
				}
				throw index_out_of_bounds(index, _holder->_shape[_level]);
			}

		protected:
			inline T* unroll()
			{
				size_t offset = _index;
				tensor_proxy* curr = _parent;
				while (curr != nullptr)
				{
					size_t tmp = curr->_index;
					for (size_t i = curr->_level + 1; i < _holder->_shape.size(); i++)
					{
						tmp *= _holder->_shape[i];
					}
					offset += tmp;
					curr = curr->_parent;
				}
				return _holder->_data + offset;
			}

		private:
			self_type* _holder;
			size_t _index;
			size_t _level;
			tensor_proxy* _parent;
		};


	private:
		T* _data;
		std::vector<size_t> _shape;
		size_t _size;
		tensor_type _mode;
		Allocator allocator;
	public:
		_tensor() : _data(nullptr), _shape(), _size(0), _mode(exclusive)
		{

		}

		_tensor(std::initializer_list<size_t> shape)
		{
			_size = 1;
			for (auto dim : shape)
			{
				_size *= dim;
				_shape.push_back(dim);
			}
			_data = allocator.allocate(_size);
			_mode = tensor_type::exclusive;
		}

		virtual ~_tensor()
		{
			destroy();
		}

		_tensor(const self_type& oth)
		{
			(*this) = oth;
		}

		_tensor(const self_type&& oth)
		{
			(*this) = oth;
		}

		inline self_type& operator= (const self_type& oth)
		{
			destroy();

			return *this;
		}

		inline self_type& operator= (const self_type&& oth)
		{
			destroy();

			return *this;
		}

		inline tensor_proxy operator[] (size_t index)
		{
			if (index < _shape[0])
			{
				return tensor_proxy(this, index);
			}
			throw index_out_of_bounds(index, _shape[0]);
		}



	private:
		void destroy()
		{
			if (_data != nullptr && _size > 0)
			{
				allocator.deallocate(_data, _size);
			}
		}

	};


	typedef _tensor<int> itensor;
	typedef _tensor<float> ftensor;
	typedef _tensor<double> dtensor;
}
