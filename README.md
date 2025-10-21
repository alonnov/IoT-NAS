# IoT device based NAS

A distributed storage system that uses UDP communication between a master node and multiple IoT minion devices. The system implements a Network Block Device (NBD) interface for storage operations with fault tolerance through backup minions.

## Architecture

### Components

- **Master Node**: Coordinates storage operations and communicates with minion devices
- **Minion Devices**: IoT devices that handle actual storage operations
- **NBD Interface**: Provides block device interface for storage operations
- **UDP Communication**: Fast message passing between master and minions

### Key Features

- **Distributed Storage**: Data distributed across multiple minion devices
- **Fault Tolerance**: Backup minions for data redundancy
- **UDP Communication**: Lightweight protocol for IoT devices
- **NBD Interface**: Standard block device interface
- **Multi-threaded**: Concurrent operation handling
- **Plugin System**: Extensible command system

### Framework Design

The system is built on a **generic, modular framework** that provides:

- **Reactor Pattern**: Event-driven architecture for handling multiple I/O operations
- **Factory Pattern**: Dynamic command creation and registration
- **Singleton Pattern**: Global service management through Handleton
- **Thread Pool**: Efficient concurrent task execution
- **Plugin System**: Hot-swappable command modules
- **Logger**: Centralized logging with configurable levels
- **Async Operations**: Non-blocking task execution with callbacks

#### Modular Components

- **ICommand**: Generic command interface for extensible operations
- **ITaskArgs**: Abstract task arguments for different operation types
- **IInputProxy**: Pluggable input/output proxy system
- **Reactor**: Event loop for handling multiple file descriptors
- **ThreadPool**: Worker thread management for parallel processing
- **Factory**: Dynamic object creation and registration system

This modular design allows the framework to be easily 
    adapted for different use cases beyond IoT storage, 
    such as:
- Network servers
- Data processing pipelines
- Real-time systems
- Microservices architecture

## Project Structure

```
final_project/
├── concrete/                    # Concrete implementation components
│   ├── include/                # Header files for concrete components
│   │   ├── AMessage.hpp
│   │   ├── AMinionArgs.hpp
│   │   ├── ATaskArgs.hpp
│   │   ├── CommandReadRequest.hpp
│   │   ├── CommandReadResponse.hpp
│   │   ├── CommandWriteRequest.hpp
│   │   ├── CommandWriteResponse.hpp
│   │   ├── FileManager.hpp
│   │   ├── ICommand.hpp
│   │   ├── IFileManager.hpp
│   │   ├── IMinionProxy.hpp
│   │   ├── MasterProxy.hpp
│   │   ├── Message.hpp
│   │   ├── MinionManager.hpp
│   │   ├── MinionProxy.hpp
│   │   ├── NBD.hpp
│   │   ├── NBDProxy.hpp
│   │   ├── ResponseManager.hpp
│   │   ├── server.h
│   │   ├── sockets.h
│   │   ├── tcp_listener.h
│   │   ├── tcp_talker.h
│   │   ├── Ticket.hpp
│   │   ├── udp.h
│   │   ├── UID.hpp
│   │   └── utils.hpp
│   ├── src/                    # Source files for concrete components
│   │   ├── AMessage.cpp
│   │   ├── AMinionArgs.cpp
│   │   ├── ATaskArgs.cpp
│   │   ├── CommandReadRequest.cpp
│   │   ├── CommandReadResponse.cpp
│   │   ├── CommandWriteRequest.cpp
│   │   ├── CommandWriteResponse.cpp
│   │   ├── FileManager.cpp
│   │   ├── Framework.cpp
│   │   ├── MasterProxy.cpp
│   │   ├── Message.cpp
│   │   ├── MinionManager.cpp
│   │   ├── MinionProxy.cpp
│   │   ├── NBD.cpp
│   │   ├── NBDProxy.cpp
│   │   ├── ResponseManager.cpp
│   │   ├── sockets.c
│   │   ├── Ticket.cpp
│   │   ├── udp.c
│   │   ├── UID.cpp
│   │   └── utils.cpp
│   └── test/                   # Example/test main files
│       ├── test_master.cpp
│       └── test_minion.cpp
├── framework/                   # Generic framework components
│   ├── include/                # Header files for framework components
│   │   ├── ACallback.hpp
│   │   ├── AsyncInjection.hpp
│   │   ├── Callback.hpp
│   │   ├── DirMonitor.hpp
│   │   ├── Dispatcher.hpp
│   │   ├── DllLoader.hpp
│   │   ├── Factory.hpp
│   │   ├── Framework.hpp
│   │   ├── FunctionTask.hpp
│   │   ├── FutureTask.hpp
│   │   ├── Handleton.hpp
│   │   ├── ICommand.hpp
│   │   ├── IInputProxy.hpp
│   │   ├── IListener.hpp
│   │   ├── ISchedulerTask.hpp
│   │   ├── ITaskArgs.hpp
│   │   ├── ITPTask.hpp
│   │   ├── LinuxListener.hpp
│   │   ├── Logger.hpp
│   │   ├── PQ.hpp
│   │   ├── Reactor.hpp
│   │   ├── Scheduler.hpp
│   │   ├── Singleton.hpp
│   │   ├── ThreadPool.hpp
│   │   └── WaitQ.hpp
│   ├── src/                    # Source files for framework components
│   │   ├── AsyncInjection.cpp
│   │   ├── DirMonitor.cpp
│   │   ├── DllLoader.cpp
│   │   ├── Framework.cpp
│   │   ├── Handleton.cpp
│   │   ├── LinuxListener.cpp
│   │   ├── Logger.cpp
│   │   ├── Reactor.cpp
│   │   ├── Scheduler.cpp
│   │   ├── ThreadPool.cpp
│   │   └── TPTask.cpp
└── README.md                   # Project documentation
```

## Quick Start

### Prerequisites

- Linux system with NBD support
- GCC compiler
- Root privileges (for NBD operations)

### Building the Project

1. **Compile the shared library (Handleton):**
```bash
g++ -fPIC -shared -o libhandleton.so \
    ./framework/src/Handleton.cpp \
    ./framework/src/Logger.cpp \
    ./framework/src/Scheduler.cpp \
    ./framework/src/ThreadPool.cpp \
    ./framework/src/Reactor.cpp \
    ./framework/src/LinuxListener.cpp \
    ./concrete/src/ATaskArgs.cpp \
    ./concrete/src/AMessage.cpp \
    ./concrete/src/Message.cpp \
    ./concrete/src/ResponseManager.cpp \
    ./concrete/src/MinionManager.cpp \
    ./concrete/src/Ticket.cpp \
    ./concrete/src/MinionProxy.cpp \
    ./concrete/src/NBDProxy.cpp \
    ./concrete/src/UID.cpp \
    ./concrete/src/FileManager.cpp \
    ./concrete/src/MasterProxy.cpp \
    ./concrete/src/AMinionArgs.cpp \
    ./concrete/src/utils.cpp \
    ./concrete/src/udp.c \
    ./concrete/src/sockets.c \
    -I ./framework/include/ \
    -I ./concrete/include/
```

2. **Compile the applications:**
```bash
# Compile master application with your main file
g++ your_master_main.cpp \
    ./framework/src/DirMonitor.cpp \
    ./framework/src/DllLoader.cpp \
    ./framework/src/AsyncInjection.cpp \
    ./concrete/src/Framework.cpp \
    ./concrete/src/CommandReadRequest.cpp \
    ./concrete/src/CommandWriteRequest.cpp \
    ./concrete/src/NBD.cpp \
    -I ./framework/include/ \
    -I ./concrete/include/ \
    -Wl,-rpath,. \
    -L. \
    -lhandleton \
    -o master

# Compile minion application with your main file
g++ your_minion_main.cpp \
    ./framework/src/DirMonitor.cpp \
    ./framework/src/DllLoader.cpp \
    ./framework/src/AsyncInjection.cpp \
    ./concrete/src/Framework.cpp \
    ./concrete/src/CommandReadResponse.cpp \
    ./concrete/src/CommandWriteResponse.cpp \
    -I ./framework/include/ \
    -I ./concrete/include/ \
    -Wl,-rpath,. \
    -L. \
    -lhandleton \
    -o minion
```

### Running the System

#### Example 1: Basic Master-Minion Setup

**Note:** First compile the applications using the commands above, then run them as shown below.

1. **Start the master node (NBD server):**
```bash
# Run the master with NBD device and minion connections
sudo ./master /dev/nbd0 192.168.1.100 5001 192.168.1.101 5002
#                    ^device  ^minion1_ip ^port1  ^minion2_ip ^port2
```

2. **Start minion devices** (run on different machines or terminals):
```bash
# Run minion on machine 1
./minion 192.168.1.100 5001

# Run minion on machine 2  
./minion 192.168.1.101 5002
```

3. **Mount the NBD device:**
```bash
sudo nbd-client 192.168.1.100 10809 /dev/nbd0
sudo mount /dev/nbd0 /mnt/mount_point
```

#### Example 2: Using Provided Test Files

To use the provided test files, replace `your_master_main.cpp` with `test_master.cpp` and `your_minion_main.cpp` with `test_minion.cpp` in the compilation commands above.

#### Example 3: Local Testing

For local testing on a single machine:

```bash
# Terminal 1: Start master
sudo ./master /dev/nbd0 127.0.0.1 5001 127.0.0.1 5002

# Terminal 2: Start first minion
./minion 127.0.0.1 5001

# Terminal 3: Start second minion  
./minion 127.0.0.1 5002

# Terminal 4: Mount and test
sudo nbd-client 127.0.0.1 10809 /dev/nbd0
sudo mount /dev/nbd0 /mnt/test
echo "Hello IoT World!" > /mnt/test/test.txt
cat /mnt/test/test.txt
```

## Configuration

### Network Configuration

- **Master Port**: Default 5001 (UDP)
- **Minion Ports**: Configurable per minion
- **NBD Port**: 10809 (TCP)

### Storage Configuration

- **Minion Size**: 4MB per minion (configurable)
- **Backup Strategy**: Each minion has a backup minion
- **Data Distribution**: Round-robin across minions

## Testing

### Quick Start Test

Use the examples above to test the system:

1. **Follow Example 2** for local testing on a single machine
2. **Follow Example 1** for distributed testing across multiple machines

### Verification Steps

After running the system, verify it's working:

```bash
# Check if NBD device is mounted
ls -la /dev/nbd0

# Test write operations
echo "Test data" > /mnt/test/test.txt

# Test read operations  
cat /mnt/test/test.txt

# Check system logs
tail -f ./log.txt
```

## Plugin Example

You can extend the system with plugins (shared libraries) that add new commands or behaviors at runtime. Below is an example of how to create, build, and use a plugin:


```cpp
#include <iostream>
#include "Factory.hpp"
#include "Handleton.hpp"
#include "Framework.hpp"

using namespace ilrd::details;

class NewReadCommand : public ICommand {
    std::pair<std::function<bool()>, std::chrono::milliseconds> Run(std::shared_ptr<ITaskArgs> args_) override {
        (void)args_;
        std::cout << "New" << std::endl;
        
        auto callback = []() -> bool {
            std::cout << "Callback executed!" << std::endl;
            return true;
        };
        return std::make_pair(callback, std::chrono::milliseconds(2000));
    }
};

std::shared_ptr<ICommand> CreateNewReadCommand() {
    return std::make_shared<NewReadCommand>();
}

__attribute__((constructor))
void SoMain() {
    ilrd::Handleton<Factory<int, ICommand>>::GetInstance()->Register(0, CreateNewReadCommand);
    std::cout << "New" << std::endl;
}
```

### Build the Plugin

```bash
g++ -fPIC -shared -o my_plugin.so fw_plugin.cpp -I./framework/include -I./concrete/include
```

### Use the Plugin

1. Place the compiled `.so` file in the plugins directory (e.g., `./plugins` or `./framework/plugins`).
2. The framework will automatically detect and load new plugins from this directory during runtime—no restart required.

## Troubleshooting

### Common Issues

#### Permission Denied for NBD Operations

**Solution**: Run with sudo privileges:
```bash
sudo ./master
sudo nbd-client <ip> 10809 /dev/nbd0
```

#### Port Already in Use

**Solution**: Check for existing processes and kill them:
```bash
sudo lsof -i :5001
sudo kill <PID>
```

#### Minion Connection Issues

**Check**:
1. Network connectivity between master and minions
2. Firewall settings
3. Correct IP addresses and ports

### Debug Mode

The system uses compile-time debug logging control:

**For Development:**
```bash
# Compile with debugging enabled (default)
g++ your_files.cpp -o your_program
```

**For Production/Release:**
```bash
# Compile with NDEBUG to disable debug logging
g++ -DNDEBUG your_files.cpp -o your_program
```

**Usage in Code:**

**Existing code works automatically!** All your current `Logger::DEBUG` calls will be handled:


**For new debug logs, you can use:**
```cpp
    Handleton<Logger>::GetInstance()->Log("New debug message", Logger::DEBUG);
```

**Benefits:**
- **Zero runtime overhead** - debug logs are completely removed in release builds
- **Clean production code** - no debug noise in logs
- **Standard approach** - widely used in professional development
- **Compile-time optimization** - debug code doesn't even exist in release builds



## License

This project was created as an educational exercise by Alon Nov.


---

