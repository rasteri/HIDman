.AUTODEPEND

#		*Translator Definitions*
CC = tcc +MASHPIPE.CFG
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
 mashpipe.obj

#		*Explicit Rules*
mashpipe.exe: mashpipe.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/L$(LIBPATH) @&&|
c0s.obj+
keydefs.obj+
mashpipe.obj
mashpipe
		# no map file
emu.lib+
maths.lib+
cs.lib
|


#		*Individual File Dependencies*
keydefs.obj: mashpipe.cfg keydefs.c 

mashpipe.obj: mashpipe.cfg mashpipe.c 

#		*Compiler Configuration File*
mashpipe.cfg: mashpipe.mak
  copy &&|
-v
-G
-O
-Z
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
| mashpipe.cfg


