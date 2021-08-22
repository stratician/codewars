// Build.h 
//
// #define THE_POOL_CLUB
#define MAIN_SERVER
	//#define TEST_SERVER
    //#define SOFTWARE3D
#define MYSQL_SYSTEM

#ifdef THE_POOL_CLUB
#define FIXEDMODE
#endif

//#define ISNOOKER_VERSION		"1.6.83"
//#define IPOOL_VERSION			"0.1.1"

//	#define GAME_VERSION			"2.0.20" //	"1.8.76" //"1.6.89b" iPool
#define GAME_VERSION				"2.3.02" //	"1.8.76" //"1.6.89b" iPool
#define GAME_VERSION_SUB			"(03)"
//	#define GAME_VERSION			"1.8.76" //	"1.6.89b" iPool
//	#define GAME_VERSION			"1.8.74" //	iSnooker
#define LOCALTEST
//	#define TEST_BOT

#define SMILEYS_ENABLED

// #define WIN_VISTA
//	#define WIN_7
//	#define WIN_XP
// #define WIN_8


#ifdef WIN_8
	// #define _WIN32_WINNT		0x0600
	// #define WINVER			0x0600
	#define WIN32_LEAN_AND_MEAN
#elif defined WIN_VISTA 
	#define _WIN32_WINNT	0x0600
	#define WINVER			0x0600
	#define WIN32_LEAN_AND_MEAN
#elif defined WIN_7
	// #define _WIN32_WINNT	0x0601
	#define WINVER			0x0602
	#define WIN32_LEAN_AND_MEAN
#else WIN_XP
	// #define _WIN32_WINNT	0x0501
	#define WINVER			0x0501
	#define WIN32_LEAN_AND_MEAN
#endif





//#define _WIN32_WINNT  0x0600
//#define WINVER 0x0501


//#define SERVERCITY_IP				"217.205.138.40"
#define MEMSET_TPC_IP				"81.29.72.242"
#define MEMSET_TSC_IP				"81.29.70.200"
#define	HOSTINGLEADER_TPC_IP		"208.117.81.46" // "216.98.8.30" // "209.208.71.50" // "209.208.92.74"
#define	HOSTINGLEADER_TSC_IP		"208.117.81.46" // "209.208.71.50" //"www.thesnookerclub.com" // "209.208.71.50"

#ifdef THE_POOL_CLUB
	#define GATEWAY_IP					HOSTINGLEADER_TPC_IP//HOSTINGLEADER_TPC_IP//MEMSET_TSC_IP//MEMSET_TPC_IP//SERVERCITY_IP
#else
	#define GATEWAY_IP					HOSTINGLEADER_TSC_IP//HOSTINGLEADER_TPC_IP//MEMSET_TSC_IP//MEMSET_TPC_IP//SERVERCITY_IP
#endif

//#define SERVERCITY_URLTOURNAMENT	"http://208.117.81.46:1788/tourney.netgenie?mode=main"
#define MEMSET_TPC_URLTOURNAMENT	"http://208.117.81.46:1780/tourney.netgenie?mode=main"
//#define MEMSET_TSC_URLTOURNAMENT	"http://81.29.70.200:1788/tourney.netgenie?mode=main"
#define MEMSET_TSC_URLTOURNAMENT	"http://localhost:1780/tourney.netgenie?mode=main"
#define GATEWAY_URLTOURNAMENT		MEMSET_TSC_URLTOURNAMENT //MEMSET_TPC_URLTOU


