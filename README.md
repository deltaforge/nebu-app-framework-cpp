Nebu App Framewok C++
===

The Nebu App Framework (libnebu-app-framework) is a framework for rapid development of Nebu application extensions in C++. The framework periodically refreshes the virtual machine and physical topology information, and notifies the application extension through callbacks.

Creating an Application Extension
---

The libnebu-app-framework implements a full event loop for a Nebu application extension, allowing application-specific actions through callbacks. To use the framework, an application extension must provide the following:

 * The extension must provide a subclass of the ApplicationHooks class. This subclass must be instantiated as a singleton and provided by the initApplication() function (must also be provided by the extension as a form of dependency injection).
 * The extension must provide a subclass of the DaemonManager class which manages the daemons launched on the virtual machines available to the application.
 * The extension must subclass the Daemon class to represent one or more types of daemons used in the application.

More information can be found in the documentation in the source code, in particular in the ApplicationHooks class.

Dependencies
---

Runtime dependencies:

 * [libnebu-common](https://github.com/deltaforge/nebu-app-framework-cpp), same version as libnebu-app-framework.
 * [log4cxx](logging.apache.org/log4cxx/), version 0.10.0.
 
Compile dependencies:

 * automake, version 2.61
 * g++, version 4.4.7
 
Compiling
---

To compile libnebu-app-framework, execute the following commands:

	./bootstrap
    ./configure
    make
    make install

To compile an application extension against libnebu-app-framework, use pkg-config for the appropriate compiler flags. Log4cxx and tinyxml2 need to be linked manually as they do not provide pkg-config support. In addition, the -pthread flag must be used when compiling with GCC. See [nebu-app-hadoop](https://github.com/deltaforge/nebu-app-hadoop) or [nebu-app-mongo](https://github.com/deltaforge/nebu-app-mongo) for examples of application extensions linking against libnebu-app-framework.

License
---

The Nebu App Framework is distributed under the LGPL version 3 license. See the provided LICENSE file for more information.
