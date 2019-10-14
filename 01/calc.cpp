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



Calc::Calc ():
	error_ (ERROR::MISSING_EXPRESSION),
	result_ (-1), 
	exp_ (nullptr), ptr_ (nullptr)
{}


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
		if (*ptr_ != ')')
		{
			error_ = ERROR::MISSING_RIGHT_PARENTHESIS;
			throw std::exception ();
		}
		++ptr_;

		return sgn ? result : -result;
	}
	else
	{	
		error_ = ERROR::MISSING_ARGUMENT;
		throw std::exception ();
	}
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
			++ptr_;
			int divider = getAtom ();
			if (divider == 0)
			{
				error_ = ERROR::DIVIDED_BY_ZERO;
				throw std::exception ();
			}

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


bool Calc::calculate (const char* exp)
{
	if (exp == nullptr)
	{
		error_ = ERROR::MISSING_EXPRESSION;
		return false;
	}

	exp_ = exp;
	ptr_ = exp_;

	try {
		result_ = parseAddSub ();
	}
	catch (std::exception err) {
		return false;
	}

	if (*ptr_ != '\0')
	{
		if (*ptr_ == ')')
		{
			error_ = ERROR::EXCESS_RIGHT_PARENTHESIS;
		}
		else
		{
			error_ = ERROR::UNKNOWN_OPERATOR;
		}

		return false;
	}

	error_ = ERROR::ALL_OKEY;
	return true;
}

int Calc::getResult ()
{
	return result_;
}

void Calc::printErrorDescription ()
{
	if (error_ == ERROR::ALL_OKEY) std::cout << "Expression is correct!\n";
	else if (error_ == ERROR::MISSING_EXPRESSION) std::cout << "Missing expression\n";
	else 
	{
		switch (error_)
		{
		  case ERROR::DIVIDED_BY_ZERO: 
			std::cout << "Divided by zero\n";
			break;
		  case ERROR::OUT_OF_RANGE: 
			std::cout << "The value of the expression does not fit in INT\n";
			break;
		  case ERROR::MISSING_RIGHT_PARENTHESIS: 
			std::cout << "Missing right parenthesis\n";
			break;
		  case ERROR::EXCESS_RIGHT_PARENTHESIS:
			std::cout << "Excess right parenthesis\n";
			break;
		  case ERROR::MISSING_ARGUMENT: 
			std::cout << "Missing argument\n";
			break;
		  case ERROR::UNKNOWN_OPERATOR:
			std::cout << "Unknown operator\n";
		}

		std::cout << exp_ << '\n';
		for (int i = 0; i < ptr_ - exp_; i++)
			std::cout << ' ';
		std::cout << '^' << '\n';	
	}
}

