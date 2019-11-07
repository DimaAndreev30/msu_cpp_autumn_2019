#include <cstdlib>
#include <cstring>
#include <exception>
#include <algorithm>

#include "bigint.h"


// Небольшие обертки на стандарные функции. Безопасно пытаются выделить память.
// В случае чего, не поломав старую, выкидывают bad_alloc
template <class T>
T* mmalloc(size_t size)
{
	T* ptr = static_cast<T*>(malloc(sizeof(T)*size));
	if (ptr == nullptr)
		throw std::bad_alloc();
	return ptr;
}
template <class T>
void mrealloc(T*& ptr, size_t size)
{
	T* tmp = static_cast<T*>(realloc(ptr, sizeof(T)*size));
	if (tmp == nullptr)
		throw std::bad_alloc();
	ptr = tmp;
}



// Конструкторы класса
BigInt::BigInt(BYTE* mem, size_t mem_size=0, bool sign=false)
	: _sign(sign),
	  _mem_size(mem_size),
	  _mem(mem)
{}


size_t BigInt::int_to_256(BYTE data[4], unsigned int value)
{
	size_t size = 0;
	do {
		data[size] = value;
		size++;
		value >>= 8;
	} while(value);
	return size;
}

BigInt::BigInt(int v)
	: _sign(false),
	  _mem_size(0),
	  _mem(nullptr)
{
	BYTE b[4];

	size_t new_size;
	if (v < 0)
	{
		_sign = true;
		new_size = int_to_256(b, -v);
	}
	else new_size = int_to_256(b, v);

	_mem = mmalloc<BYTE>(new_size);
	_mem_size = new_size;
	std::memcpy(_mem, b, _mem_size);
}

BigInt::BigInt(unsigned int v=0)
	: _sign(false),
	  _mem_size(0),
	  _mem(nullptr)
{
	BYTE b[4];

	size_t new_size = int_to_256(b, v);

	_mem = mmalloc<BYTE>(new_size);
	_mem_size = new_size;
	std::memcpy(_mem, b, _mem_size);
}


BigInt::BigInt(const BigInt& value)
	: _sign(value._sign),
	  _mem_size(value._mem_size),
	  _mem(nullptr)
{
	_mem = mmalloc<BYTE>(_mem_size);
	std::memcpy(_mem, value._mem, _mem_size);
}

BigInt& BigInt::operator=(const BigInt& value)
{
	if (this != &value)
	{
		mrealloc<BYTE>(_mem, value._mem_size);
		_mem_size = value._mem_size;
		std::memcpy(_mem, value._mem, _mem_size);
		_sign = value._sign;
	}
	return *this;
}


BigInt::BigInt(BigInt&& value)
	: _sign(value._sign),
	  _mem_size(value._mem_size),
	  _mem(value._mem)
{
	value._sign = false;
	value._mem_size = 0;
	value._mem = nullptr;
}

BigInt& BigInt::operator=(BigInt&& value)
{
	if (this != &value)
	{
		free(_mem);

		_mem_size = value._mem_size;
		_mem = value._mem;
		_sign = value._sign;

		value._sign = false;
		value._mem_size = 0;
		value._mem = nullptr;
	}
	return *this;
}


BigInt::~BigInt()
{
	free(_mem);
}


BigInt::BigInt(const char* bit_str)
	: _sign(false),
	  _mem_size(0),
	  _mem(nullptr)
{
	if (*bit_str == '-')
	{
		bit_str++;
		_sign = true;
	}
	for (;*bit_str == '0'; bit_str++) {}

	size_t size = 0;
	for (;; size++)
	{
		if (bit_str[size] < '0'  || (bit_str[size] > '9' &&
		    bit_str[size] < 'A') ||  bit_str[size] > 'F') break;
	}

	if (size == 0)
	{
		_sign = false;
		_mem_size = 1;
		_mem = mmalloc<BYTE>(_mem_size);
		_mem[0] = 0;
	}
	else
	{
		size_t new_size = (size + 1)/2;
		_mem = mmalloc<BYTE>(new_size);
		_mem_size = new_size;

		// Небольшой фокус для того, что бы неполная пара в строке
		// обработалась так же, как и все остальные.
		bit_str += size & 1;
		for (ptrdiff_t i = -(size & 1), j = _mem_size - 1; j >= 0; j--)
		{
			BYTE byte = 0;
			do
			{
				byte <<= 4;
				if (bit_str[i] <= '9') byte += bit_str[i] - '0';
				else byte += bit_str[i] - 'A' + 10;
				i++;
			} while (i & 1);
			_mem[j] = byte;
		}
	}
}



// Операторы сравнения
bool BigInt::operator==(const BigInt& value) const
{
	return !(*this != value);
}

bool BigInt::operator!=(const BigInt& value) const
{
	if (this == &value) return false;
	if (_sign != value._sign) return true;

/*{
	// Код на случай, если допускается нулевые цифры в конце массива
	// Я же попытаюсь этого избежать, так что это не понадобится
	size_t min_size = _mem_size < value._mem_size ? _mem_size : value._mem_size;
	for (size_t i = min_size; i < _mem_size; i++)
	{
		if (_mem[i] != 0) return true;
	}
	for (size_t i = min_size; i < value._mem_size; i++)
	{
		if (value._mem[i] != 0) return true;
	}
}*/

	if (_mem_size != value._mem_size) return true;
	return std::memcmp(_mem, value._mem, _mem_size);
}

bool BigInt::operator>=(const BigInt& value) const
{
	if (this == &value) return true;
	if (_sign == value._sign)
	{
		/*
		// Код на случай, если допускается нулевые цифры в конце массива
		// Я же попытаюсь этого избежать, так что это не понадобится
		size_t min_size = _mem_size < value._mem_size ? _mem_size : value._mem_size;
		for (size_t i = min_size; i < _mem_size; i++)
		{
			if (_mem[i] != 0) return !_sign;
		}
		for (size_t i = min_size; i < value._mem_size; i++)
		{
			if (value._mem[i] != 0) return _sign;
		}
		*/

		if (_mem_size != value._mem_size)
			return (_mem_size > value._mem_size) ^ _sign;
		for (ptrdiff_t i = _mem_size - 1; i >= 0; i--)
		{
			if (_mem[i] != value._mem[i])
				return (_mem[i] > value._mem[i]) ^ _sign;
		}
		return true;
	}
	else return _sign < value._sign;
}

bool BigInt::operator<=(const BigInt& value) const
{
	return (value >= *this);
}

bool BigInt::operator> (const BigInt& value) const
{
	return !(value >= *this);
}

bool BigInt::operator< (const BigInt& value) const
{
	return !(*this >= value);
}



// Арифметические операторы
void BigInt::push_sum(const BigInt* a, const BigInt* b)
{
	if (a->_mem_size < b->_mem_size) std::swap(a, b);

	size_t new_size = a->_mem_size + 1;
	if ((a->_mem_size == b->_mem_size ? b->_mem[a->_mem_size - 1] : 0) +
		a->_mem[a->_mem_size - 1] < 255)
	{
			// Стараемся без причины не выделять лишний байт,
			// а только если есть подозрение на переполнения старшего байта
			new_size--;
	}
	mrealloc<BYTE>(_mem, new_size);

	unsigned short extra_bit = 0;
	size_t i = 0;
	for (; i < b->_mem_size; i++)
	{
		_mem[i] = extra_bit = a->_mem[i] + b->_mem[i] + extra_bit;
		extra_bit >>= 8;
	}
	for (; i < a->_mem_size; i++)
	{
		_mem[i] = extra_bit = a->_mem[i] + extra_bit;
		extra_bit >>= 8;
	}

	if (extra_bit) _mem[i] = 1;
	else if (new_size != i) mrealloc<BYTE>(_mem, --new_size);
	_mem_size = new_size;
}

void BigInt::push_sub(const BigInt* a, const BigInt* b)
{
	// Упорядочиваем аргументы:
	bool res_sign = a->_mem_size < b->_mem_size;
	if (res_sign) std::swap(a, b);
	else if (a->_mem_size == b->_mem_size)
	{
		// Такой случай приходится обрабатывать отдельно,
		// ведь нельзя сразу сказать, какой аргумент больше
		size_t n = 1;
		ptrdiff_t i;
		for (i = a->_mem_size - 1; i >= 0; i--)
		{
			if (a->_mem[i] != b->_mem[i])
			{
				res_sign = a->_mem[i] < b->_mem[i];
				if (res_sign) std::swap(a, b);
				n = i + 1;
				break;
			}
		}
		if (i < 0) _sign = false;

		mrealloc(_mem, n);
		_mem_size = n;

		short extra_bit = 0;
		n = 0;
		for (size_t i = 0; i < _mem_size; i++)
		{
			if (_mem[i] = extra_bit = a->_mem[i] - b->_mem[i] + extra_bit) n = i;
			extra_bit >>= 8;
		}

		if (_mem_size != ++n)
		{
			mrealloc<BYTE>(_mem, n);
			_mem_size = n;
		}

		_sign ^= res_sign;
		return;
	}


	mrealloc<BYTE>(_mem, a->_mem_size);

	short extra_bit = 0;
	size_t i = 0, n = 0;
	for (; i < b->_mem_size; i++)
	{
		if (_mem[i] = extra_bit = a->_mem[i] - b->_mem[i] + extra_bit) n = i;
		extra_bit >>= 8;
	}
	for (; i < a->_mem_size; i++)
	{
		if (_mem[i] = extra_bit = a->_mem[i] + extra_bit) n = i;
		extra_bit >>= 8;
	}
	
	if (a->_mem_size != ++n)
	{
		mrealloc<BYTE>(_mem, n);
		_mem_size = n;
	}
	else _mem_size = a->_mem_size;

	_sign ^= res_sign;
	return;
}


BigInt BigInt::operator+(const BigInt& op) const
{
	BigInt result (nullptr, 0, _sign);
	if (_sign == op._sign) result.push_sum(this, &op);
	else result.push_sub(this, &op);

	return result;
}

BigInt BigInt::operator-(const BigInt& op) const
{
	BigInt result (nullptr, 0, _sign);
	if (_sign != op._sign) result.push_sum(this, &op);
	else result.push_sub(this, &op);

	return result;
}

BigInt BigInt::operator-() const
{
	BigInt result = *this;
	result._sign = !result._sign;

	return result;
}


BigInt& BigInt::operator+=(const BigInt& op)
{
	if (_sign == op._sign) push_sum(this, &op);
	else push_sub(this, &op);

	return *this;
}

BigInt& BigInt::operator-=(const BigInt& op)
{
	if (_sign != op._sign) push_sum(this, &op);
	else push_sub(this, &op);

	return *this;
}



// Перевод большого числа в десятичную систему для последующего вывода на экран
// Функция добавляет очередной разряд большого числа в десятичное представление
// Каждый элемент массива bigint10 хранит 7 значащих цифр большого числа
void append_next_byte(unsigned int* bigint10, BigInt::BYTE byte, size_t& n)
{
	const int MAX_VALUE = 10000000;

	for (int i = 0; i < n; i++)
	{
		bigint10[i] = bigint10[i]*256 + byte;
		byte = bigint10[i]/MAX_VALUE;
		bigint10[i] %= MAX_VALUE;
	}
	if (byte != 0)
	{
		bigint10[n] = byte;
		n += 1;
	}
}

std::ostream& operator<<(std::ostream& out, const BigInt& value)
{
	if (value._mem_size == 0 ||
	    value._mem == nullptr)
	{
		out << "UNDEFINED!\n";
		return out;
	}
	// out << value._mem_size << ' ' << value._mem[0] << ' ' << value._sign << std::endl;

	// Достаточное число десятичных цифр для записи большого числа:
	size_t size = (value._mem_size*8 + 2)/3;
	// Достаточный размер массива для хранения записи большого числа:
	size = (size + 6)/7;
	// Число значимых элементов массива (ненулевых):
	size_t n = 0;


	unsigned int* bigint10 = mmalloc<unsigned int>(size);
	bigint10[0] = 0;

	for (int i = value._mem_size - 1; i >= 0; i--)
	{
		append_next_byte(bigint10, value._mem[i], n);
	}


	if (n)
	{
		if (value._sign) out << '-';
		out << bigint10[n - 1];
		for (ptrdiff_t i = n - 2; i >= 0; i--)
		{
			int k = 1;
			for (; k <= 1000000; k = k*10)
			{
				if (k > bigint10[i]) break;
			}
			for (; k <= 1000000; k = k*10)
				out << '0';
			out << bigint10[i];
		}
	}
	else out << '0';

	free(bigint10);

	return out;
}
