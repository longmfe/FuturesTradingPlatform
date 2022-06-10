.PHONY: debug release clean
cc=g++
target_d=./Debug/CTPtest
target_r=./Release/CTPtest
othobjs=-lstdc++ -lpthread -ldl -lrt
flg_i=-I/usr/include -I/usr/local/include -ICTPTest_64 
flg_L=-L.
cflag_d=-g -DDEBUG -std=c++0x
cflag_r=-O3 -std=c++0x
srcs=CTPTest_64/main.cpp
flg_L+=-LCTPTest_64
othobjs+=-lthostmduserapi_se
othobjs+=-lthosttraderapi_se
objs=$(patsubst %cpp,%o,$(srcs))
objs_d=$(patsubst %cpp,%do,$(srcs))
$(warning $(srcs))

%.o:%.cpp
	$(cc) $(cflag_r) $(flg_i) -c $< -o $@

%.do:%.cpp
	$(cc) $(cflag_d) $(flg_i) -c $< -o $@

debug:$(target_d)
$(target_d):$(objs_d)
	-mkdir ./Debug
	$(cc) $(cflag_d) $(flg_L) $(objs_d) $(othobjs) -o $(target_d)
	-rm $(objs_d)

release:$(target_r)
$(target_r):$(objs)
	-mkdir ./Release
	$(cc) $(cflag_r) $(flg_L) $(objs) $(othobjs) -o $(target_r)
	-rm $(objs)

clean:
	-rm $(objs) $(objs_d)
	-rm -rf ../Debug
	-rm -rf ../Release

