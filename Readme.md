
- [Project structure](#tree)
  - [common](#common)
  - [generator](#generator)
  - [processor](#processor)
  - [logger](#logger)
  - [mq_client](#mq_client)
  - [random](#random)
- [Build](#build)
- [Run](#Run)
- [Requirements](#requirements)
- [Troubleshooting](#troubleshooting)


<a name="tree"></a>
```
├── build.sh
├── CMakeLists.txt
├── common
│   ├── config.h
│   └── data.h
├── generator
│   ├── CMakeLists.txt
│   └── src
│       └── main.cpp
├── logger
│   ├── CMakeLists.txt
│   └── log.h
├── mq_client
│   ├── cli_wrap.h
│   └── CMakeLists.txt
├── processor
│   ├── CMakeLists.txt
│   └── src
│       └── main.cpp
├── random
│   ├── CMakeLists.txt
│   └── random.h
└── Readme.md

```
### common
contains common headers (Data and config)

### generator
has an implementation of the _simple random location data generator_

### processor
has an implementation of the _location data processor_

### logger
simple logger implementation, to play with `fmt` and `source_location`

### mq_client
wrapper for mqtt client

### random
just for fun implementation of pseudorandom number generator gotten from [Jason Turner](https://www.youtube.com/watch?v=xMdwK9p5qOw) c++ vlog, [original source](https://prng.di.unimi.it/xoroshiro128plusplus.c)


## Build
```bash
./build.sh
```
## Run
```bash
mosquito -p 5555
./build/processor/proc
./build/generator/gen
```
Change port in [config](common/config.h#L9) if needed.

Type `q + Enter` in terminal to stop proc/gen.

## Requirements
cmake, clang/g++/build-essential, mosquito, [paho.mqtt.cpp](https://github.com/eclipse/paho.mqtt.cpp)

## Troubleshooting
depending on installed `paho.qtt.c` library, change `target_link_libraries` for [mq_client](mq_client/CMakeLists.txt#L9) according to [paho C client comprises four variant libraries](https://github.com/eclipse/paho.mqtt.c#libraries)

Currently `mq_client` is targeting to `paho-mqtt3as`
