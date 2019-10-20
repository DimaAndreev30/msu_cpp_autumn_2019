import subprocess
import numpy as np

def check (args, res):

	out = subprocess.run (["./alloc", *(args.split())], stdout = subprocess.PIPE)
	output = out.stdout.decode ("ascii").split ()

	if len(res) != len(output):
		print ("Wrong output: must be {0}, got {1}".format (res, output))
		return

	if len(res) > 1:

		noutput = np.array(output[0:-1], dtype=int)
		k = 0		
		for i, j in zip(noutput, res):
			if i != j:
				print ("must be {0}, got {1} at {2}".format (j, i, k))
				return
			k += 1

	if output[-1] != res[-1]:
		print ("must be {0}, got {1}".format (res[-1], output[-1]))
		return

	print ("pass test {0}".format (args))


if __name__ == '__main__':

	check ("100 a100", [0, 'All_right'])
	check ("100 a50 a50", [0, 50, 'All_right'])
	check ("100 a10 a10 a79 a1", [0, 10, 20, 99, 'All_right'])
	
	check ("100 a50 a50 r a99 a1", [0, 50, 0, 99, 'All_right'])
	check ("100 a1 r a100 r a100", [0, 0, 0, 'All_right'])

	check ("100 a1000 a100", [-1, 0, 'All_right'])
	check ("100 a100 a1 a100 r a100", [0, -1, -1, 0, 'All_right'])
	check ("100 a100 r a1", [0, 0, 'All_right'])

	check ("0 a0 a0 a0 a1 a2", [-1, -1, -1, -1, -1, "All_right"])
	check ("-1", ["std::bad_alloc"])
	check ("100 a-1", ["std::bad_alloc"])
	check ("100 a10 a-5", [0, "std::bad_alloc"])

	check ("10000000000000000000", ["std::bad_alloc"])
	
