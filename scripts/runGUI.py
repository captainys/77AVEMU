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
SRCDIR=os.path.join(MUTSUDIR,"src")
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



if __name__=="__main__":
	buildgui.Run()
	Run(sys.argv[1:])
