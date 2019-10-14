#include <stdexcept>

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


	enum class ERROR {
		ALL_OKEY,
		MISSING_EXPRESSION,
		MISSING_RIGHT_PARENTHESIS,
		EXCESS_RIGHT_PARENTHESIS,
		MISSING_ARGUMENT,
		UNKNOWN_OPERATOR,
		DIVIDED_BY_ZERO,
		OUT_OF_RANGE
	} error_;

	//Указатель на начало выражения для вывода отчета об ошибке 
	//Время жизни этой строки не должно быть меньше времени жизни объекта этого класса
	const char* exp_;

	int result_; //Результат рассчета выражения
	const char* ptr_; //Указатель на обрабатываемую часть выражения


	//Парсинг и расчет выражения:
	int getAtom ();
	int parseMulDiv ();
	int parseAddSub ();

  public:
	Calc ();

	bool calculate (const char* exp);
	int getResult ();
	void printErrorDescription ();
};
