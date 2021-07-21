/*
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
*/
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
