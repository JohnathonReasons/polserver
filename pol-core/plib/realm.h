/*
History
=======
2005/06/01 Shinigami: added getmapshapes - to get access to mapshapes
                      added getstatics - to fill a list with statics
2005/06/06 Shinigami: added readmultis derivative - to get a list of statics

Notes
=======

*/

#ifndef PLIB_REALM_H
#define PLIB_REALM_H

#include "realmdescriptor.h"
#include "staticblock.h"
#include "../pol/uconst.h" // for MOVEMODE under gcc, bleh
#include "../pol/udatfile.h"

#include "WorldChangeReasons.h"

#include "../clib/rawtypes.h"

#include <set>
#include <vector>
#include <memory>


namespace Pol {
  namespace Core {
	class UObject;
	struct LosObj;
	struct Zone;
	class ItemsVector;
  }
  namespace Mobile {
	class Character;
  }
  namespace Items {
	class Item;
  }
  namespace Multi {
	class UMulti;
  }
  namespace Plib {
	class MapServer;
	class MapShapeList;
	struct MAPTILE_CELL;
	class MapTileServer;
	class StaticServer;
	

	typedef std::vector<Multi::UMulti*> MultiList;

	class Realm
	{
	public:
	  explicit Realm( const std::string& realm_name, const std::string& realm_path = "" );
	  explicit Realm( const std::string& realm_name, Realm* realm );
	  ~Realm();
	  bool is_shadowrealm;
	  unsigned int shadowid;
	  Realm* baserealm;
	  const std::string shadowname;

	  unsigned short width() const;
	  unsigned short height() const;
	  unsigned short grid_width() const;
	  unsigned short grid_height() const;

	  unsigned season() const;

	  bool valid( unsigned short x, unsigned short y, short z ) const;
	  const std::string name() const;

      void add_mobile(const Mobile::Character& chr, WorldChangeReason reason);
      void remove_mobile(const Mobile::Character& chr, WorldChangeReason reason);

      void add_toplevel_item(const Items::Item& item);
      void remove_toplevel_item(const Items::Item& item);

      void add_multi(const Multi::UMulti& multi);
      void remove_multi(const Multi::UMulti& multi);

      unsigned int mobile_count() const;
      unsigned int offline_mobile_count() const;
      unsigned int toplevel_item_count() const;
      unsigned int multi_count() const;

	  bool walkheight( unsigned short x, unsigned short y, short oldz,
					   short* newz,
					   Multi::UMulti** pmulti, Items::Item** pwalkon,
					   bool doors_block,
					   Core::MOVEMODE movemode,
					   short* gradual_boost = NULL );
	  bool walkheight( const Mobile::Character* chr, unsigned short x, unsigned short y, short oldz,
					   short* newz,
					   Multi::UMulti** pmulti, Items::Item** pwalkon,
					   short* gradual_boost = NULL );

	  bool lowest_walkheight( unsigned short x, unsigned short y, short oldz,
							  short* newz,
							  Multi::UMulti** pmulti, Items::Item** pwalkon,
							  bool doors_block,
							  Core::MOVEMODE movemode,
							  short* gradual_boost = NULL );

	  bool dropheight( unsigned short dropx,
					   unsigned short dropy,
					   short dropz,
					   short chrz,
					   short* newz,
					   Multi::UMulti** pmulti );

	  bool has_los( const Core::LosObj& att, const Core::LosObj& tgt ) const;
	  bool has_los( const Core::UObject& iatt, const Core::UObject& itgt ) const;
	  bool has_los( const Mobile::Character& iatt, const Core::UObject& itgt ) const;

	  bool navigable( unsigned short x, unsigned short y, short z, short height ) const;

	  Multi::UMulti* find_supporting_multi( unsigned short x, unsigned short y, short z ) const;

	  bool lowest_standheight( unsigned short x, unsigned short y, short* z ) const;
	  bool findstatic( unsigned short x, unsigned short y, unsigned short objtype ) const;
	  void getstatics( StaticEntryList& statics, unsigned short x, unsigned short y ) const;
	  bool groundheight( unsigned short x, unsigned short y, short* z ) const;
	  MAPTILE_CELL getmaptile( unsigned short x, unsigned short y ) const;
	  void getmapshapes( MapShapeList& shapes, unsigned short x, unsigned short y, unsigned int anyflags ) const;
	  void readmultis( MapShapeList& vec, unsigned short x, unsigned short y, unsigned int flags ) const;
	  void readmultis( MapShapeList& vec, unsigned short x, unsigned short y, unsigned int flags, MultiList& mvec ) const;
	  void readmultis( Core::StaticList& vec, unsigned short x, unsigned short y ) const;

	  Core::Zone** zone;
	  std::set<unsigned int> global_hulls; //xy-smashed together
	  unsigned getUOMapID() const;
	  unsigned getNumStaticPatches() const;
	  unsigned getNumMapPatches() const;
	  static unsigned int encode_global_hull( unsigned short ax, unsigned short ay );
	protected:
	  static void standheight( Core::MOVEMODE movemode,
							   MapShapeList& shapes,
							   short oldz, bool* result,
							   short* newz,
							   short* gradual_boost = NULL );

	  static void lowest_standheight( Core::MOVEMODE movemode,
									  MapShapeList& shapes,
									  short oldz, bool* result,
									  short* newz,
									  short* gradual_boost = NULL );

	  static bool dropheight( MapShapeList& shapes,
							  short dropz,
							  short chrz,
							  short* newz );

	  void readdynamics( MapShapeList& vec,
						 unsigned short x,
						 unsigned short y,
						 Core::ItemsVector& walkon_items,
						 bool doors_block );

	  bool dynamic_item_blocks_los( const Core::LosObj& att, const Core::LosObj& target,
									unsigned short x, unsigned short y, short z ) const;

	  bool static_item_blocks_los( unsigned short x, unsigned short y, short z ) const;
	  bool los_blocked( const Core::LosObj& att, const Core::LosObj& target,
						unsigned short x, unsigned short y, short z ) const;

	  Multi::UMulti* find_supporting_multi( MultiList& mvec, short z ) const;

	private:
	  const RealmDescriptor _descriptor;
      unsigned int _mobile_count;
      unsigned int _offline_count;
      unsigned int _toplevel_item_count;
      unsigned int _multi_count;
	  std::unique_ptr<MapServer> _mapserver;
	  std::unique_ptr<StaticServer> _staticserver;
	  std::unique_ptr<MapTileServer> _maptileserver;

	private:
	  // not implemented:
	  Realm& operator=( const Realm& );
	  Realm( const Realm& );
	public:
      size_t sizeEstimate() const;
	};


    inline unsigned int Realm::mobile_count()  const {
        return _mobile_count;
    }
    inline unsigned int Realm::offline_mobile_count()  const {
        return _offline_count;
    } 
    inline unsigned int Realm::toplevel_item_count()  const {
        return _toplevel_item_count;
    }
    inline unsigned int Realm::multi_count()  const {
        return _multi_count;
    }

    inline void Realm::add_toplevel_item(const Items::Item& /*item*/) {
        ++_toplevel_item_count;
    }
    inline void Realm::remove_toplevel_item(const Items::Item& /*item*/) {
        --_toplevel_item_count;
    }

    inline void Realm::add_multi(const Multi::UMulti& /*multi*/) {
        ++_multi_count;
    }
    inline void Realm::remove_multi(const Multi::UMulti& /*multi*/) {
        --_multi_count;
    }

	inline unsigned Realm::getUOMapID() const
	{ 
	  return _descriptor.uomapid; 
	};
	inline unsigned Realm::getNumStaticPatches() const
	{ 
	  return _descriptor.num_static_patches; 
	};
	inline unsigned Realm::getNumMapPatches() const
	{ 
	  return _descriptor.num_map_patches; 
	};
	inline unsigned int Realm::encode_global_hull( unsigned short ax, unsigned short ay )
	{
	  return ( static_cast<unsigned int>( ax ) << 16 ) | ay;
	}

	inline unsigned short Realm::width() const
	{
	  return _descriptor.width;
	}
	inline unsigned short Realm::height() const
	{
	  return _descriptor.height;
	}

  }
}
#endif
