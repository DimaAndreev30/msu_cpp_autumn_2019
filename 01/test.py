import subprocess

def check (expr, answer, ret=0):

	out = subprocess.run (["./calc", expr], stdout = subprocess.PIPE)

	result = out.stdout.decode ("ascii").split ('\n')[0]
	returncode = out.returncode;

	if ret == 0:
		if returncode != 0:
			print ("error in {0}: parse failed with {1}".format (expr, out.stdout.decode ("ascii")))
		elif result != answer:
			print ("error in {0}: must be {1}, got {2}".format (expr, answer, result))
		else:
			print ("pass: {0}".format (expr))
	else:
		if returncode == 0:
			print ("error in {0}: parse succeeded when {1}".format (expr, answer))
		elif result != answer:
			print ("error in {0}: must be {1}, got {2}".format (expr, answer, result))
		else:
			print ("pass: {0}".format (expr))
	

if __name__ == '__main__':

#Типы синтаксических ошибок:
#	'Divided by zero'
#	'Missing right parenthesis'
#	'Excess right parenthesis'
#	'Missing argument'
#	'Unknown operator'

	print ("Operations and space-symbols:")
	check ("1", "1")
	check ("2 + 2", "4")
	check ("     (     -2   *-9 - 17)*4", "4")
	check ("(1 + 6) /  -7*4  /-   1", "4")
	check ("(100500 + -100400)/100 * (100*500/10)", "5000")
	check ("  - -- -  - -- ( -   ----- -- --(-1))", "1")

	print ("\nParenthesis number:")
	check ("(1 + (2 + (3 + 4)", "Missing right parenthesis", 1)
	check ("(1 + (2 + (3 + 4))))", "Excess right parenthesis", 1)
	check ("(1 + (2 + (3 + 4)))", "10")
	check ("(((1 + 9) - 10) * (1 + 3) + 4) + (1 - 1)", "4")

	print ("\nSyntax error:")
	check ("1 + a", "Missing argument", 1)
	check ("(1 + 2 + (4 - 2)*1)/a1", "Missing argument", 1)
	check ("1 + 2 + ", "Missing argument", 1)
	check ("1 ^ 2", "Unknown operator", 1)

	print ("\nDivided by zero:")
	check ("1/0", "Divided by zero", 1)
	check ("1/(100 - 50*2)", "Divided by zero", 1)
	
