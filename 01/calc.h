#include <stdexcept>

class CalcException : public std::exception
{
  public:
	//Используются в качестве индекса для массива строк-описаний ошибок
	//Не менять порядок! Добавлять новые только в конец!
	enum ERROR {
		MISSING_RIGHT_PAR = 0,
		EXCESS_RIGHT_PAR,
		MISSING_ARGUMENT,
		UNKNOWN_OPERATOR,
		DIVIDED_BY_ZERO
	};


  private:
	int pos_; //Позиция в выражении, вызвавшая исключение
	ERROR code_; //Код ошибки

  public:
	CalcException (ERROR code, int pos):
		pos_ (pos), code_ (code)
	{}

	const char* what () const noexcept 
	{
		static const char* exceptionDesc [] = {
			"Missing right parenthesis",
			"Excess right parenthesis",
			"Missing argument",
			"Unknown operator",
			"Divided by zero"
			};

		return exceptionDesc [code_];
	}
	int where () const noexcept { return pos_; }
};


class Calc
{
	enum class OP_CODE {
		NON,
		ADD,
		SUB,
		MUL,
		DIV,
		NEG
	};

	const char* exp_; //Указатель на начало выражения
	const char* ptr_; //Указатель на обрабатываемую часть выражения


	//Парсинг и расчет выражения:
	int getAtom ();
	int parseMulDiv ();
	int parseAddSub ();

  public:
	int calculate (const char* exp);
};
