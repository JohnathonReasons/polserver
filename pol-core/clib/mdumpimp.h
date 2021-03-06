/*
History
=======

Notes
=======

*/

#ifndef CLIB_MDUMPIMP_H
#define CLIB_MDUMPIMP_H

namespace Pol {
  namespace Clib {
	class HiddenMiniDumper
	{
	public:
	  static void Initialize();

	  static bool _Initialized;
	  static std::string _MiniDumpType;
	  static unsigned _DumpCount;
	  static char _StartTimestamp[32];

	  static LONG WINAPI TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo );
      static void print_backtrace();
	};
  }
}
#endif
