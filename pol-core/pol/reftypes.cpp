/*
History
=======


Notes
=======

*/
#include "reftypes.h"

#include "uobject.h"
#include "mobile/charactr.h"
#include "item/item.h"
#include "mobile/npc.h"

#include "../clib/logfacility.h"

#include <map>

namespace Pol {
  namespace Core {
#if REFTYPE_DEBUG
	int hits = 0;
	int uobjectrefs = 0;
	int characterrefs;
	int npcrefs;
	int itemrefs;

	typedef std::map<int, CharacterRef*> CharacterRefs;
	CharacterRefs crefs;
	unsigned int crefs_count;
#endif

	void display_reftypes()
	{
#if REFTYPE_DEBUG
      fmt::Writer tmp;
	  for ( CharacterRefs::iterator itr = crefs.begin(); itr != crefs.end(); ++itr )
	  {
        tmp << "cref " << ( *itr ).first << "=0x" << fmt::hexu(( *itr ).second->get()->serial )<< "\n";
	  }
      INFO_PRINT << tmp.str();
#endif
	}
#if REFTYPE_DEBUG
	UObjectRef::UObjectRef( UObject* ptr ) : ref_ptr<UObject>(ptr)
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
	  }
	  ++uobjectrefs;
	}
	UObjectRef::UObjectRef( const UObjectRef& rptr ) : ref_ptr<UObject>(rptr)
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
	  }
	  ++uobjectrefs;
	}

	UObjectRef::~UObjectRef()
	{
	  --uobjectrefs;
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
	  }
	}

	CharacterRef::CharacterRef( Mobile::Character* ptr ) : ref_ptr<Mobile::Character>(ptr), cref_instance(0)
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
		cref_instance = crefs_count++;
		if (cref_instance == 50)
		{
		  cref_instance = cref_instance;
		}
		crefs[ cref_instance ] = this;
	  }
	  ++characterrefs;
	}
    CharacterRef::CharacterRef( const CharacterRef& rptr ) : ref_ptr<Mobile::Character>( rptr ), cref_instance( 0 )
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
		cref_instance = crefs_count++;
		if (cref_instance == 50)
		{
		  cref_instance = cref_instance;
		}
		crefs[ cref_instance ] = this;
	  }
	  ++characterrefs;
	}
	CharacterRef::~CharacterRef()
	{
	  --characterrefs;
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		if (cref_instance == 50)
		{
		  cref_instance = cref_instance;
		}
		crefs.erase( cref_instance );
		--hits;
	  }
	}

	CharacterRef& CharacterRef::operator=( const CharacterRef& rptr )
	{
	  set( rptr.get() );

	  return *this;
	}
    void CharacterRef::set( Mobile::Character* chr )
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		crefs.erase( cref_instance );
		--hits;
	  }

      ref_ptr<Mobile::Character>::set( chr );

	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
		cref_instance = crefs_count++;
		crefs[ cref_instance ] = this;
	  }
	}
	void CharacterRef::clear()
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		crefs.erase( cref_instance );
		--hits;
	  }

      ref_ptr<Mobile::Character>::clear( );
	}


	NPCRef::NPCRef( Mobile::NPC* ptr ) : ref_ptr<Mobile::NPC>(ptr)
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
	  }
	  ++npcrefs;
	}
	NPCRef::NPCRef( const NPCRef& rptr ) : ref_ptr<Mobile::NPC>(rptr)
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
	  }
	  ++npcrefs;
	}

	NPCRef::~NPCRef()
	{
	  --npcrefs;
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		--hits;
	  }
	}


	ItemRef::ItemRef( Items::Item* ptr ) : ref_ptr<Items::Item>(ptr)
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
	  }
	  ++itemrefs;
	}

	ItemRef::ItemRef( const ItemRef& rptr ) : ref_ptr<Items::Item>(rptr)
	{
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		++hits;
	  }
	  ++itemrefs;
	}

	ItemRef::~ItemRef()
	{
	  --itemrefs;
	  if (get() && get()->serial_ext == 0x619d1300)
	  {
		--hits;
	  }
	}
#endif
  }
}