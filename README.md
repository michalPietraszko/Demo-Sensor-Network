

# Demo-Sensor-Network

Description
--------------  
  **Sensor Network Application** is a C++ showcase project demonstrating some of coding techniques, principles and challenges
  
  * For information about Sensor Network App navigate here: [What is it?](#System-information)
  * For information about building & running navigate here: [How to run it?](#Platform-support)
  * For information about current features & roadmap navigate here: [What is implemented?](#Project-information)
  * For information about used technologies & technicalities navigate here: [What is it using?](#Technicalities)

System Information
------------------------

**Sensor Network Application** is a simulation of a real world system where *Sensors* send arbitraty messages to the *Main Node*. *Main Node* is storing that messages in a circular buffer and later reporting all of the buffer content to the *System Observer*. *System Observer* has an arbitrary number of *Listeners* that are the end recepiants of all stored *Sensor* messages in a given time period.

System runtime can be observed with logs live in **./logs/syslog.log** file. [See the latest release for screenshots](https://github.com/michalPietraszko/Demo-Sensor-Network/releases/latest)

**System can be summarized as such:**
* *Sensors* periodically report some information to the *Main Node*

* *Main Node* stores the messages in circular buffer
* *Main Node* periodically reports all data in its buffer to the *System Observer*
* Upon reception of data from the *Main Node*, *System Observer* is sending that data
to all of its *Listeners*  

**UML Diagram:**

![uml diagram](https://i.imgur.com/aZ6HSVW.png)

**System configuration:**
* One or more *Sensors* with configurable *report period* and *report start value* for each
Note: For the sake of simulation *Sensors* are reporting integer values incremented by one each 
time a report is sent

* Singular *Main Node* with configurable *circular buffer size*

* Singular *System Observer* with configurable *Main Node poll time*

* One or more *Listeners* 

After the configuration is accepted system is considered to be *running* and only additional 
**Sensors** and **Listeners** can be added. No runtime removal is not supported. 

**User interaction**
Application has a simple and intuitive text interface that will guide you through the Application setup.
If you're lost at any point you can use *help* command or *quit* if you wish to quit the Application. (*version 1.0*)
		
Platform support
-----------------
Demo Sensor Network application can run under MacOS or Linux.

Dependencies
-----------------

Required dependencies are *Conan* and a reasonably modern C++ compiler (*C++ 17 support*).
  * **Conan**
    ```
    sudo apt install python3-pip
    sudo pip3 install conan
    ```
  
  * **CMake** For dependencies
  * **Clang** 11.0.3 on MacOS
  
  * **GCC** 9.3 on Linux


Note: Project was tested on MacOS with *Clang 11.0.3* and on Linux with *GCC 9.3*. *Conan* is configured to use these compilers. If you expirience any problems with building or don't have the compiler versions to match you can try changing (*Conan*) profile for your platform in **./buildscript/conan/profile**

Note: **Your compiler version, eg.** ```gcc -v``` **has to match the version speified in a Conan profile file.** 

Build
-----------------

  * **Debug mode** 
    ```
    ./generate.sh
    ```
  * **Release mode** 
    ```
    ./generate.sh --release
    ```
    
## Run
  
  * **Debug mode** 
    ```
    ./bin/Debug/SensorNetworkApp
    ```
  * **Release mode** 
    ```
    ./bin/Release/SensorNetworkApp
    ```
    
  Project information
------------------------

### Currently supported features:

**Version 0.8 - Beta** - *released*
- [x] Main Node (*no cyclic buffer*) and Sensor(s) support 
- [x] Runtime Sensor creation
- [x] Multi-process operation  (*boost interprocess*)
- [x] Event messaging system (*boost shared memory*)
- [x] Abstract rendering API
- [x] Console UI (*stdin, stdout*)
- [x] Abstract Scene Creator with support for different layouts (‘*Views*’)
- [x] Logging to a ‘*syslog*’ file

### Roadmap:

**Version 1.0 - Full Feature Support** - *in progress*
- [ ] Add cyclic buffer support to Main Node
- [ ] Add System Observer
- [ ] Add System Observer scene
- [ ] Add Listener scene
- [ ] Add ‘Help’ Scene and ‘quit’ command support

**Version 1.1 - Cleanup and New UI**
- [ ] Add ImGui renderer
- [ ] Project cleanup:
	- [ ]  Split from header mostly to header + cpp
	- [ ]  Remove bad practices eg. global ‘using’, aliases for same type used in different files eg.  
	-
				chrono::milliseconds time => Timer::DurationMs time
	- [ ] Remove unused header files
	- [ ] Fix function parameter types - remove ‘*by value*’ where not necessary, add *const* where appropriate ect.

**Version 1.2 - QoL and Performance Improvements**
- [ ] *Constexpr* what can be *constexpr*
- [ ] '*Smart*' *event* support
- [ ]  Add *GTest/GMock* support and a few basic TCs
- [ ]  Add CI support

**Version 1.3 - Additonal features**
- [ ] Add multithreading support
- [ ] Add C++ features

  
Technicalities
----------------

### Used technologies:
 
* Premake 5 - *build system*
* Conan (*conan-io*) - *package manager*
* Dear ImGui  (*in version 1.1*) - *immediate mode GUI library*
* OpenGL (*in version 1.1*) *rendering API*

### Used libraries:

* Boost (*interprocess*)
* STL

### Used design patterns and techniques:

- Singleton, Controller, MVC, State, Factory, Observer (*in version 1.0*)
- Template meta programming, Static/Dynamic Polymorphism

**Principles**
- DRY, Clean code, YAGNI, SOLID

### Some of the C++ used in the project

**C++ 11/14 features**

- lambda expressions
- future/promise
- parameter pack (*variadic templates*)
- smart pointers
- moving/universal references/perfect forwarding *and others*

**C++ 17**
- any
- fold expressions
- constexpr if
- optional *and others*

**STL**

* algorithm, any, array, cassert, chrono, functional, initializer_list, iostream,
type_traits, tuple, memory, string, sstream, utility

**Containers**
* unordered_map, vector, array, set

### Other languages / features

You can find a few not-so-great bash scripts for project building and generating code and a clang-format script 
