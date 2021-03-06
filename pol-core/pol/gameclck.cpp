/*
History
=======


Notes
=======

*/


#include "gameclck.h"

#include "polclock.h"
#include "globals/uvars.h"

#include "../clib/stlutil.h"
#include "../clib/spinlock.h"

#include <ctime>
#include <mutex>

namespace Pol {
  namespace Core {
	static gameclock_t gameclock;

	static time_t last_read;

	static Clib::SpinLock _gameclock_lock;

	void start_gameclock()
	{
	  std::lock_guard<Clib::SpinLock> lock( _gameclock_lock );
	  std::string gameclock_str;
	  if ( gamestate.global_properties->getprop( "gameclock", gameclock_str ) )
	  {
		char ch_s;
		ISTRINGSTREAM is( gameclock_str );
		is >> ch_s >> gameclock;
	  }
	  else
	  {
		gameclock = 0;
	  }

	  last_read = poltime();
	}

	void update_gameclock()
	{
	  OSTRINGSTREAM os;
	  os << "s" << read_gameclock();
	  gamestate.global_properties->setprop( "gameclock", OSTRINGSTREAM_STR( os ) );
	}

	void stop_gameclock()
	{
	  update_gameclock();
	}

	gameclock_t read_gameclock()
	{
	  std::lock_guard<Clib::SpinLock> lock( _gameclock_lock );
	  time_t new_last_read = poltime();
	  unsigned int diff = static_cast<unsigned int>( new_last_read - last_read );
	  gameclock += diff;
	  last_read = new_last_read;
	  return gameclock;
	}
  }
}