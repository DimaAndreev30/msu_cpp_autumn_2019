#pragma once

#include <iostream>
#include <cstddef>


class BigInt
{
public:
	using BYTE = unsigned char;

private:
	// Большое число хранится как бы в 256-ричной системе счисления.
	// _mem - массив типа BYTE цифр абсолютного значения большого числа.
	// Массив не может заканчиваться нулями, если само число не равно нулю.
	// _sign - знак числа (true: < 0, false: >= 0)
	bool _sign;
	size_t _mem_size;
	BYTE* _mem;


	BigInt(BYTE* mem, size_t mem_size, bool sign);

	// Работают с операндами как с положительными числами,
	// не зависимо от значения их поля _sign
	void push_sum(const BigInt* a, const BigInt* b); // !
	void push_sub(const BigInt* a, const BigInt* b); // !

	size_t int_to_256(BYTE data[4], unsigned int value);

public:
	BigInt(int value); // !
	BigInt(unsigned int value); // !
	BigInt(const BigInt& value);
	BigInt(BigInt&& value);
	BigInt& operator=(const BigInt& value);
	BigInt& operator=(BigInt&& value);
	~BigInt();

	// Принимает строку, содержащюю шестнадцатиричную запись большого числа
	BigInt(const char* bit_str); // !

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
