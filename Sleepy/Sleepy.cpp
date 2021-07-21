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

#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

//This class allows having control of the life of a secondary thread
class Sleepy {
   public:
    Sleepy() : _alive(false), _bussy(false) {
    }

    //Mark the procces as alive
    void start() {
        std::unique_lock<std::mutex> lockAlive(_aliveMutex);
        _alive = true;
        _aliveCV.notify_all();
    }

    //Set the bussy flag to avoid any interruption
    void bussy() {
        std::unique_lock<std::mutex> lockBussy(_bussyMutex);
        _bussy = true;
        _progress = 0;
        _bussyCV.notify_all();
    }

    //Wakeup a sleeping thread
    void wakeUp() {
        std::unique_lock<std::mutex> lockAlive(_aliveMutex);
        if (_alive)
            _aliveCV.notify_all();
    }

    //Wait for a while and check if the thread is still alive or dead
    //Returns true if the thread must continue working, false in the other case.
    template <class Rep, class Period>
    bool await(std::chrono::duration<Rep, Period> const &time) {
        std::unique_lock<std::mutex> lockBussy(_bussyMutex);
        _bussy = false;
        _bussyCV.notify_all();
        lockBussy.unlock();  //Avoid a deadlock
        std::unique_lock<std::mutex> lockAlive(_aliveMutex);
        //If the thread is still alive when the timeout was reached, the wait_for must return false, and the process has to continue.
        if (_alive)
            return !_aliveCV.wait_for(lockAlive, time, [&] { return !_alive;/*If still alive, wait until timeout*/ });
        else
            return false;
    }

    //Finish the thread and wait if busy
    //Returns true if the thread was able to finish its job, false in the other case.
    template <class Rep, class Period>
    bool finish(std::chrono::duration<Rep, Period> const &time) {
        std::unique_lock<std::mutex> lockAlive(_aliveMutex);
        _alive = false;
        _aliveCV.notify_all();
        lockAlive.unlock();  //Avoid a deadlock
        std::unique_lock<std::mutex> lockBussy(_bussyMutex);
        if (_bussy)
            return _bussyCV.wait_for(lockBussy, time, [&] { return !_bussy;/*If still bussy, wait until timeout*/ });
        else
            return true;
    }

    void progress(int progress) {
        _progress = progress;
    }

    int progress() const {
        return _progress;
    }

   private:
    //Control the life of the thread
    std::condition_variable _aliveCV;
    std::mutex _aliveMutex;
    bool _alive;

    //Control the busy time
    std::condition_variable _bussyCV;
    std::mutex _bussyMutex;
    bool _bussy;

    //Control the progress of the job
    std::atomic<int> _progress;
};

Sleepy sleepy;

void run() {
    sleepy.start();
    do {
        sleepy.bussy();
        std::cout << "Working..." << std::endl;
        //Simulate some long running task
        for (int i = 0; i <= 60; i++) {
            sleepy.progress(i);
            std::this_thread::sleep_for(1s);
            std::cout << i << " ";
            std::cout.flush();
        }
        std::cout << "Going to sleep..." << std::endl;
    } while (sleepy.await(30s)); //Wait 30 seconds to do the job again
}

int main() {
    std::cout << "Press enter to finish:" << std::endl;
    std::thread task(run);
    std::string str;
    std::getline(std::cin, str);
    if (35 < sleepy.progress()) {  //Job is almost finished or not running.
        std::cout << "Wait 15 second before exit..." << std::endl;
        if (sleepy.finish(15s)) {
            std::cout << "background task ended successfully" << std::endl;
            task.join();  //Wait the thread to finish
        } else {
            std::cout << "Background task is still running." << std::endl;
            task.detach();  //Allow the OS to clean up
        }
    } else {  //Exit without waiting the task to finish.
        std::cout << "Background task is still running." << std::endl;
        task.detach();  //Allow the OS to clean up
    }
    std::cout << "Exit." << std::endl;
    return 0;
}