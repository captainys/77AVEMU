import os
import subprocess
import shutil
import sys
import sys

import buildgui

MACHINETYPE="AV"

THISFILE=os.path.realpath(__file__)
THISDIR=os.path.dirname(THISFILE)
MUTSUDIR=os.path.join(THISDIR,"..")

BUILDDIR=os.path.join(MUTSUDIR,"gui","build")
SRCDIR=os.path.join(MUTSUDIR,"gui","src")
ROMDIR=os.path.join(MUTSUDIR,"..","77AVTEST","ROMS",MACHINETYPE)
DISKDIR=os.path.join(MUTSUDIR,"..","77AVTEST","DISK")
TAPEDIR=os.path.join(MUTSUDIR,"..","77AVTEST","TAPE")



def ExeExtension():
	if sys.platform.startswith('win'):
		return ".exe"
	else:
		return ""



def MutsuExe():
	fName=os.path.join(BUILDDIR,"main_gui","Mutsu_GUI"+ExeExtension())
	if os.path.isfile(fName):
		return fName
	fName=os.path.join(BUILDDIR,"main_gui","Release","Mutsu_GUI"+ExeExtension())
	if os.path.isfile(fName):
		return fName
	fName=os.path.join(BUILDDIR,"main_gui","Mutsu_GUI.app","Contents","MacOS","Mutsu_GUI"+ExeExtension())
	if os.path.isfile(fName):
		return fName
	throw



def Run(argv):
	subprocess.Popen([
		MutsuExe(),
		ROMDIR,
		"-AUTOSCALE",
		#"-SYM",
		#os.path.join(MUTSUDIR,"symtables","RUN"+MACHINETYPE+".txt"),
		#"-DEBUG",
		#"-PAUSE",
	]+argv).wait()



def GitPull():
	cwd=os.getcwd();
	os.chdir(SRCDIR)
	if os.path.isdir("public"):
		proc=subprocess.Popen(["git","pull"])
		proc.communicate();
		if 0!=proc.returncode:
			print("Failed to update public libraries.")
			throw
	else:
		proc=subprocess.Popen(["git","clone","https://github.com/captainys/public.git"])
		proc.communicate()
		if 0!=proc.returncode:
			print("Failed to checkout public libraries.")
			throw
		if True!=os.path.isdir(BUILDDIR):
			os.mkdir(BUILDDIR)
		os.chdir(BUILDDIR)
		proc=subprocess.Popen(["cmake","../src"])
		proc.communicate()
		if 0!=proc.returncode:
			print("Failed to set up CMake projects.")
			throw
	os.chdir(cwd);



if __name__=="__main__":
	GitPull()
	buildgui.Run()
	Run(sys.argv[1:])
