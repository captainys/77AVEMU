import os
import subprocess
import shutil
import sys
import sys

import build

MACHINETYPE="7"

THISFILE=os.path.realpath(__file__)
THISDIR=os.path.dirname(THISFILE)
MUTSUDIR=os.path.join(THISDIR,"..")

BUILDDIR=os.path.join(MUTSUDIR,"build")
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
	fName=os.path.join(MUTSUDIR,"build","main_cui","Mutsu_CUI"+ExeExtension())
	if os.path.isfile(fName):
		return fName
	fName=os.path.join(MUTSUDIR,"build","main_cui","Release","Mutsu_CUI"+ExeExtension())
	if os.path.isfile(fName):
		return fName
	fName=os.path.join(MUTSUDIR,"build","main_cui","Mutsu_CUI.app","Contents","MacOS","Mutsu_CUI"+ExeExtension())
	if os.path.isfile(fName):
		return fName
	throw



def Run(argv):
	subprocess.Popen([
		MutsuExe(),
	]+argv).wait()



if __name__=="__main__":
	build.Run()
	Run(sys.argv[1:])
