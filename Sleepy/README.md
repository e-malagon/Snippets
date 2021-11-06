# Control long-running tasks in a secondary thread(Sleepy)

A long-running task can be executed inside of a secondary thread. This approach is convenient because the main thread doesn't need to worry about the execution of the task. A common practice is to use a "while running" pattern like this:

```cpp
#include <iostream>
#include <thread>

using namespace std;

bool running = true;

void run()
{
    while (running) {
        // Do something
    }
}

int main()
{
    std::thread task(run);
    running = false; // Stop the thread
    task.join();
    return 0;
}
```

This approach has an important drawback. It must be working continuously to be able to check the running flag, even if the task only needs to be executed a few times. There is no option to use a long sleep because the thread won't be able to catch a change in the running flag. This design is a waste of resources.

This issue can be solved using a condition_variable alongside the running flag. A condition_variable is a class that can help us with the synchronization process. It was introduced in C++11. It includes some methods to wait for a change in some variable and other methods to notify the change. That is what we need here. It can be used to replace the sleep function with the wait_for() method. It will sleep the thread until the flag is changed or the timeout is reached. Any change in the running flag must be followed by a notify_all() to force the thread to wake up and check the flag. It not only prevents the thread from being blocked by the sleep but also can be used to force a wake up from other threads.

There is still a remaining issue if the task is still running when the flag is changed the main thread must wait until the job is finished or interrupt the task without knowing the status of it. It can be prevented using a secondary flag alongside another condition_variable, it is possible to check if the task is still running and wait for a few seconds to take control again and decide if it is necessary to interrupt it or give a few seconds more.

Here is a better implementation of this program:

```cpp
#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

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

    //Wakeup a sleeping thread without finishing it
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
        //If the thread is still alive when the timeout was reached,
        //the wait_for must return false, and the process has to continue.
        if (_alive)
            return !_aliveCV.wait_for(lockAlive, time, [&] {
              return !_alive;/*If still alive, wait until timeout*/ 
            });
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
            return _bussyCV.wait_for(lockBussy, time, [&] {
              return !_bussy;/*If still bussy, wait until timeout*/
            });
        else
            return true;
    }

  	//Set the progress of the job
    void progress(int progress) {
        _progress = progress;
    }

  	//Get the progress of the job
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
```

The Sleepy class is doing all synchronization tasks. The secondary thread must call start, bussy, and await methods to interact with the Sleepy class. The other threads must call wakeUp and finish methods to ask the thread to wake up and do its job or exit. Optionally progress can be used to communicate the progress of current execution to all the threads. 

Let see some output snippets to review how the code is working.

If the enter key is pressed before the 35 seconds threshold is reached, there is no time to finish the job. The process finishes immediately, leaving the responsibility to the OS to clean up the secondary thread.

```console
Press enter to finish:
Working...
0 1 2 3 4 5 6 7 8 
Background task is still running.
Exit.
```

If the enter key is pressed after the 35 seconds threshold was reached, but before 45 seconds, the task needs more than 15 seconds to finish, but the main thread is trying to wait enough time to leave it to finish the job.

```console
Press enter to finish:
Working...
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 
Wait 15 second before exit...
42 43 44 45 46 47 48 49 50 51 52 53 54 55 56
Background task is still running.
Exit.
```

If the enter key is pressed after the 45 seconds threshold was reached, the task has enough time to finish.

```console
Press enter to finish:
Working...
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 
Wait 15 second before exit...
56 57 58 59 60 Going to sleep...
background task ended successfully
Exit.
```

If the enter key is pressed when the secondary thread is sleeping, there is no need to wait. The process is finished immediately.

```console
Press enter to finish:
Working...
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 Going to sleep...

Wait 15 second before exit...
background task ended successfully
Exit.
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