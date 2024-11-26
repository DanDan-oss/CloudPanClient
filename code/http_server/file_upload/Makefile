
CC=gcc

CPPLFAGS=-I /usr/include
CFLAGS=-Wall
LIBS=-lfcgi

fcgi=./fileupload.exe
targer=$(fcgi)
ALL:$(targer) 

# 生成所有.o文件
%.o:%.c
	$(CC) -c $< -o $@ $(CPPLFAGS) $(CFLAGS)

$(fcgi): main.o
	$(CC) $^ $(LIBS) -o $@


clean:
	-rm -rf *.o $(targer)

.PHONY:clean ALL