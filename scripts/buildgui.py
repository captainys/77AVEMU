import os
import subprocess
import shutil
import sys

THISFILE=os.path.realpath(__file__)
THISDIR=os.path.dirname(THISFILE)
BUILDDIR=os.path.join(THISDIR,"..","gui","build")



def Run():
	cwd=os.getcwd()

	os.chdir(BUILDDIR)

	cmd=[
		"cmake",
		"--build",
		".",
		"--config",
		"Release",
		"--parallel",
	]

	proc=subprocess.Popen(cmd)
	proc.communicate();

	print(cmd)

	os.chdir(cwd)

	if proc.returncode!=0:
		print("Build Error!")
		quit()



if __name__=="__main__":
	Run()


