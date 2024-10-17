.AUTODEPEND

#		*Translator Definitions*
CC = tcc +KEYTEST.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = C:\TC\LIB
INCLUDEPATH = C:\TC\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 keydefs.obj \
 keynames.obj \
 keytest.obj

#		*Explicit Rules*
keytest.exe: keytest.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/L$(LIBPATH) @&&|
c0s.obj+
keydefs.obj+
keynames.obj+
keytest.obj
keytest
		# no map file
emu.lib+
maths.lib+
cs.lib
|


#		*Individual File Dependencies*
keydefs.obj: keytest.cfg keydefs.c 

keynames.obj: keytest.cfg keynames.c 

keytest.obj: keytest.cfg keytest.c 

#		*Compiler Configuration File*
keytest.cfg: keytest.mak
  copy &&|
-v
-vi-
-w-ret
-w-nci
-w-inl
-wpin
-wamb
-wamp
-w-par
-wasm
-wcln
-w-cpt
-wdef
-w-dup
-w-pia
-wsig
-wnod
-w-ill
-w-sus
-wstv
-wucp
-wuse
-w-ext
-w-ias
-w-ibc
-w-pre
-w-nst
-I$(INCLUDEPATH)
-L$(LIBPATH)
| keytest.cfg


