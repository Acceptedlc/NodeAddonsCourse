v8_lib_dir := v8_lib/
v8_lib := $(addprefix $(v8_lib_dir),$(shell ls $(v8_lib_dir)))

lanch_stage_1: src/lanch_stage_1.cc $(v8_lib)
	g++ -I. -Iinclude src/lanch_stage_1.cc -o out/lanch_stage_1 -Wl,--start-group  $(v8_lib) -Wl,--end-group -lrt -ldl -pthread -std=c++0x

helloworld : src/helloworld.cc $(v8_lib)
	g++ -I. -Iinclude src/helloworld.cc -o out/helloworld -Wl,--start-group  $(v8_lib) -Wl,--end-group -lrt -ldl -pthread -std=c++0x
