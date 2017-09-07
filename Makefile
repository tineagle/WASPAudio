main: audio

audio: audio.o
	clang -o audio audio.o

audio.o : audio.h audio.c
	clang -c audio.c

clean:
	rm -f audio audio.o
