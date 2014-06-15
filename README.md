[![Build Status](https://travis-ci.org/mgerhardy/simpleai.svg?branch=master)](https://travis-ci.org/mgerhardy/simpleai)

### simpleai

SimpleAI is a small C++ AI behaviour tree library released under MIT.

I've written a small blog entry about it here: http://mgerhardy.blogspot.de/2014/03/simpleai-debugger-simpleai-last-few.html

### Features:

* LUA script interface to create the trees - but every other data source
  is possible, too. The LUA script interface is just implemented as an
  example.
* Aggro list implementation
* Several standard selectors and conditions
* Group management
* Movement implementations for steering
* Network based remote debugging
* QT5 based remote debugger

### Compile the lib:

* ./configure
* make
* make install

### Compile the remote debugger

* cd src/debugger
* qmake
* make

### Running it:

SimpleAI comes with a small example that is located in src/example. You can
run this with:

  ./src/example/example src/example/behaviours.lua

After you ran it, you can connect with the remote debugger and inspect the live
state of every spawned AI entity.

### Using it:

* Make sure your game character extends ICharacter or includes it as component.
* Implement your behaviour tree loader by extending the class ITreeloader.
* Extend the AIDebugger to deliver your own MapView that renders the map of
  your game.
* Add your own condition and task factories to the AIRegistry.
* Assign attributes to your game characters that should be shown in the
  debuggers live view.
