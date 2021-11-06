# C++ factory pattern for dynamic plugin loading on Linux

Like other compiled languages, C/C++ programs are bundled into a single executable file. It is possible to link against a shared library, but the reference to it must be provided at compile-time anyway. In UNIX-like operative systems, there are four functions that can be used to overpass this limitation. Functions dlopen, dlerror, dlsym, and dlclose allow us load and call functions from shared libraries at runtime.

#include <dlfcn.h>

void *dlopen(const char *filename, int flag);

char *dlerror(void);

void *dlsym(void *handle, const char *symbol);

int dlclose(void *handle);

Those functions are well documented on man pages, so we will focus only on a small explanation of each one. Function dlopen allows us to load a shared object and get an opaque handle for the loaded object and dlclose must be used to release the reference to that handle. Function dlsym is used to obtain the address of a symbol and dlerror is used to check if there was an error on the call of the previous functions.

Here is an example got from Ubuntu man pages:

```c
#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char **argv) {
    void *handle;
    double (*cosine)(double);
    char *error;

    handle = dlopen ("libm.so", RTLD_LAZY);
    if (!handle) {
        fprintf (stderr, "%s\n", dlerror());
        exit(1);
    }

    cosine = dlsym(handle, "cos");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }

    printf ("%f\n", (*cosine)(2.0));
    dlclose(handle);
    return 0;
}
```

As you can see the usage is no so complicated but is still an old fashion Language C style. We can take advantage of those functions to create a factory pattern to load some plugins at runtime. 


## Plugin class definition 

The plugin class is an abstract class that contains a list of methods to be implemented by each plugin. Also, it has a static newPlugin method. This method is called from outside of the class to get a new instance of the currently loaded plugin. Also, it includes an inner factory class and a static pointer to it. This class is the key that makes this pattern work. The constructor of the factory sets the static pointer to this, making it the default factory. After that, each call to Plugin::newPlugin will call the newPlugin method of the default factory, returning an instance of that plugin. This implementation allows only one plugin at a time, but it is not so complicated to add support for multiple plugins. 

```cpp
#pragma once

//Base plugin class
class Plugin {
public:
    // Plugin methods, this is pure virtual in this example but add an
    // implementation to have a default implementation
    virtual void run() = 0;
    
    // Instantiate a new plugin without being worried of the factory
    static Plugin* newPlugin();
    
    // Self-registered factory
    class Factory {
	public:
		Factory();
		~Factory();
		virtual Plugin* newPlugin() = 0;
	};

private:
    // Pointer to a self-registered factory
    // Use std::map<std::string, Factory*>
    // or any other container to allow more
    // than one plugin at a time.
	static Factory* factory;
};
```


## Plugin methods implementation

The Plugin::newPlugin method must check if the factory pointer is pointing to a valid factory class to obtain a new instance of the loaded plugin.

```cpp
#include "Plugin.hpp"

#include <iostream>

Plugin::Factory* Plugin::factory = nullptr;

Plugin* Plugin::newPlugin() {
    if(Plugin::factory) {
        // If a factory is available then return whatever it returns
        return Plugin::factory->newPlugin();
    } else {
        // Instead of throwing an exception a default implementation
        // can be returned
        throw std::runtime_error("Plugin not available");
    }
}

Plugin::Factory::Factory() {
    // Register this as default factory
    factory = this;
}

Plugin::Factory::~Factory() {
    // Deregister this factory
    factory = nullptr;
}
```


## Main application

In the main program, it is necessary to call only dlopen and dlclose to load and unload the shared object, but it is not required to use any other function because each loaded plugin will be self-registered.  

```cpp
#include <dlfcn.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>

#include "Plugin.hpp"

class Application {
public:
    Application(std::string pluginName) {
        plugin = dlopen(pluginName.c_str(), RTLD_NOW);
        if(plugin == NULL) {
            std::cerr << dlerror() << std::endl;
        }
    }

    ~Application() {
        if(plugin != NULL)
            dlclose(plugin);
    }

    void run() {
        // Request a new Plugin instance
        Plugin *s = Plugin::newPlugin();
        s->run();
        delete s;
    }
private:
    void* plugin;
};

int main(int argc, char **argv){
    // Run some application instances on its owned context, at the
    // end of each block all library resources will be released
    try {
        std::cout << "Plugin 1 --------------------------" << std::endl;
        Application a1("./libplugin1.so");
        a1.run();
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        std::cout << "Plugin 2 --------------------------" << std::endl;
        Application a2("./libplugin2.so");
        a2.run();
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        std::cout << "Plugin 3 --------------------------" << std::endl;
        Application a3("./libplugin3.so");
        a3.run();
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
}
```


## Plugin implementation 

To create a plugin, it is necessary to extend the base class Plugin, also implement all virtual methods of Plugin class and the newPlugin method of the Factory class. The last step is to create an instance of the factory if it is omitted there is no way to allow the Factory class to register itself.

```cpp
#include "Plugin.hpp"
#include <iostream>

class Plugin1 : public Plugin {
public:    
    void run() override {
        std::cout << "Plugin1::run()" << std::endl;
    }

    class Factory : public Plugin::Factory {
    public:
        Plugin *newPlugin() {
            return new Plugin1;
        }
    };
private:
    // Disallow class instantiation outside of the factory
    Plugin1() {
    }
};

// Define a new factory it'll be automatically registered.
Plugin1::Factory sf;
```

```cpp
#include "Plugin.hpp"
#include <iostream>

class Plugin2 : public Plugin {
public:    
    void run() override {
        std::cout << "Plugin2::run()" << std::endl;
    }
    
    class Factory : public Plugin::Factory {
    public:
        Plugin *newPlugin() {
            return new Plugin2;
        }
    };
private:
    // Disallow class instantiation outside of the factory
    Plugin2() {
    }
};

// Define a new factory it'll be automatically registered.
Plugin2::Factory sf;
```

Execution: Here is an example of the execution of the application code:
```console
Plugin 1 --------------------------
Plugin1::run()
Plugin 2 --------------------------
Plugin2::run()
Plugin 3 --------------------------
./libplugin3.so: cannot open shared object file: No such file or directory
Plugin not available
```


## License

MIT License

Copyright (c) 2021 Edgar Malagón Calderón

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.