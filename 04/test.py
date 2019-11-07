import subprocess

def check (expr, answer):

	out = subprocess.run (["./bigint", expr], stdout = subprocess.PIPE)

	result = out.stdout.decode ("ascii")

	if result == answer:
		print('pass')
	else:
		print(f'must be {answer} but got {result}')


if __name__ == '__main__':

	print('Преобразование и вывод больших чисел:')
	check('i0', '0')
	check('i1', '1')
	check('i-1', '-1')
	check('i123456789', '123456789')
	check('i-987654321', '-987654321')
	check('i2147483647', '2147483647')
	check('i-2147483648', '-2147483648')
	check('u3210123456', '3210123456')
	check('u4294967295', '4294967295')

	check('s-1', '-1')
	check('s0', '0')
	check('s1', '1')
	check('sFFFFFFFF', '4294967295')
	check('sF0F0F0F0', '4042322160')
	check('s-100000000', '-4294967296')
	check('s000000000000123456789ABCDEF0', '1311768467463790320')
