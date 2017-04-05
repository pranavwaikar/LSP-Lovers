
FOR 64-Bit (Various Forms):---
	 gcc -o mygcc64 mygcc64.c 
	./mygcc64 -S -o Hello_world Hello_world.c (Creates .S File)

	./mygcc64 -o Hello_world Hello_world.c (Creates .exe File)
	./Hello_world.exe

	./mygcc64 Hello_world.c	(Creates .exe File)
	./Hello_world.exe


FOR 32-Bit (Various Forms):---
	 gcc -o mygcc32 mygcc32.c 
	./mygcc32 -S -o Hello_world Hello_world.c  (Creates .S File)

	./mygcc32 -o Hello_world Hello_world.c	(Creates .exe File)
	./Hello_world.exe	

	./mygcc32 Hello_world.c	(Creates .exe File)
	./Hello_world.exe
