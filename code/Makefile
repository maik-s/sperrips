# Inspired by https://stackoverflow.com/a/2908351/2835625

SRC_DIR := . handler proto_files/cpp_out comparer mapper printers
OBJ_SUFFIX = bin/
OBJ_DIR := $(foreach dir,$(SRC_DIR), $(OBJ_SUFFIX)/$(dir)/)
#$(warning $(OBJ_DIR))
$(foreach dir,$(OBJ_DIR), $(shell mkdir -p $(dir) tests/proto))
SRC_FILES := $(foreach dir,$(SRC_DIR), $(wildcard $(dir)/*.cpp))
#$(warning $(SRC_FILES))
OBJ_FILES := $(foreach srcfile, $(SRC_FILES), $(patsubst $(dir $(srcfile))$(notdir $(srcfile)), $(OBJ_SUFFIX)$(dir $(srcfile))$(basename $(notdir $(srcfile))).o, $(srcfile)))
#$(warning $(OBJ_FILES))
LDFLAGS := -pthread
CPPFLAGS := -g --std=c++17
CXXFLAGS := -pthread
LIBRARIES := -l:libprotobuf.a
.PHONY: proto_files clean

sperrips: version.h bin/proto_files/cpp_out/Syscall.pb.o $(OBJ_FILES) 
	g++ $(LDFLAGS) -o $@ -Wl,--start-group $^ $(LIBRARIES) -Wl,--end-group

version.h: ../.git/index
	echo "#define GIT_COMMIT \"$(shell git rev-parse HEAD)\"" > $@
	touch bin/Recorder.o && rm bin/Recorder.o

all: proto_files sperrips

bin/%.o: ./%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	find bin/ -type f -name "*.o" -exec rm {} \;
	find proto_files/cpp_out -type f -name "Syscall.pb.*" -exec rm {} \;
	find tests/proto -type f -exec rm {} \;
	rm sperrips

bin/Recorder.o: version.h
	g++ $(LDFLAGS) -o $@ -Wl,--start-group $^ $(LIBRARIES) -Wl,--end-group

proto_files:
	protoc --cpp_out=proto_files/cpp_out --proto_path=proto_files/ Syscall.proto --python_out=utils/proto
	mv proto_files/cpp_out/Syscall.pb.cc proto_files/cpp_out/Syscall.pb.cpp

bin/proto_files/cpp_out/Syscall.pb.o: proto_files/Syscall.proto
	protoc --cpp_out=proto_files/cpp_out --proto_path=proto_files/ Syscall.proto --python_out=utils/proto
	mv proto_files/cpp_out/Syscall.pb.cc proto_files/cpp_out/Syscall.pb.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ proto_files/cpp_out/Syscall.pb.cpp