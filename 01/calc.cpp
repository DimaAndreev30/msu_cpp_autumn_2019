#include <iostream>
#include <cctype>
#include "calc.h"


//Пара вспомогательных функций:

//Пропускает пробельные символы
void skipSpace (const char*& exp)
{
	while (isspace (*exp)) exp++;
}

//Считывает целое число типа Int (в предположении, что оно там есть)
int getInt (const char*& exp)
{
	int result = 0;
	
	do {
		result = 10*result + (*exp - '0');
		++exp;
	} while (isdigit (*exp));

	return result;
}


//Пропускает все пробельные символы в начале
//Считывает аргумент оператора как число типа Int или блок, записанный внутри скобок ()
//Так же считывает унарные операции, как самые приоритетные
int Calc::getAtom ()
{
	bool sgn = 1;
	skipSpace (ptr_);

	while (*ptr_ == '-')
	{
		++ptr_;
		sgn ^= 1;
		skipSpace (ptr_);
	}

	if (isdigit (*ptr_)) 
	{
		return sgn ? getInt (ptr_) : -getInt (ptr_);
	}
	else if (*ptr_ == '(') 
	{
		++ptr_;
		int result = parseAddSub ();

		skipSpace (ptr_);
		if (*ptr_ != ')') throw CalcException (CalcException::MISSING_RIGHT_PAR, ptr_ - exp_);
		++ptr_;

		return sgn ? result : -result;
	}
	else throw CalcException (CalcException::MISSING_ARGUMENT, ptr_ - exp_ - 1);
}

//Пропускает все пробельные символы в начале
//Считывает ряд операций первого уровня приоритетности (*, /)
//Аргумент для них это getAtom
int Calc::parseMulDiv ()
{
	int result = getAtom ();

	while (true)
	{
		skipSpace (ptr_);

		switch (*ptr_)
		{
		  case '*':
		  {
			++ptr_;
			result *= getAtom ();
			break;
		  }
		  case '/':
		  {
			int pos = ptr_ - exp_;

			++ptr_;
			int divider = getAtom ();

			if (divider == 0) throw CalcException (CalcException::DIVIDED_BY_ZERO, pos);

			result /= divider;
			break;
		  }
		  default: 
			return result;
		}
	}
}

//Пропускает все пробельные символы в начале
//Считывает ряд операций второго уровня приоритетности (+, -)
//Аргумент для них это getDivMul
int Calc::parseAddSub ()
{
	int result = parseMulDiv ();

	while (true)
	{
		skipSpace (ptr_);

		switch (*ptr_)
		{
		  case '+':
		  {
			++ptr_;
			result += parseMulDiv ();
			break;
		  }
		  case '-':
		  {
			++ptr_;
			result -= parseMulDiv ();
			break;
		  }
		  default: 
			return result;
		}
	}
}


int Calc::calculate (const char* exp)
{
	if (exp == nullptr) throw std::invalid_argument ("Missing expression");

	
	exp_ = exp;
	ptr_ = exp_;

	int result = parseAddSub ();

	if (*ptr_ != '\0')
	{
		if (*ptr_ == ')') 
			throw CalcException (CalcException::EXCESS_RIGHT_PAR, ptr_ - exp_);

		else    throw CalcException (CalcException::UNKNOWN_OPERATOR, ptr_ - exp_);
	}

	return result;
}

