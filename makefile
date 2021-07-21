all: sleepy plugins libplugin1 libplugin2
	echo "Building"

sleepy: Sleepy/Sleepy.cpp
	g++ --std=c++14 -o sleepy Sleepy/Sleepy.cpp -pthread

plugins: Plugins/Plugins.cpp Plugins/Plugin.cpp
	g++ --std=c++11 -rdynamic -ggdb -o plugins Plugins/Plugins.cpp Plugins/Plugin.cpp -ldl

libplugin1: Plugins/Plugin1.cpp
	g++ -shared -fPIC -ggdb -Wl,-soname,libplugin1.so -o libplugin1.so Plugins/Plugin1.cpp

libplugin2: Plugins/Plugin2.cpp
	g++ -shared -fPIC -ggdb -Wl,-soname,libplugin2.so -o libplugin2.so Plugins/Plugin2.cpp

clean:
	rm -f sleepy application libplugin1.so libplugin2.so
