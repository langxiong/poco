//
// ServerApplication.h
//
// $Id: //poco/Main/Util/include/Poco/Util/ServerApplication.h#4 $
//
// Library: Util
// Package: Application
// Module:  ServerApplication
//
// Definition of the ServerApplication class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Util_ServerApplication_INCLUDED
#define Util_ServerApplication_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/Application.h"
#include "Poco/Event.h"


namespace Poco {
namespace Util {


class Util_API ServerApplication: public Application
	/// A subclass of the Application class that is used
	/// for implementing server applications.
	///
	/// A ServerApplication allows for the application
	/// to run as a Windows service or as a Unix daemon
	/// without the need to add extra code.
	///
	/// For a ServerApplication to work both from the command line
	/// and as a daemon or service, a few rules must be met:
	///   - Subsystems must be registered in the constructor.
	///   - All non-trivial initializations must be made in the
	///     initialize() method.
	///   - At the end of the main() method, waitForTerminationRequest()
	///     should be called.
	///   - The main(argc, argv) function must look as follows:
	///
	///   int main(int argc, char** argv)
	///   {
	///       MyServerApplication app;
	///       return app.run(argc, argv);
	///   }
	///
	/// The POCO_SERVER_MAIN macro can be used to implement main(argc, argv).
	/// If POCO has been built with POCO_WIN32_UTF8, POCO_SERVER_MAIN supports
	/// Unicode command line arguments.
	///
	/// On Windows platforms, an application built on top of the
	/// ServerApplication class can be run both from the command line
	/// or as a service.
	///
	/// To run an application as a Windows service, it must be registered
	/// with the Windows Service Control Manager (SCM). To do this, the application
	/// can be started from the command line, with the /registerService option
	/// specified. This causes the application to register itself with the
	/// SCM, and then exit. Similarly, an application registered as a service can
	/// be unregistered, by specifying the /unregisterService option.
	/// The file name of the application executable (excluding the .exe suffix)
	/// is used as the service name. Additionally, a more user-friendly name can be
	/// specified, using the /displayName option (e.g., /displayName="Demo Service").
	///
	/// An application can determine whether it is running as a service by checking
	/// for the "application.runAsService" configuration property.
	/// 
	///     if (config().getBool("application.runAsService", false))
	///     {
	///         // do service specific things
	///     }
	///
	/// Note that the working directory for an application running as a service
	/// is the Windows system directory (e.g., C:\Windows\system32). Take this
	/// into account when working with relative filesystem paths.
	///
	/// An application registered as a Windows service can be started
	/// with the NET START <name> command and stopped with the NET STOP <name>
	/// command. Alternatively, the Services MMC applet can be used.
	///
	/// On Unix platforms, an application built on top of the ServerApplication
	/// class can be optionally run as a daemon. A daemon, when launched, immediately
	/// forks off a background process that does the actual work. After launching
	/// the background process, the foreground process exits.
	/// 
	/// After the initialization is complete, but before entering the main() method,
	/// the current working directory for the daemon process is changed to the root
	/// directory ("/"), as it is common practice for daemon processes. Therefore, be
	/// careful when working with files, as relative paths may not point to where
	/// you expect them point to.
	///
	/// An application can determine whether it is running as a daemon by checking
	/// for the "application.runAsDaemon" configuration property.
	///
	///     if (config().getBool("application.runAsDaemon", false))
	///     {
	///         // do daemon specific things
	///     }
{
public:
	ServerApplication();
		/// Creates the ServerApplication.

	~ServerApplication();
		/// Destroys the ServerApplication.
		
	bool isInteractive() const;
		/// Returns true if the application runs from the command line.
		/// Returns false if the application runs as a Unix daemon
		/// or Windows service.

	int run(int argc, char** argv);
		/// Runs the application by performing additional initializations
		/// and calling the main() method.

#if defined(POCO_WIN32_UTF8)
	int run(int argc, wchar_t** argv);
		/// Runs the application by performing additional initializations
		/// and calling the main() method.
		///
		/// This Windows-specific version of init is used for passing
		/// Unicode command line arguments from wmain().
#endif

protected:
	int run();
	void waitForTerminationRequest();
	void defineOptions(OptionSet& options);
	void handleOption(const std::string& name, const std::string& value);
	static void terminate();

private:
#if defined(POCO_OS_FAMILY_UNIX)
	bool isDaemon(int argc, char** argv);
	void beDaemon();
#elif defined(POCO_OS_FAMILY_WINDOWS)
	enum Action
	{
		SRV_RUN,
		SRV_REGISTER,
		SRV_UNREGISTER
	};
	static BOOL __stdcall ConsoleCtrlHandler(DWORD ctrlType);
	static void __stdcall ServiceControlHandler(DWORD control);
	static void __stdcall ServiceMain(DWORD argc, LPTSTR* argv);

	bool hasConsole();
	bool isService();
	void beService();
	void registerService();
	void unregisterService();
	
	Action      _action;
	std::string _displayName;

	static Poco::Event           _terminated;
	static SERVICE_STATUS        _serviceStatus; 
	static SERVICE_STATUS_HANDLE _serviceStatusHandle; 
#endif
};


} } // namespace Poco::Util


//
// Macro to implement main()
//
#if defined(_WIN32) && defined(POCO_WIN32_UTF8)
	#define POCO_SERVER_MAIN(App) \
	int wmain(int argc, wchar_t** argv)	\
	{									\
		App app;						\
		return app.run(argc, argv);		\
	}
#else
	#define POCO_SERVER_MAIN(App) \
	int main(int argc, char** argv)		\
	{									\
		App app;						\
		return app.run(argc, argv);		\
	}
#endif


#endif // Util_ServerApplication_INCLUDED
