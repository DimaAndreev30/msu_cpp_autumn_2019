#pragma once

#include <iostream>
#include <cstdint>
#include <cstddef>


template <class T = uint8_t, class L = uint16_t, size_t type_size = 8*size_of(T)>
class BigInt
{
	// -------------------------------------------------------------------------
	// Тип T должен представлять собой битовый массив длины type_size >= 8
	// По умолчанию инициализироваться нулем
	// И поддерживать операции:
	//     побитового сдвига и побитового отрицания
	//     сложения и сравнения
	//     конверции в uint8_t и наоборот
	// При отклонении поведения этих операций от стандартного
	// не гарантируется корректная работа класса, но это на вашей совести ;)
	// -------------------------------------------------------------------------
	// Аналогичными свойствами должен обладать тип L с тем лишь отличием,
	// что он должен допускать хранение по крайней мере type_size + 8 бит
	// Должна поддерживаться конверция между типами T и L
	// static_assert((~T{} < (~L{})))
	// -------------------------------------------------------------------------


	T _sign;
	size_t _mem_size;
	T* _mem;


	void push_sum(const BigInt* a, const BigInt* b); // !
	void push_sub(const BigInt* a, const BigInt* b); // !

	template <class S>
	void init_by(S value);
	void init_by_str_binary(const char* str, const uint8_t system);


public:
	BigInt(const BigInt& value);
	BigInt(BigInt&& value);
	BigInt& operator=(const BigInt& value);
	BigInt& operator=(BigInt&& value);
	~BigInt();

	BigInt(const T* data, size_t size, bool sign);

	template <class S>
	BigInt(S value);
	// Принимает строку, содержащюю шестнадцатиричную запись большого числа
	BigInt(const char* str, uint8_t system);


	bool operator==(const BigInt& value) const;
	bool operator!=(const BigInt& value) const;
	bool operator>=(const BigInt& value) const;
	bool operator<=(const BigInt& value) const;
	bool operator> (const BigInt& value) const;
	bool operator< (const BigInt& value) const;

	BigInt operator+(const BigInt& op) const;
	BigInt operator-(const BigInt& op) const;
	BigInt operator-() const;

	BigInt& operator+=(const BigInt& op);
	BigInt& operator-=(const BigInt& op);

	friend std::ostream& operator<<(std::ostream& out, const BigInt& value); // !
};
