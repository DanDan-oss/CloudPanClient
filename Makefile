
CC=gcc

CPPLFAGS=-I /usr/include/fastdfs \
		-I /usr/include/fastcommon
CFLAGS=-Wall
LIBS=-lfdfsclient\
	-lfastcommon

fdfs=./fdfs.exe
targer=$(fdfs)
ALL:$(targer) 

# 生成所有.o文件
%.o:%.c
	$(CC) -c $< -o $@ $(CPPLFAGS) $(CFLAGS)

$(fdfs): main.o fdfs_api.o make_log.o 
	$(CC) $^ $(LIBS) -o $@


clean:
	-rm -rf *.o $(targer)

.PHONY:clean ALL