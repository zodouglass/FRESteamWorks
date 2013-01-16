//
//  FRESteamWorks.h
//  FRESteamWorks
//
//  Created by David ´Oldes´ Oliva on 3/29/12.
//  Copyright (c) 2012 Amanita Design. All rights reserved.
//
#if defined(WIN32)
	#define _CRT_SECURE_NO_WARNINGS
	#define EXPORT __declspec( dllexport ) 
	#include <windows.h>
	//#include <stdio.h>
	//#include <stdlib.h>
	#include <string.h>
	#include "FlashRuntimeExtensions.h"
#else
	// Symbols tagged with EXPORT are externally visible.
	// Must use the -fvisibility=hidden gcc option.
	#define EXPORT __attribute__((visibility("default"))) 
	#include <Foundation/Foundation.h>
	#include <Adobe AIR/Adobe AIR.h>
#endif

#include <string>
#include <steam_api.h>
#include <ISteamRemoteStorage.h>

// General result codes
enum ResponseTypes
{
	RESPONSE_OnUserStatsReceived,
	RESPONSE_OnUserStatsStored,
	RESPONSE_OnAchievementStored,
	RESPONSE_OnUserFileShare,
	RESPONSE_OnPublishWorkshopFile,
	RESPONSE_EnumeratePublishedWorkshopFiles,
	RESPONSE_GetPublishedFileDetails
};
enum ResponseCodes
{
	RESPONSE_OK,
	RESPONSE_FAILED,
};


class CSteam
{
private:
	int64 m_iAppID; // Our current AppID
	bool m_bInitialized;
	
    
public:
	CSteam();
	~CSteam();

	//results from EnumeratePublishedWorkshopFiles
	int32 EnumeratedWorkshopFilesLength;
	uint64 EnumeratedWorkshopFiles[50];

	//results from GetPublishedFileDetails
	RemoteStorageGetPublishedFileDetailsResult_t *PublishedFileDetailsResult;
	//map<uint64, RemoteStorageGetPublishedFileDetailsResult_t> mapPublishedFileDetailsResult;

	bool RequestStats();
	bool SetAchievement( const char* ID );
	bool ClearAchievement( const char* ID );
	bool IsAchievement( const char* ID );
	bool GetStat( const char* ID, int32 *value );
	bool GetStat( const char* ID, float *value );
	bool SetStat( const char* ID, int32 value );
	bool SetStat( const char* ID, float value );
	bool StoreStats();
	bool ResetAllStats( bool bAchievementsToo );

	//workshop
	void FileShare( const char* fileName );
	void OnUserFileShare(RemoteStorageFileShareResult_t *pCallback,	bool bIOFailure );
	CCallResult<CSteam, RemoteStorageFileShareResult_t> m_CallbackUserFileShare;

	void PublishWorkshopFile( const char* fileName, const char* previewFile,  const char* title,  const char* description,  const char* longdescription, ERemoteStoragePublishedFileVisibility visiblity, SteamParamStringArray_t *pTags, EWorkshopFileType type);
	void OnPublishWorkshopFile(RemoteStoragePublishFileResult_t *pCallback,	bool bIOFailure );
	CCallResult<CSteam, RemoteStoragePublishFileResult_t> m_CallbackUserPublishWorkshopFile;

	void EnumeratePublishedWorkshopFiles( EWorkshopEnumerationType eEnumerationType, uint32 unStartIndex, uint32 unCount, uint32 unDays, SteamParamStringArray_t *pTags, SteamParamStringArray_t *pUserTags );
	void OnEnumeratePublishedWorkshopFiles(RemoteStorageEnumerateWorkshopFilesResult_t *pCallback,	bool bIOFailure );
	CCallResult<CSteam, RemoteStorageEnumerateWorkshopFilesResult_t> m_CallbackEnumeratePublishedWorkshopFiles;

	void GetPublishedFileDetails(PublishedFileId_t unPublishedFileId);
	void OnGetPublishedFileDetails(RemoteStorageGetPublishedFileDetailsResult_t *pCallback, bool bIOFailure );
	CCallResult<CSteam, RemoteStorageGetPublishedFileDetailsResult_t> m_CallbackGetPublishedFileDetails;
	//end workshop

	void DispatchEvent( const int req_type, const int response );
    
	STEAM_CALLBACK( CSteam, OnUserStatsReceived,	UserStatsReceived_t,	m_CallbackUserStatsReceived );
	STEAM_CALLBACK( CSteam, OnUserStatsStored,		UserStatsStored_t,		m_CallbackUserStatsStored );
	STEAM_CALLBACK( CSteam, OnAchievementStored,	UserAchievementStored_t, m_CallbackAchievementStored );
};

extern "C" {
	FREObject AIRSteam_Init(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_RequestStats(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_SetAchievement(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_ClearAchievement(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_IsAchievement(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_RunCallbacks(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetStatInt(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetStatFloat(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_SetStatInt(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_SetStatFloat(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_StoreStats(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_ResetAllStats(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	//Steam Cloud
	FREObject AIRSteam_GetFileCount(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetFileSize(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_FileExists(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_FileWrite(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_FileRead(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_FileDelete(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_IsCloudEnabledForApp(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_SetCloudEnabledForApp(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	//Steam Workshop
	FREObject AIRSteam_FileShare(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_PublishWorkshopFile(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_EnumeratePublishedWorkshopFiles(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetEnumeratedWorkshopFiles(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetEnumeratedWorkshopFilesLength(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetPublishedFileDetails(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetPublishedFileDetailsResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);


	FREObject UInt64ToFREObject( uint64 value);
	uint64 FREObjectToUint64( FREObject valueString );

    // A native context instance is created
    void ContextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, 
                            uint32_t* numFunctions, const FRENamedFunction** functions);
    void ContextFinalizer(FREContext ctx);
    EXPORT void ExtInitializer(void** extDataToSet, FREContextInitializer* ctxInitializerToSet, 
                               FREContextFinalizer* ctxFinalizerToSet);
    EXPORT void ExtFinalizer(void* extData);
}
