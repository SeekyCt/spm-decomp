/*
    Definitions used for loading maps
*/

#include <common.h>
#include <spm/evtmgr.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::map_data)

USING(spm::evtmgr::EvtScriptCode)
USING(wii::mtx::Vec3)

#define MAP_ID_MAX 0x1d4

typedef struct
{
/* 0x00 */ const char * name;
/* 0x04 */ const char * filename;
/* 0x08 */ const char * fallbackDoorName; // door name used if entered with a null name,
                                          // ignored if this is null
/* 0x0C */ Vec3 fallbackSpawnPos; // position to use when entered with a null door name
                                  // and fallbackDoorName isn't set
/* 0x18 */ EvtScriptCode * initScript; // In rel, linked by prolog function
} MapData;
SIZE_ASSERT(MapData, 0x1c)

extern MapData * mapData[MAP_ID_MAX];

/*
    Returns the MapData for a map by name
*/
MapData * mapDataPtr(const char * mapName);

/*
    Returns the init script for a map by anme
*/
EvtScriptCode * mapDataEvtPtr(const char * mapName);

CPP_WRAPPER_END()
