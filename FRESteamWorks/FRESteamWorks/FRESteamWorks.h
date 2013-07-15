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
	RESPONSE_GetPublishedFileDetails,
	RESPONSE_UGCDownload,
	RESPONSE_UGCRead,
	RESPONSE_OnCommitPublishedFileUpdated,
	RESPONSE_OnDeletePublishedFile,
	RESPONSE_EnumerateUserPublishedFiles, 
	RESPONSE_EnumerateUserSubscribedFiles,
	RESPONSE_LeaderboardFindResult,
	RESPONSE_LeaderboardScoreUploaded,
	RESPONSE_LeaderboardScoresDownloaded
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
	RemoteStorageEnumerateWorkshopFilesResult_t *EnumerateWorkshopFilesResult;
	RemoteStorageEnumerateUserPublishedFilesResult_t *EnumerateUserPublishedFilesResult;
	RemoteStorageEnumerateUserSubscribedFilesResult_t *EnumerateUserSubscribedFilesResult;

	uint32 enumerateResultsReturned;

	//result from UGCDownload
	RemoteStorageDownloadUGCResult_t *UGCDownloadResult;

	//result from publishWorkshopFile
	PublishedFileId_t publishWorkshopFileResult;

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

	//leaderboards
	SteamLeaderboard_t leaderboardFindResult;

	void FindLeaderboard(const char* leaderboardName );
	void OnFindLeaderboard(LeaderboardFindResult_t *pCallback, bool bIOFailure );
	CCallResult<CSteam, LeaderboardFindResult_t> m_CallbackLeaderboardFind;

	LeaderboardScoreUploaded_t *leaderboardScoreUploadedResult;
	void UploadLeaderboardScore( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore, const int32 *pScoreDetails, int cScoreDetailsCount );
	void OnUploadLeaderboardScore(LeaderboardScoreUploaded_t *pCallback, bool bIOFailure );
	CCallResult<CSteam, LeaderboardScoreUploaded_t> m_CallbackUploadLeaderboardScore;
	
	LeaderboardScoresDownloaded_t *leaderboardScoresDownloadedResult;
	void DownloadLeaderboardEntries( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart, int nRangeEnd );
	void OnLeaderboardScoresDownloaded(LeaderboardScoresDownloaded_t *pCallback, bool bIOFailure );
	CCallResult<CSteam, LeaderboardScoresDownloaded_t> m_CallbackDownloadLeaderboardEntries;

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

	void EnumerateUserPublishedFiles(uint32 unStartIndex );
	void OnEnumerateUserPublishedFiles(RemoteStorageEnumerateUserPublishedFilesResult_t *pCallback,	bool bIOFailure );
	CCallResult<CSteam, RemoteStorageEnumerateUserPublishedFilesResult_t> m_CallbackEnumerateUserPublishedFiles;

	void EnumerateUserSubscribedFiles(uint32 unStartIndex );
	void OnEnumerateUserSubscribedFiles(RemoteStorageEnumerateUserSubscribedFilesResult_t *pCallback,	bool bIOFailure );
	CCallResult<CSteam, RemoteStorageEnumerateUserSubscribedFilesResult_t> m_CallbackEnumerateUserSubscribedFiles;

	void GetPublishedFileDetails(PublishedFileId_t unPublishedFileId);
	void OnGetPublishedFileDetails(RemoteStorageGetPublishedFileDetailsResult_t *pCallback, bool bIOFailure );
	CCallResult<CSteam, RemoteStorageGetPublishedFileDetailsResult_t> m_CallbackGetPublishedFileDetails;
	
	void UGCDownload(UGCHandle_t ugcHandle, uint32 unPriority);
	void OnUGCDownload(RemoteStorageDownloadUGCResult_t *pCallback, bool bIOFailure );
	CCallResult<CSteam, RemoteStorageDownloadUGCResult_t> m_CallbackUGCDownload;

	
	
	PublishedFileUpdateHandle_t CreatePublishedFileUpdateRequest( PublishedFileId_t unPublishedFileId );
	bool UpdatePublishedFileFile( PublishedFileUpdateHandle_t updateHandle, const char *pchFile );
	bool UpdatePublishedFilePreviewFile( PublishedFileUpdateHandle_t updateHandle, const char *pchPreviewFile );
	bool UpdatePublishedFileTitle( PublishedFileUpdateHandle_t updateHandle, const char *pchTitle );
	bool UpdatePublishedFileDescription( PublishedFileUpdateHandle_t updateHandle, const char *pchDescription ) ;
	bool UpdatePublishedFileVisibility( PublishedFileUpdateHandle_t updateHandle, ERemoteStoragePublishedFileVisibility eVisibility );
	bool UpdatePublishedFileTags( PublishedFileUpdateHandle_t updateHandle, SteamParamStringArray_t *pTags );

	void CommitPublishedFileUpdate(PublishedFileUpdateHandle_t updateHandle);
	void OnCommitPublishedFileUpdated(RemoteStorageUpdatePublishedFileResult_t *pCallback, bool bIOFailure );
	CCallResult<CSteam, RemoteStorageUpdatePublishedFileResult_t> m_CallbackCommitPublishedFileUpdate;

	void DeletePublishedFile(PublishedFileId_t unPublishedFileId );
	void OnDeletePublishedFile(RemoteStorageDeletePublishedFileResult_t *pCallback, bool bIOFailure );
	CCallResult<CSteam, RemoteStorageDeletePublishedFileResult_t> m_CallbackDeletePublishedFile;

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
	FREObject AIRSteam_GetEnumeratedWorkshopFilesResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetPublishedFileDetails(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetPublishedFileDetailsResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_UGCDownload(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetUGCDownloadResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_UGCRead(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_EnumerateUserPublishedFiles(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_EnumerateUserSubscribedFiles(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetEnumerateUserPublishedFilesResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetEnumerateUserSubscribedFilesResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	//update published file
	FREObject AIRSteam_CreatePublishedFileUpdateRequest(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_UpdatePublishedFileFile(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_UpdatePublishedFilePreviewFile(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_UpdatePublishedFileTitle(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_UpdatePublishedFileDescription(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_UpdatePublishedFileVisibility(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_UpdatePublishedFileTags(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_CommitPublishedFileUpdate(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_DeletePublishedFile(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetPublishWorkshopFileResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	//leaderboards
	FREObject AIRSteam_FindLeadboard(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetFindLeadboardResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_UploadLeaderboardScore(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetLeaderboardScoreUploadedResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_DownloadLeaderboardEntries(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_GetDownloadedLeaderboardEntryResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_IsSubscribedApp(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject AIRSteam_IsDlcInstalled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);

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
