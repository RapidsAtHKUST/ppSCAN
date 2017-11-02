CXX := icpc

CPPFLAGS := -shared-intel -shared-libgcc -g -march=native -O3 -qopenmp --std=c++0x -Xlinker -rpath -Xlinker /opt/intel/lib/mic

scanxp:scanxp.cpp graph.cpp

clean:
	$(RM) scanxp
