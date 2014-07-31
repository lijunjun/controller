## Server Controller

it provides basic operation such as events, memory management, process, threads, synchronization.
it provides a small tool to controll another process. but it's not completed. you have to create events in destination process by yourself.

currently, it only supports windows platform.

About third party tools. Now using [jsoncpp][] and [google test framework][]

Now it only includes win32 lib and dll. if you want to build release or win64 release, debug. You have to build these third party tools
by yourself. And you need to put *.lib files into folder /lib/ and put *.exe or *.dll or *.pdb into folder /run/

[google test framework]:https://github.com/smarr/googletest/
[jsoncpp]: http://sourceforge.net/projects/jsoncpp/

### Future plan
dynamically control the running server.

