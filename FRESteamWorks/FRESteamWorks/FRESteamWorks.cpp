//
//  FRESteamWorks.cpp
//  FRESteamWorks
//
//  Created by David `Oldes` Oliva on 3/29/12.
//  Copyright (c) 2012 Amanita Design. All rights reserved.
//

#include "FRESteamWorks.h"


FREContext  AIRContext; // Used to dispatch event back to AIR

// Global access to Steam object
CSteam*	g_Steam = NULL;

CSteam::CSteam(): 				
m_iAppID( 0 ),
m_bInitialized( false ),
m_CallbackUserStatsReceived( this, &CSteam::OnUserStatsReceived ),
m_CallbackUserStatsStored( this, &CSteam::OnUserStatsStored ),
m_CallbackAchievementStored( this, &CSteam::OnAchievementStored )
{
    m_iAppID = SteamUtils()->GetAppID();
    m_bInitialized = !( NULL == SteamUserStats() || NULL == SteamUser() );
    RequestStats();
}

bool CSteam::RequestStats() {
	// Is Steam loaded? If not we can't get stats.
	if ( NULL == SteamUserStats() || NULL == SteamUser() ) {
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if ( !SteamUser()->BLoggedOn() ) {
		return false;
	}
	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}

bool CSteam::SetAchievement(const char* ID) {
	bool result = false;
	if (m_bInitialized) {
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	return result;
}

bool CSteam::ClearAchievement(const char* ID) {
	bool result = false;
	if (m_bInitialized) {
		SteamUserStats()->ClearAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	return result;
}

bool CSteam::IsAchievement(const char* ID) {
	bool result;
	if (m_bInitialized) SteamUserStats()->GetAchievement(ID, &result);
	return result;
}

bool CSteam::GetStat(const char* ID, int32 *value) {
	bool result;
	if (m_bInitialized) result = SteamUserStats()->GetStat(ID, value);
	return result;
}
bool CSteam::GetStat(const char* ID, float *value) {
	bool result;
	if (m_bInitialized) result = SteamUserStats()->GetStat(ID, value);
	return result;
}


bool CSteam::SetStat(const char* ID, int32 value) {
	bool result;
	if (m_bInitialized) result = SteamUserStats()->SetStat(ID, value);
	return result;
}
bool CSteam::SetStat(const char* ID, float value) {
	bool result;
	if (m_bInitialized) result = SteamUserStats()->SetStat(ID, value);
	return result;
}

bool CSteam::StoreStats() {
	bool result;
	if (m_bInitialized) result = SteamUserStats()->StoreStats();
	return result;
}

bool CSteam::ResetAllStats( bool bAchievementsToo ) {
	if (m_bInitialized) {
		SteamUserStats()->ResetAllStats(bAchievementsToo);
		return SteamUserStats()->StoreStats();
	}
	return false;
}


void CSteam::FindLeaderboard( const char* leaderboardName ) {

	SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard((char *)leaderboardName);
	//SteamAPICall_t hSteamAPICall = SteamUserStats()->FindOrCreateLeaderboard((char *)leaderboardName, k_ELeaderboardSortMethodAscending, k_ELeaderboardDisplayTypeNumeric );
	m_CallbackLeaderboardFind.Set( hSteamAPICall, this, &CSteam::OnFindLeaderboard );
}

void CSteam::UploadLeaderboardScore( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore, const int32 *pScoreDetails, int cScoreDetailsCount )
{
	SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(hSteamLeaderboard, eLeaderboardUploadScoreMethod,nScore,pScoreDetails,cScoreDetailsCount);
	m_CallbackUploadLeaderboardScore.Set( hSteamAPICall, this, &CSteam::OnUploadLeaderboardScore );
}

void CSteam::DownloadLeaderboardEntries( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart, int nRangeEnd )
{
	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(hSteamLeaderboard, eLeaderboardDataRequest,nRangeStart,nRangeEnd);
	m_CallbackDownloadLeaderboardEntries.Set( hSteamAPICall, this, &CSteam::OnLeaderboardScoresDownloaded );
}

void CSteam::FileShare( const char* fileName ) {

	SteamAPICall_t hSteamAPICall = SteamRemoteStorage()->FileShare((char *)fileName);
	m_CallbackUserFileShare.Set( hSteamAPICall, this, &CSteam::OnUserFileShare );
}

void CSteam::EnumeratePublishedWorkshopFiles( EWorkshopEnumerationType eEnumerationType, uint32 unStartIndex, uint32 unCount, uint32 unDays, SteamParamStringArray_t *pTags, SteamParamStringArray_t *pUserTags ) {
	//TODO - get tags
	SteamAPICall_t hSteamAPICall = SteamRemoteStorage()->EnumeratePublishedWorkshopFiles(eEnumerationType, unStartIndex, unCount, unDays, pTags, pUserTags);
	m_CallbackEnumeratePublishedWorkshopFiles.Set( hSteamAPICall, this, &CSteam::OnEnumeratePublishedWorkshopFiles );
}

void CSteam::EnumerateUserPublishedFiles( uint32 unStartIndex) {
	SteamAPICall_t hSteamAPICall = SteamRemoteStorage()->EnumerateUserPublishedFiles(unStartIndex);
	m_CallbackEnumerateUserPublishedFiles.Set( hSteamAPICall, this, &CSteam::OnEnumerateUserPublishedFiles );
}

void CSteam::EnumerateUserSubscribedFiles( uint32 unStartIndex) {
	SteamAPICall_t hSteamAPICall = SteamRemoteStorage()->EnumerateUserSubscribedFiles(unStartIndex);
	m_CallbackEnumerateUserSubscribedFiles.Set( hSteamAPICall, this, &CSteam::OnEnumerateUserSubscribedFiles );
}

void CSteam::PublishWorkshopFile( const char* fileName, const char* previewFile,  const char* title,  const char* description,  const char* longdescription, ERemoteStoragePublishedFileVisibility visiblity, SteamParamStringArray_t *pTags, EWorkshopFileType type) {
	//TODO - get tags
	SteamAPICall_t hSteamAPICall = SteamRemoteStorage()->PublishWorkshopFile((char *)fileName, (char *)previewFile, m_iAppID, (char *)title, (char *)description, visiblity, pTags, type);
	m_CallbackUserPublishWorkshopFile.Set( hSteamAPICall, this, &CSteam::OnPublishWorkshopFile );
}

void CSteam::GetPublishedFileDetails(PublishedFileId_t unPublishedFileId) {
	SteamAPICall_t hSteamAPICall = SteamRemoteStorage()->GetPublishedFileDetails(unPublishedFileId);
	m_CallbackGetPublishedFileDetails.Set( hSteamAPICall, this, &CSteam::OnGetPublishedFileDetails );
}

void CSteam::UGCDownload(UGCHandle_t ugcHandle, uint32 unPriority) {
	SteamAPICall_t hSteamAPICall = SteamRemoteStorage()->UGCDownload(ugcHandle, unPriority);
	m_CallbackUGCDownload.Set( hSteamAPICall, this, &CSteam::OnUGCDownload );
}


PublishedFileUpdateHandle_t CSteam::CreatePublishedFileUpdateRequest( PublishedFileId_t unPublishedFileId ){
	return SteamRemoteStorage()->CreatePublishedFileUpdateRequest(unPublishedFileId);
}

bool CSteam::UpdatePublishedFileFile( PublishedFileUpdateHandle_t updateHandle, const char *pchFile ){
	return SteamRemoteStorage()->UpdatePublishedFileFile(updateHandle, pchFile );
}

bool CSteam::UpdatePublishedFilePreviewFile( PublishedFileUpdateHandle_t updateHandle, const char *pchPreviewFile ){
	return SteamRemoteStorage()->UpdatePublishedFilePreviewFile(updateHandle, pchPreviewFile );
}
	
bool CSteam::UpdatePublishedFileTitle( PublishedFileUpdateHandle_t updateHandle, const char *pchTitle ){
	return SteamRemoteStorage()->UpdatePublishedFileTitle(updateHandle, pchTitle );
}
	
bool CSteam::UpdatePublishedFileDescription( PublishedFileUpdateHandle_t updateHandle, const char *pchDescription ) {
	return SteamRemoteStorage()->UpdatePublishedFileDescription(updateHandle, pchDescription );
}
	
bool CSteam::UpdatePublishedFileVisibility( PublishedFileUpdateHandle_t updateHandle, ERemoteStoragePublishedFileVisibility eVisibility ){
	return SteamRemoteStorage()->UpdatePublishedFileVisibility(updateHandle, eVisibility );
}
	
bool CSteam::UpdatePublishedFileTags( PublishedFileUpdateHandle_t updateHandle, SteamParamStringArray_t *pTags ){
	return SteamRemoteStorage()->UpdatePublishedFileTags(updateHandle, pTags );
}

void CSteam::CommitPublishedFileUpdate(PublishedFileUpdateHandle_t updateHandle){
	SteamAPICall_t hSteamAPICall = SteamRemoteStorage()->CommitPublishedFileUpdate(updateHandle);
	m_CallbackCommitPublishedFileUpdate.Set( hSteamAPICall, this, &CSteam::OnCommitPublishedFileUpdated );
}
	
void CSteam::DeletePublishedFile(PublishedFileId_t unPublishedFileId ){
	SteamAPICall_t hSteamAPICall = SteamRemoteStorage()->DeletePublishedFile(unPublishedFileId);
	m_CallbackDeletePublishedFile.Set( hSteamAPICall, this, &CSteam::OnDeletePublishedFile );
}

void CSteam::DispatchEvent(const int req_type, const int response) {
	FREResult res;
	char code[5];
	char level[5];
    
	sprintf(code,  "%d", req_type);
	sprintf(level, "%d", response);
	if((res=FREDispatchStatusEventAsync(AIRContext, (const uint8_t*)code, (const uint8_t*)level)) != FRE_OK)
	{
		//Debug::logDebug("ERROR: FREDispatchStatusEventAsync(ctx, (const uint8_t*)code, (const uint8_t*)level) = %d", res);
		return;
	}
}

//called when SteamRemoteStorate->EnumeratePublishedWorkshopFiles() returns asynchronously
void CSteam::OnEnumeratePublishedWorkshopFiles( RemoteStorageEnumerateWorkshopFilesResult_t *pCallback, bool bIOFailure ) {
	//save the values returned to be retrived by the AS portation
	EnumerateWorkshopFilesResult = pCallback;
	enumerateResultsReturned = pCallback->m_nResultsReturned; 
	g_Steam->DispatchEvent(RESPONSE_EnumeratePublishedWorkshopFiles, pCallback->m_eResult);
}

//called when SteamRemoteStorate->EnumerateUserPublishedFiles() returns asynchronously
void CSteam::OnEnumerateUserPublishedFiles( RemoteStorageEnumerateUserPublishedFilesResult_t *pCallback, bool bIOFailure ) {
	//save the values returned to be retrived by the AS portation
	EnumerateUserPublishedFilesResult = pCallback;
	enumerateResultsReturned = pCallback->m_nResultsReturned; 
	g_Steam->DispatchEvent(RESPONSE_EnumerateUserPublishedFiles, pCallback->m_eResult);
}

//called when SteamRemoteStorate->EnumerateUserSubscribedFiles() returns asynchronously
void CSteam::OnEnumerateUserSubscribedFiles( RemoteStorageEnumerateUserSubscribedFilesResult_t *pCallback, bool bIOFailure ) {
	//save the values returned to be retrived by the AS portation
	EnumerateUserSubscribedFilesResult = pCallback;
	enumerateResultsReturned = pCallback->m_nResultsReturned; 
	g_Steam->DispatchEvent(RESPONSE_EnumerateUserSubscribedFiles, pCallback->m_eResult);
}

//called when SteamUserStats->FindLeaderboard() returns asynchronously

void CSteam::OnFindLeaderboard( LeaderboardFindResult_t *pCallback, bool bIOFailure ) {
	leaderboardFindResult = pCallback->m_hSteamLeaderboard;
	g_Steam->DispatchEvent(RESPONSE_LeaderboardFindResult, 1);
}

//called when SteamUserStats->UploadLeaderboardScore() returns asynchronously
void CSteam::OnUploadLeaderboardScore( LeaderboardScoreUploaded_t *pCallback, bool bIOFailure ) {
	leaderboardScoreUploadedResult = pCallback;
	g_Steam->DispatchEvent(RESPONSE_LeaderboardScoreUploaded, 1);
}

void CSteam::OnLeaderboardScoresDownloaded( LeaderboardScoresDownloaded_t *pCallback, bool bIOFailure ) {
	leaderboardScoresDownloadedResult = pCallback;
	g_Steam->DispatchEvent(RESPONSE_LeaderboardScoresDownloaded, 1);
}
//called when SteamRemoteStorate->FileShare() returns asynchronously

void CSteam::OnUserFileShare( RemoteStorageFileShareResult_t *pCallback, bool bIOFailure ) {
	g_Steam->DispatchEvent(RESPONSE_OnUserFileShare, pCallback->m_eResult);
}

//called when SteamRemoteStorate->PublishWorkshopFile() returns asynchronously
void CSteam::OnPublishWorkshopFile( RemoteStoragePublishFileResult_t *pCallback, bool bIOFailure ) {
	publishWorkshopFileResult = pCallback->m_nPublishedFileId;
	g_Steam->DispatchEvent(RESPONSE_OnPublishWorkshopFile, pCallback->m_eResult);
}

void CSteam::OnGetPublishedFileDetails( RemoteStorageGetPublishedFileDetailsResult_t *pCallback, bool bIOFailure ) {
	//save the result so the native AS3 can call GetPublishedFileDetailsResult from the listener of the status event
	PublishedFileDetailsResult = pCallback;
	//mapPublishedFileDetailsResult.insert(make_pair(pCallback->m_nPublishedFileId, *pCallback) );
	g_Steam->DispatchEvent(RESPONSE_GetPublishedFileDetails, pCallback->m_eResult);
}

void CSteam::OnUGCDownload(RemoteStorageDownloadUGCResult_t *pCallback, bool bIOFailure ){
	//save the result
	UGCDownloadResult = pCallback;
	g_Steam->DispatchEvent(RESPONSE_UGCDownload, pCallback->m_eResult);
}

void CSteam::OnUserStatsReceived( UserStatsReceived_t *pCallback ) {
	// we may get callbacks for other games' stats arriving, ignore them
	if ( m_iAppID == pCallback->m_nGameID )	{
		g_Steam->DispatchEvent(RESPONSE_OnUserStatsReceived, pCallback->m_eResult);
	}
}


void CSteam::OnUserStatsStored( UserStatsStored_t *pCallback ) {
	// we may get callbacks for other games' stats arriving, ignore them
	if ( m_iAppID == pCallback->m_nGameID )	{
		DispatchEvent(RESPONSE_OnUserStatsStored, pCallback->m_eResult);
	}
}


void CSteam::OnAchievementStored( UserAchievementStored_t *pCallback ) {
    // we may get callbacks for other games' stats arriving, ignore them
    if ( m_iAppID == pCallback->m_nGameID ) {
        g_Steam->DispatchEvent(RESPONSE_OnAchievementStored, RESPONSE_OK);
    }
}

void CSteam::OnCommitPublishedFileUpdated(RemoteStorageUpdatePublishedFileResult_t *pCallback, bool bIOFailure ) {
	g_Steam->DispatchEvent(RESPONSE_OnCommitPublishedFileUpdated, pCallback->m_eResult);
}

void CSteam::OnDeletePublishedFile(RemoteStorageDeletePublishedFileResult_t *pCallback, bool bIOFailure ) {
	g_Steam->DispatchEvent(RESPONSE_OnDeletePublishedFile, pCallback->m_eResult);
}

extern "C" {
    
    FREObject AIRSteam_Init(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		bool bRet;
        
		if (g_Steam == NULL) {
			bRet = SteamAPI_Init();
			if (bRet)
				g_Steam = new CSteam();
		} else {
			bRet = true;
		}
		FRENewObjectFromBool((uint32_t)bRet, &result);
		return result;
	}
    
	FREObject AIRSteam_RequestStats(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam) {
			FRENewObjectFromBool((uint32_t)g_Steam->RequestStats(), &result);
		} else {
			FRENewObjectFromBool(false, &result);
		}
		SteamAPI_RunCallbacks();
		return result;
	}
    
	FREObject AIRSteam_SetAchievement(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam && argc == 1) {
			FREResult res;
			uint32_t len = -1;
			const uint8_t *ID = 0;
			if((res=FREGetObjectAsUTF8(argv[0], &len, &ID)) == FRE_OK) {
				FRENewObjectFromBool((uint32_t)g_Steam->SetAchievement((const char *)ID), &result);
			}
		}
		if (NULL == result) FRENewObjectFromBool(false, &result);
		SteamAPI_RunCallbacks();
		return result;
	}
	
	FREObject AIRSteam_ClearAchievement(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam) {
			FREResult res;
			uint32_t len = -1;
			const uint8_t *ID = 0;
			if((res=FREGetObjectAsUTF8(argv[0], &len, &ID)) == FRE_OK) {
				FRENewObjectFromBool((uint32_t)g_Steam->ClearAchievement((const char *)ID), &result);
			}
		}
		if (NULL == result) FRENewObjectFromBool(false, &result);
		return result;
	}
    
	FREObject AIRSteam_IsAchievement(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam && argc == 1) {
			FREResult res;
			uint32_t len = -1;
			const uint8_t *ID = 0;
			if((res=FREGetObjectAsUTF8(argv[0], &len, &ID)) == FRE_OK) {
				FRENewObjectFromBool((uint32_t)g_Steam->IsAchievement((const char *)ID), &result);
			}
		}
		if (NULL == result) FRENewObjectFromBool(false, &result);
		return result;
	}
    
	FREObject AIRSteam_RunCallbacks(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		SteamAPI_RunCallbacks();
		return NULL;
	}
    
	FREObject AIRSteam_GetStatInt(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam) {
			FREResult res;
			uint32_t len = -1;
			const uint8_t *ID = 0;
			int32 value;
			if((res=FREGetObjectAsUTF8(argv[0], &len, &ID)) == FRE_OK) {
				g_Steam->GetStat((const char *)ID, &value);
				FRENewObjectFromInt32(value, &result);
			}
		}
		return result;
	}
    
	FREObject AIRSteam_GetStatFloat(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam) {
			FREResult res;
			uint32_t len = -1;
			const uint8_t *ID = 0;
			float value;
			if((res=FREGetObjectAsUTF8(argv[0], &len, &ID)) == FRE_OK) {
				g_Steam->GetStat((const char *)ID, &value);
				FRENewObjectFromDouble(value, &result);
			}
		}
		return result;
	}
    
	FREObject AIRSteam_SetStatInt(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam && argc == 2) {
			uint32_t len = -1;
			const uint8_t *ID = 0;
			int32 value;
			if(
               FREGetObjectAsUTF8(argv[0], &len, &ID) == FRE_OK
               && FREGetObjectAsInt32(argv[1], &value) == FRE_OK
               ) {
				FRENewObjectFromBool((uint32_t)g_Steam->SetStat((const char *)ID, value), &result);
			}
		}
		return result;
	}
	FREObject AIRSteam_SetStatFloat(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam && argc == 2) {
			uint32_t len = -1;
			const uint8_t *ID = 0;
			double value;
			if(
               FREGetObjectAsUTF8(argv[0], &len, &ID) == FRE_OK
               && FREGetObjectAsDouble(argv[1], &value) == FRE_OK
               ) {
				FRENewObjectFromBool((uint32_t)g_Steam->SetStat((const char *)ID, (float)value), &result);
			}
		}
		return result;
	}
    
	FREObject AIRSteam_StoreStats(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam) {
			FRENewObjectFromBool((uint32_t)g_Steam->StoreStats(), &result);
		} else {
			FRENewObjectFromBool(false, &result);
		}
		return result;
	}
    
	FREObject AIRSteam_ResetAllStats(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam && argc == 1) {
			uint32_t bAchievementsToo;
			if( FREGetObjectAsBool( argv[0], &bAchievementsToo ) == FRE_OK ) {
				FRENewObjectFromBool((uint32_t)g_Steam->ResetAllStats((bAchievementsToo!=0)), &result);
			}
		}
		if ( NULL == result ) FRENewObjectFromBool(false, &result);
		return result;
	}
	
	//Steam Cloud
	FREObject AIRSteam_GetFileCount(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		int32 count;
		if (g_Steam) {
			count = SteamRemoteStorage()->GetFileCount();
		}
		FRENewObjectFromInt32((uint32_t)count, &result);
		return result;
	}
    
	FREObject AIRSteam_GetFileSize(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		int32 fileSize = 0;
		if (g_Steam && argc==1) {
			uint32_t len = -1;
			const uint8_t *fileName = 0;
			if(	FREGetObjectAsUTF8(argv[0], &len, &fileName) == FRE_OK ) {
				fileSize = SteamRemoteStorage()->GetFileSize((char *)fileName);
			}
		}
		FRENewObjectFromInt32((uint32_t)fileSize, &result);
		return result;
	}
    
	FREObject AIRSteam_FileExists(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		bool fileExists = 0;
		if (g_Steam && argc==1) {
			uint32_t len = -1;
			const uint8_t *fileName = 0;
			if(	FREGetObjectAsUTF8(argv[0], &len, &fileName) == FRE_OK ) {
				fileExists = SteamRemoteStorage()->FileExists((char *)fileName);
			}
		}
		FRENewObjectFromBool((uint32_t)fileExists, &result);
		return result;
	}

	FREObject AIRSteam_FileWrite(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		FREByteArray byteArray;
		bool retVal=0;
        
		if (g_Steam && argc==2) {
            
			uint32_t len = -1;
			const uint8_t *fileName = 0;
			if(
               FREGetObjectAsUTF8(argv[0], &len, &fileName) == FRE_OK
               && FREAcquireByteArray(argv[1], &byteArray) == FRE_OK
               ) {
				retVal = SteamRemoteStorage()->FileWrite((char *)fileName, byteArray.bytes, byteArray.length);
				FREReleaseByteArray(argv[1]); 
			}
		}
		FRENewObjectFromBool((uint32_t)retVal, &result);
		return result;
	}
    
	FREObject AIRSteam_FileRead(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		FREByteArray byteArray;
		int32 retVal=0;
        
		if (g_Steam && argc==2) {
            
			uint32_t len = -1;
			const uint8_t *fileName = 0;
			uint32 fileSize;
			char *dataIn;
			if(
               FREGetObjectAsUTF8(argv[0], &len, &fileName) == FRE_OK
               && FREAcquireByteArray(argv[1], &byteArray) == FRE_OK
               ) {
				fileSize = SteamRemoteStorage()->GetFileSize((char *)fileName);
				if(fileSize > 0 && fileSize <= byteArray.length) {
					dataIn = (char *)malloc(fileSize);
					retVal = SteamRemoteStorage()->FileRead((char *)fileName, dataIn, fileSize);
					memcpy (byteArray.bytes, dataIn, fileSize); 
					free(dataIn);
				}
				FREReleaseByteArray(argv[1]);
			}
		}
		FRENewObjectFromBool((uint32_t)retVal, &result);
		return result;
	}
    
	FREObject AIRSteam_FileDelete(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		bool retVal = 0;
		if (g_Steam && argc==1) {
			uint32_t len = -1;
			const uint8_t *fileName = 0;
			if(	FREGetObjectAsUTF8(argv[0], &len, &fileName) == FRE_OK ) {
				retVal = SteamRemoteStorage()->FileDelete((char *)fileName);
			}
		}
		FRENewObjectFromBool((uint32_t)retVal, &result);
		return result;
	}
    
	FREObject AIRSteam_IsCloudEnabledForApp(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		bool retVal = 0;
		if (g_Steam) {
			retVal = SteamRemoteStorage()->IsCloudEnabledForApp();
		}
		FRENewObjectFromBool((uint32_t)retVal, &result);
		return result;
	}
    
	FREObject AIRSteam_SetCloudEnabledForApp(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		bool retVal = 0;
		if (g_Steam && argc==1) {
			uint32_t enabled = 0;
			bool bEnabled;
			if(	FREGetObjectAsBool(argv[0], &enabled) == FRE_OK ) {
				bEnabled = (enabled!=0);
				SteamRemoteStorage()->SetCloudEnabledForApp(bEnabled);
				retVal = (bEnabled == SteamRemoteStorage()->IsCloudEnabledForApp());
			}
		}
		FRENewObjectFromBool((uint32_t)retVal, &result);
		return result;

	}

	//leaderboards

	FREObject AIRSteam_FindLeadboard(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;
		uint32_t len = -1;
		const uint8_t *leaderboardName = 0;

		if (g_Steam  && argc==1 && FREGetObjectAsUTF8(argv[0], &len, &leaderboardName) == FRE_OK) {
			g_Steam->FindLeaderboard((char *)leaderboardName);
			FRENewObjectFromBool(true , &result);
		} else {
			FRENewObjectFromBool(false, &result);
		}

		return result;
	}

	FREObject AIRSteam_GetFindLeadboardResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;
		if (g_Steam )
		{
			result = UInt64ToFREObject(g_Steam->leaderboardFindResult);
		}
		return result;
	}

	FREObject AIRSteam_DownloadLeaderboardEntries(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;

		uint64 hSteamLeaderboard = 0;
		uint32_t eLeaderboardDataRequest = 0;
		uint32_t nRangeStart = 0;
		uint32_t nRangeEnd = 0;

		int32 *scoreDetails = 0;
		int scoreDetailsCount = 0;

		
		if (g_Steam && argc==4	&& FREGetObjectAsUint32(argv[1], &eLeaderboardDataRequest) == FRE_OK
								&& FREGetObjectAsUint32(argv[2], &nRangeStart) == FRE_OK 
								&& FREGetObjectAsUint32(argv[3], &nRangeEnd) == FRE_OK ) 
		{
			hSteamLeaderboard = FREObjectToUint64(argv[0]);

			g_Steam->DownloadLeaderboardEntries(hSteamLeaderboard, (ELeaderboardDataRequest)eLeaderboardDataRequest, nRangeStart, nRangeEnd);
			FRENewObjectFromBool(true , &result);
		} else {
			FRENewObjectFromBool(false, &result);
		}

		return result;
	}

	
	FREObject AIRSteam_GetDownloadedLeaderboardEntryResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;

		if (g_Steam )
		{
			LeaderboardScoresDownloaded_t leaderboardScoresDownloaded_t = *g_Steam->leaderboardScoresDownloadedResult;

			FREObject leaderboardResults;
			FRENewObject((const uint8_t*)"Array", 0, NULL, &leaderboardResults, NULL );
			FRESetArrayLength( leaderboardResults, leaderboardScoresDownloaded_t.m_cEntryCount );

			/* get entries and return an array of objects */
			

			for ( int index = 0; index < leaderboardScoresDownloaded_t.m_cEntryCount; index++ )
			{
				LeaderboardEntry_t leaderboardEntry;
				int32 details[3];		// TODO - add support for details
				SteamUserStats()->GetDownloadedLeaderboardEntry( 
					leaderboardScoresDownloaded_t.m_hSteamLeaderboardEntries, 
					index, &leaderboardEntry, details, 0 ); //TODO - swap NULL for details

				FREObject entry;
				FRENewObject((const uint8_t*)"Object", 0, NULL, &entry,NULL);

				FREObject m_steamIDUser = UInt64ToFREObject(leaderboardEntry.m_steamIDUser.ConvertToUint64()); //user id

				const char *pchName = SteamFriends()->GetFriendPersonaName( leaderboardEntry.m_steamIDUser ); //user name
				FREObject steamName;
				uint32_t len = -1;
				FRENewObjectFromUTF8(len, (const uint8_t *)pchName, &steamName);
				//FREObject m_hUGC = UInt64ToFREObject(leaderboardEntry.m_hUGC);
				FREObject m_nGlobalRank;
				FREObject m_nScore;
				//FREObject m_cDetails;

				FRENewObjectFromInt32(leaderboardEntry.m_nGlobalRank, &m_nGlobalRank);
				FRENewObjectFromInt32(leaderboardEntry.m_nScore, &m_nScore);
				//FRENewObjectFromInt32(leaderboardEntry.m_cDetails, &m_cDetails);

				FRESetObjectProperty(entry, (const uint8_t*)"personaName", steamName, NULL);
				FRESetObjectProperty(entry, (const uint8_t*)"steamIDUser", m_steamIDUser, NULL);
				//FRESetObjectProperty(entry, (const uint8_t*)"m_hUGC", m_hUGC, NULL);
				FRESetObjectProperty(entry, (const uint8_t*)"globalRank", m_nGlobalRank, NULL);
				FRESetObjectProperty(entry, (const uint8_t*)"score", m_nScore, NULL);
				//FRESetObjectProperty(entry, (const uint8_t*)"m_cDetails", m_cDetails, NULL);

				//push to result array
				FRESetArrayElementAt( leaderboardResults, index, entry );

			}
			

			/* return the result object 

			FRENewObject((const uint8_t*)"Object", 0, NULL, &result,NULL);
			
			FREObject m_hSteamLeaderboard = UInt64ToFREObject(leaderboardScoresDownloaded_t.m_hSteamLeaderboard);
			FREObject m_hSteamLeaderboardEntries = UInt64ToFREObject(leaderboardScoresDownloaded_t.m_hSteamLeaderboardEntries);
			FREObject m_cEntryCount;
			FRENewObjectFromInt32(leaderboardScoresDownloaded_t.m_cEntryCount, &m_cEntryCount);


			// fill properties of FREObject result
			FRESetObjectProperty(result, (const uint8_t*)"m_hSteamLeaderboard", m_hSteamLeaderboard, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"m_hSteamLeaderboardEntries", m_hSteamLeaderboardEntries, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"m_cEntryCount", m_cEntryCount, NULL);
			*/

			FREObject m_cEntryCount;
			FRENewObjectFromInt32(leaderboardScoresDownloaded_t.m_cEntryCount, &m_cEntryCount);
			FRENewObject((const uint8_t*)"Object", 0, NULL, &result,NULL);
			FRESetObjectProperty(result, (const uint8_t*)"m_cEntryCount", m_cEntryCount, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"entries", leaderboardResults, NULL);
		}


		
		return result;
	}

	FREObject AIRSteam_UploadLeaderboardScore(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;

		uint64 leaderboardHandle = 0;
		uint32_t score = 0;
		uint32_t leaderboardUploadScoreMethod = 0;

		int32 *scoreDetails = 0;
		int scoreDetailsCount = 0;

		
		if (g_Steam && argc==5 && FREGetObjectAsUint32(argv[1], &score) == FRE_OK
								&& FREGetObjectAsUint32(argv[2], &leaderboardUploadScoreMethod) == FRE_OK ) 
		{
			leaderboardHandle = FREObjectToUint64(argv[0]);

			g_Steam->UploadLeaderboardScore(leaderboardHandle, (ELeaderboardUploadScoreMethod)leaderboardUploadScoreMethod, score, scoreDetails, scoreDetailsCount);
			FRENewObjectFromBool(true , &result);
		} else {
			FRENewObjectFromBool(false, &result);
		}

		return result;
	}

	FREObject AIRSteam_GetLeaderboardScoreUploadedResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;

		if (g_Steam )
		{
			LeaderboardScoreUploaded_t leaderboardScoreUploaded_t = *g_Steam->leaderboardScoreUploadedResult;

			FRENewObject((const uint8_t*)"Object", 0, NULL, &result,NULL);

			FREObject m_bSuccess;
			FREObject m_hSteamLeaderboard = UInt64ToFREObject(leaderboardScoreUploaded_t.m_hSteamLeaderboard);
			FREObject m_nScore;
			FREObject m_bScoreChanged;
			FREObject m_nGlobalRankNew;
			FREObject m_nGlobalRankPrevious;
			
			FRENewObjectFromBool(leaderboardScoreUploaded_t.m_bSuccess, &m_bSuccess);
			FRENewObjectFromBool(leaderboardScoreUploaded_t.m_bScoreChanged, &m_bScoreChanged);
			FRENewObjectFromInt32(leaderboardScoreUploaded_t.m_nScore, &m_nScore);
			FRENewObjectFromInt32(leaderboardScoreUploaded_t.m_nGlobalRankNew, &m_nGlobalRankNew);
			FRENewObjectFromInt32(leaderboardScoreUploaded_t.m_nGlobalRankPrevious, &m_nGlobalRankPrevious);


			// fill properties of FREObject result
			FRESetObjectProperty(result, (const uint8_t*)"success", m_bSuccess, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"scoreChanged", m_bScoreChanged, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"steamLeaderboard", m_hSteamLeaderboard, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"score", m_nScore, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"globalRankNew", m_nGlobalRankNew, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"globalRankPrevious", m_nGlobalRankPrevious, NULL);
		}
		
		return result;
	}

	//DLC
	
	FREObject AIRSteam_IsSubscribedApp(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;

		if (g_Steam && argc==1 )
		{
			uint64 dlcAppId = FREObjectToUint64(argv[0]);

			bool isSubscribed = SteamApps()->BIsSubscribedApp( (AppId_t)dlcAppId );

			FRENewObjectFromBool(isSubscribed, &result);
		}

		return result;
	}
	
	FREObject AIRSteam_IsDlcInstalled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;
		
		if (g_Steam && argc==1 )
		{
			uint64 dlcAppId = FREObjectToUint64(argv[0]);

			bool isDlcInstalled = SteamApps()->BIsDlcInstalled( (AppId_t)dlcAppId );

			FRENewObjectFromBool(isDlcInstalled, &result);
		}
		return result;
	}


	//Steam Workshop
	
	FREObject AIRSteam_FileShare(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		const uint8_t *fileName = 0;

		if (g_Steam  && argc==1 && FREGetObjectAsUTF8(argv[0], &len, &fileName) == FRE_OK) {
			g_Steam->FileShare((char *)fileName);
			FRENewObjectFromBool(true , &result);
		} else {
			FRENewObjectFromBool(false, &result);
		}
		SteamAPI_RunCallbacks();
		return result;
	}

	FREObject AIRSteam_PublishWorkshopFile(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		const uint8_t *fileName = 0;
		const uint8_t *previewFile = 0;
		const uint8_t *title = 0;
		const uint8_t *description = 0;
		const uint8_t *longDescription = 0;
		uint32_t visiblity = 0;
		uint32_t type = 0;
		
		

		if (g_Steam &&  argc == 8)
		{
			if(FREGetObjectAsUTF8(argv[0], &len, &fileName) == FRE_OK &&
				FREGetObjectAsUTF8(argv[1], &len, &previewFile) == FRE_OK &&
				FREGetObjectAsUTF8(argv[2], &len, &title) == FRE_OK &&
				FREGetObjectAsUTF8(argv[3], &len, &description) == FRE_OK &&
				FREGetObjectAsUTF8(argv[4], &len, &longDescription) == FRE_OK &&
				FREGetObjectAsUint32(argv[5], &visiblity) == FRE_OK &&
				FREGetObjectAsUint32(argv[7], &type) == FRE_OK
			) 
			{
				//FREObject tags = argv[6]; //tag array
				//uint32_t *tag_len = 0; // array length
				//FREGetArrayLength(tags, tag_len);

				g_Steam->PublishWorkshopFile((char *)fileName, (char *)previewFile, (char *)title, (char *)description, (char *)longDescription, (ERemoteStoragePublishedFileVisibility)visiblity, NULL, (EWorkshopFileType)type );
				FRENewObjectFromBool(true , &result);
			}
			else {
				FRENewObjectFromBool(false, &result);
			}

			
		} else {
			FRENewObjectFromBool(false, &result);
		}
		SteamAPI_RunCallbacks();
		return result;
	}
	

	FREObject AIRSteam_GetPublishWorkshopFileResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam )
		{
			result = UInt64ToFREObject(g_Steam->publishWorkshopFileResult);
		}
		return result;
	}

	FREObject AIRSteam_GetPublishedFileDetails(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 publishedFileId = 0;

		if (g_Steam &&  argc == 1)
		{
			publishedFileId = FREObjectToUint64(argv[0]);
			if( publishedFileId != 0 ) 
			{
				g_Steam->GetPublishedFileDetails(publishedFileId);
				FRENewObjectFromBool(true , &result);
			}
			else {
				FRENewObjectFromBool(false, &result);
			}
		} else {
			FRENewObjectFromBool(false, &result);
		}
		return result;
	}

	
	FREObject AIRSteam_GetPublishedFileDetailsResult (FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;

		if (g_Steam )
		{
			RemoteStorageGetPublishedFileDetailsResult_t fileDetails = *g_Steam->PublishedFileDetailsResult;

			FRENewObject((const uint8_t*)"Object", 0, NULL, &result,NULL);

			FREObject m_nPublishedFileId;
			FREObject m_nCreatorAppID;
			FREObject m_nConsumerAppID;
			FREObject m_rgchTitle = NULL;
			FREObject m_rgchDescription = NULL;
			FREObject m_hFile;
			FREObject m_hPreviewFile;
			FREObject m_ulSteamIDOwner;
			FREObject m_rtimeCreated;
			FREObject m_rtimeUpdated;
			FREObject m_eVisibility;
			FREObject m_bBanned;
			FREObject m_rgchTags;
			FREObject m_bTagsTruncated;
			FREObject m_pchFileName;
			FREObject m_nFileSize;
			FREObject m_nPreviewFileSize;
			FREObject m_rgchURL;
			FREObject m_eFileType;

			uint32_t len = -1;

			// populate published file details
			m_nPublishedFileId = UInt64ToFREObject(fileDetails.m_nPublishedFileId);
			FRENewObjectFromInt32(fileDetails.m_nCreatorAppID, &m_nCreatorAppID);
			FRENewObjectFromInt32(fileDetails.m_nConsumerAppID, &m_nConsumerAppID);
			FRENewObjectFromUTF8(len, (const uint8_t *)fileDetails.m_rgchTitle, &m_rgchTitle);
			FRENewObjectFromUTF8(len, (const uint8_t *)fileDetails.m_rgchDescription, &m_rgchDescription);
			m_hFile = UInt64ToFREObject(fileDetails.m_hFile);
			m_hPreviewFile = UInt64ToFREObject(fileDetails.m_hPreviewFile);
			m_ulSteamIDOwner = UInt64ToFREObject(fileDetails.m_ulSteamIDOwner);
			FRENewObjectFromInt32(fileDetails.m_rtimeCreated, &m_rtimeCreated);
			FRENewObjectFromInt32(fileDetails.m_rtimeUpdated, &m_rtimeUpdated);
			FRENewObjectFromInt32(fileDetails.m_eVisibility, &m_eVisibility);
			FRENewObjectFromBool(fileDetails.m_bBanned, &m_bBanned);
			FRENewObjectFromUTF8(len, (const uint8_t *)fileDetails.m_rgchTags, &m_rgchTags);
			FRENewObjectFromBool(fileDetails.m_bTagsTruncated, &m_bTagsTruncated);
			FRENewObjectFromUTF8(len, (const uint8_t *)fileDetails.m_pchFileName, &m_pchFileName);
			FRENewObjectFromInt32(fileDetails.m_nFileSize, &m_nFileSize);
			FRENewObjectFromInt32(fileDetails.m_nPreviewFileSize, &m_nPreviewFileSize);
			FRENewObjectFromUTF8(len, (const uint8_t *)fileDetails.m_rgchURL, &m_rgchURL);
			FRENewObjectFromInt32(fileDetails.m_eFileType, &m_eFileType);

			// fill properties of FREObject result
			FRESetObjectProperty(result, (const uint8_t*)"publishedFileId", m_nPublishedFileId, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"creatorAppID", m_nCreatorAppID, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"consumerAppID", m_nConsumerAppID, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"title", m_rgchTitle, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"description", m_rgchDescription, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"file", m_hFile, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"previewFile", m_hPreviewFile, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"steamIDOwner", m_ulSteamIDOwner, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"timeCreated", m_rtimeCreated, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"timeUpdated", m_rtimeUpdated, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"visibility", m_eVisibility, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"banned", m_bBanned, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"tags", m_rgchTags, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"tagsTruncated", m_bTagsTruncated, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"fileName", m_pchFileName, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"fileSize", m_nFileSize, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"previewFileSize", m_nPreviewFileSize, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"url", m_rgchURL, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"fileType", m_eFileType, NULL);
		}
		
		return result;
	}

	//ENUMERATION
	FREObject AIRSteam_EnumerateUserPublishedFiles(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		uint32_t unStartIndex = 0;
		uint32_t len = -1;
		if (g_Steam &&  argc == 1)
		{
			if(FREGetObjectAsUint32(argv[0], &unStartIndex) == FRE_OK )
			{
				g_Steam->EnumerateUserPublishedFiles(unStartIndex);
				FRENewObjectFromBool(true , &result);
			}
			else
				FRENewObjectFromBool(false , &result);
		}
		return result;
	}

	FREObject AIRSteam_EnumerateUserSubscribedFiles(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		uint32_t unStartIndex = 0;
		uint32_t len = -1;
		if (g_Steam &&  argc == 1)
		{
			if(FREGetObjectAsUint32(argv[0], &unStartIndex) == FRE_OK )
			{
				g_Steam->EnumerateUserSubscribedFiles(unStartIndex);
				FRENewObjectFromBool(true , &result);
			}
			else
				FRENewObjectFromBool(false , &result);
		}
		return result;
	}
	FREObject AIRSteam_EnumeratePublishedWorkshopFiles(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint32_t eEnumerationType = 0;
		uint32_t unStartIndex = 0;
		uint32_t unCount = 0;
		uint32_t unDays = 0;
		//SteamParamStringArray_t *pTags = NULL;
		//SteamParamStringArray_t *pUserTags = NULL;

		if (g_Steam &&  argc == 6)
		{
			if(
				FREGetObjectAsUint32(argv[0], &eEnumerationType) == FRE_OK &&
				FREGetObjectAsUint32(argv[1], &unStartIndex) == FRE_OK &&
				FREGetObjectAsUint32(argv[2], &unCount) == FRE_OK &&
				FREGetObjectAsUint32(argv[3], &unDays) == FRE_OK 
			) 
			{

				g_Steam->EnumeratePublishedWorkshopFiles((EWorkshopEnumerationType )eEnumerationType, unStartIndex, unCount, unDays, NULL, NULL );
				FRENewObjectFromBool(true , &result);
			}
			else {
				FRENewObjectFromBool(false, &result);
			}

			
		} else {
			FRENewObjectFromBool(false, &result);
		}
		SteamAPI_RunCallbacks();
		return result;
	}

	FREObject AIRSteam_GetEnumeratedWorkshopFilesResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam )
		{
			FRENewObject((const uint8_t*)"Object", 0, NULL, &result,NULL);

			FREObject m_nResultsReturned;
			FREObject m_nTotalResultCount;
			FREObject m_rgPublishedFileId;
			FREObject m_rgScore;
			//	float m_rgScore[ k_unEnumeratePublishedFilesMaxResults ];

			FRENewObjectFromInt32(g_Steam->EnumerateWorkshopFilesResult->m_nTotalResultCount, &m_nTotalResultCount);
			FRENewObjectFromInt32(g_Steam->enumerateResultsReturned, &m_nResultsReturned);

			//PublishedFileId
			FRENewObject((const uint8_t*)"Array", 0, NULL, &m_rgPublishedFileId, NULL );
			FRESetArrayLength( m_rgPublishedFileId, g_Steam->enumerateResultsReturned );
			
			FRENewObject((const uint8_t*)"Array", 0, NULL, &m_rgScore, NULL );
			FRESetArrayLength( m_rgScore, g_Steam->enumerateResultsReturned );

			for ( uint32_t i = 0; i <  g_Steam->enumerateResultsReturned; i++)
			{
				uint64 val =  g_Steam->EnumerateWorkshopFilesResult->m_rgPublishedFileId[i];
				FREObject element = UInt64ToFREObject(val);
				FRESetArrayElementAt( m_rgPublishedFileId, i, element );

				float score =  g_Steam->EnumerateWorkshopFilesResult->m_rgScore[i];
				FREObject scoreObj;
				FRENewObjectFromDouble( score, &scoreObj );
				FRESetArrayElementAt( m_rgScore, i, scoreObj );
			}
			

			// fill properties of FREObject result
			FRESetObjectProperty(result, (const uint8_t*)"resultsReturned", m_nResultsReturned, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"totalResultCount", m_nTotalResultCount, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"publishedFileId", m_rgPublishedFileId, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"score", m_rgScore, NULL);
		}
		return result;
	}

	FREObject AIRSteam_GetEnumerateUserPublishedFilesResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam )
		{
			FRENewObject((const uint8_t*)"Object", 0, NULL, &result,NULL);

			FREObject m_nResultsReturned;
			FREObject m_nTotalResultCount;
			FREObject m_rgPublishedFileId;

			FRENewObjectFromInt32(g_Steam->EnumerateUserPublishedFilesResult->m_nTotalResultCount, &m_nTotalResultCount);
			FRENewObjectFromInt32(g_Steam->enumerateResultsReturned, &m_nResultsReturned);

			//PublishedFileId
			FRENewObject((const uint8_t*)"Array", 0, NULL, &m_rgPublishedFileId, NULL );
			FRESetArrayLength( m_rgPublishedFileId, g_Steam->enumerateResultsReturned );

			for ( uint32_t i = 0; i <  g_Steam->enumerateResultsReturned; i++)
			{
				uint64 val =  g_Steam->EnumerateUserPublishedFilesResult->m_rgPublishedFileId[i];
				FREObject element = UInt64ToFREObject(val);
				FRESetArrayElementAt( m_rgPublishedFileId, i, element );
			}
			

			// fill properties of FREObject result
			FRESetObjectProperty(result, (const uint8_t*)"resultsReturned", m_nResultsReturned, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"totalResultCount", m_nTotalResultCount, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"publishedFileId", m_rgPublishedFileId, NULL);
		}
		return result;
	}
	
	FREObject AIRSteam_GetEnumerateUserSubscribedFilesResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		if (g_Steam )
		{
			FRENewObject((const uint8_t*)"Object", 0, NULL, &result,NULL);

			FREObject m_nResultsReturned;
			FREObject m_nTotalResultCount;
			FREObject m_rgPublishedFileId;

			FRENewObjectFromInt32(g_Steam->EnumerateUserSubscribedFilesResult->m_nTotalResultCount, &m_nTotalResultCount);
			FRENewObjectFromInt32(g_Steam->enumerateResultsReturned, &m_nResultsReturned);

			//PublishedFileId
			FRENewObject((const uint8_t*)"Array", 0, NULL, &m_rgPublishedFileId, NULL );
			FRESetArrayLength( m_rgPublishedFileId, g_Steam->enumerateResultsReturned );

			for ( uint32_t i = 0; i <  g_Steam->enumerateResultsReturned; i++)
			{
				uint64 val =  g_Steam->EnumerateUserSubscribedFilesResult->m_rgPublishedFileId[i];
				FREObject element = UInt64ToFREObject(val);
				FRESetArrayElementAt( m_rgPublishedFileId, i, element );
			}
			

			// fill properties of FREObject result
			FRESetObjectProperty(result, (const uint8_t*)"resultsReturned", m_nResultsReturned, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"totalResultCount", m_nTotalResultCount, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"publishedFileId", m_rgPublishedFileId, NULL);
		}
		return result;
	}

	FREObject AIRSteam_UGCDownload(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 ugcHandle = 0;
		uint32 priority = 0;

		if (g_Steam &&  argc == 2)
		{
			ugcHandle = FREObjectToUint64(argv[0]);
			if( ugcHandle != 0 && FREGetObjectAsUint32(argv[1], &priority) == FRE_OK ) 
			{
				g_Steam->UGCDownload((UGCHandle_t)ugcHandle, priority);
				FRENewObjectFromBool(true , &result);
			}
			else {
				FRENewObjectFromBool(false, &result);
			}
		} else {
			FRENewObjectFromBool(false, &result);
		}
		return result;
	}

	FREObject AIRSteam_GetUGCDownloadResult(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;

		if (g_Steam )
		{
			RemoteStorageDownloadUGCResult_t fileDetails = *g_Steam->UGCDownloadResult;

			FRENewObject((const uint8_t*)"Object", 0, NULL, &result,NULL);

			FREObject m_hFile;
			FREObject m_nAppID;
			FREObject m_nSizeInBytes;
			FREObject m_pchFileName;
			FREObject m_ulSteamIDOwner;

			uint32_t len = -1;

			// populate published file details
			m_hFile = UInt64ToFREObject(fileDetails.m_hFile);
			m_nAppID = UInt64ToFREObject(fileDetails.m_nAppID);
			FRENewObjectFromInt32(fileDetails.m_nSizeInBytes, &m_nSizeInBytes);
			FRENewObjectFromUTF8(len, (const uint8_t *)fileDetails.m_pchFileName, &m_pchFileName);
			m_ulSteamIDOwner = UInt64ToFREObject(fileDetails.m_ulSteamIDOwner);

			// fill properties of FREObject result
			FRESetObjectProperty(result, (const uint8_t*)"file", m_hFile, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"appID", m_nAppID, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"sizeInBytes", m_nSizeInBytes, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"fileName", m_pchFileName, NULL);
			FRESetObjectProperty(result, (const uint8_t*)"steamIDOwner", m_ulSteamIDOwner, NULL);
		}
		
		return result;
	}

	FREObject AIRSteam_UGCRead(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		int32 readResult = 0;
		if (g_Steam && argc == 4 )
		{
			uint32_t len = -1;
			uint64 ugcHandle = 0;
			uint32 fileSize = 0;
			uint32 offset = 0;

			FREByteArray byteArray;
			char *dataIn;


			ugcHandle = FREObjectToUint64(argv[0]);
			if( ugcHandle != 0
				&& FREAcquireByteArray(argv[1], &byteArray) == FRE_OK
				&& FREGetObjectAsUint32(argv[2], &fileSize) == FRE_OK 
				&& FREGetObjectAsUint32(argv[3], &offset) == FRE_OK )
			{
				if(fileSize > 0 && fileSize <= byteArray.length) 
				{
					dataIn = (char *)malloc(fileSize);
					readResult = SteamRemoteStorage()->UGCRead(ugcHandle, dataIn, fileSize, offset);
					memcpy (byteArray.bytes, dataIn, fileSize); 
					free(dataIn);
					FRENewObjectFromInt32(readResult, &result);
				}
				FREReleaseByteArray(argv[1]);
				
			}
	
		}
		
		FRENewObjectFromInt32(readResult, &result);
		return result;
	}

	FREObject AIRSteam_CreatePublishedFileUpdateRequest(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 publishedFileId = 0;

		if (g_Steam &&  argc == 1)
		{
			publishedFileId = FREObjectToUint64(argv[0]);
			if( publishedFileId != 0 ) 
			{
				PublishedFileUpdateHandle_t updateHandle = g_Steam->CreatePublishedFileUpdateRequest(publishedFileId);
				result = UInt64ToFREObject( updateHandle );
			}
		}
		return result;
	}

	FREObject AIRSteam_UpdatePublishedFileFile(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 updateHandle = 0;
		const uint8_t *fileName = 0;

		if (g_Steam &&  argc == 2)
		{
			updateHandle = FREObjectToUint64(argv[0]);
			if( updateHandle != 0 && FREGetObjectAsUTF8(argv[1], &len, &fileName) == FRE_OK ) 
			{
				 FRENewObjectFromBool( g_Steam->UpdatePublishedFileFile(updateHandle, (char *)fileName), &result);
			}
			else
				FRENewObjectFromBool(false, &result);
		}
		else
			FRENewObjectFromBool(false, &result);
		return result;
	}
	
	FREObject AIRSteam_UpdatePublishedFilePreviewFile(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 updateHandle = 0;
		const uint8_t *fileName = 0;

		if (g_Steam &&  argc == 2)
		{
			updateHandle = FREObjectToUint64(argv[0]);
			if( updateHandle != 0 && FREGetObjectAsUTF8(argv[1], &len, &fileName) == FRE_OK ) 
			{
				 FRENewObjectFromBool( g_Steam->UpdatePublishedFilePreviewFile(updateHandle, (char *)fileName), &result);
			}
			else
				FRENewObjectFromBool(false, &result);
		}
		else
			FRENewObjectFromBool(false, &result);
		return result;
	}
	
	FREObject AIRSteam_UpdatePublishedFileTitle(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 updateHandle = 0;
		const uint8_t *title = 0;

		if (g_Steam &&  argc == 2)
		{
			updateHandle = FREObjectToUint64(argv[0]);
			if( updateHandle != 0 && FREGetObjectAsUTF8(argv[1], &len, &title) == FRE_OK ) 
			{
				 FRENewObjectFromBool( g_Steam->UpdatePublishedFileTitle(updateHandle, (char *)title), &result);
			}
			else
				FRENewObjectFromBool(false, &result);
		}
		else
			FRENewObjectFromBool(false, &result);
		return result;
	}
	
	FREObject AIRSteam_UpdatePublishedFileDescription(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 updateHandle = 0;
		const uint8_t *description = 0;

		if (g_Steam &&  argc == 2)
		{
			updateHandle = FREObjectToUint64(argv[0]);
			if( updateHandle != 0 && FREGetObjectAsUTF8(argv[1], &len, &description) == FRE_OK ) 
			{
				 FRENewObjectFromBool( g_Steam->UpdatePublishedFileDescription(updateHandle, (char *)description), &result);
			}
			else
				FRENewObjectFromBool(false, &result);
		}
		else
			FRENewObjectFromBool(false, &result);
		return result;
	}
	
	FREObject AIRSteam_UpdatePublishedFileVisibility(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 updateHandle = 0;
		uint32_t visibility = 0;

		if (g_Steam &&  argc == 2)
		{
			updateHandle = FREObjectToUint64(argv[0]);
			if( updateHandle != 0 && FREGetObjectAsUint32(argv[1], &visibility) == FRE_OK ) 
			{
				 FRENewObjectFromBool( g_Steam->UpdatePublishedFileVisibility(updateHandle, (ERemoteStoragePublishedFileVisibility)visibility), &result);
			}
			else
				FRENewObjectFromBool(false, &result);
		}
		else
			FRENewObjectFromBool(false, &result);
		return result;
	}
	
	FREObject AIRSteam_UpdatePublishedFileTags(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		FRENewObjectFromBool(false, &result);
		//TODO
		return result;
	}
	
	FREObject AIRSteam_CommitPublishedFileUpdate(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 updateHandle = 0;

		if (g_Steam &&  argc == 1)
		{
			updateHandle = FREObjectToUint64(argv[0]);
			if( updateHandle != 0 ) 
			{
				g_Steam->CommitPublishedFileUpdate(updateHandle);
				FRENewObjectFromBool(true, &result);
			}
			else
				FRENewObjectFromBool(false, &result);
		}
		else
			FRENewObjectFromBool(false, &result);
		return result;
	}
	
	FREObject AIRSteam_DeletePublishedFile(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32_t len = -1;
		uint64 pubFileId = 0;

		if (g_Steam &&  argc == 1)
		{
			pubFileId = FREObjectToUint64(argv[0]);
			if( pubFileId != 0 ) 
			{
				g_Steam->DeletePublishedFile(pubFileId);
				FRENewObjectFromBool(true, &result);
			}
			else
				FRENewObjectFromBool(false, &result);
		}
		else
			FRENewObjectFromBool(false, &result);
		return result;
	}

	//friends
	FREObject AIRSteam_GetFriendCount(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		//todo - support other flags
		int friendCount = SteamFriends()->GetFriendCount(k_EFriendFlagImmediate);
		FRENewObjectFromInt32(friendCount, &result);
	
		return result;
	}

	FREObject AIRSteam_GetFriendByIndex(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		
		uint32 friendIndex = 0;

		if (argc == 1)
		{
			if( FREGetObjectAsUint32(argv[0], &friendIndex) == FRE_OK ) 
			{
				//todo - support other flags
				CSteamID friendId = SteamFriends()->GetFriendByIndex(friendIndex, k_EFriendFlagImmediate);
				result = UInt64ToFREObject( friendId.ConvertToUint64() );
			}
		} 

		return result;
	}

	FREObject AIRSteam_GetFriendPersonaName(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		FREObject result;
		

		uint64 friendID = 0;

		if (argc==1	 ) 
		{
			friendID = FREObjectToUint64(argv[0]);

			const char *friendName = SteamFriends()->GetFriendPersonaName(CSteamID(friendID) ); 
			uint32_t len = -1;
			FRENewObjectFromUTF8(len, (const uint8_t *)friendName, &result);
		} 
		
		return result;
	}
	
	//============================

	FREObject UInt64ToFREObject( uint64 value) {
		FREObject result;
		uint32_t len = -1;
		char chrarray[20];
		_ui64toa_s(value, chrarray, 20, 10 ); //convert base10 number to char * 
		FRENewObjectFromUTF8(len, (const uint8_t *)chrarray, &result);
		return result;
	}

	uint64 FREObjectToUint64( FREObject valueString )
	{
		uint64 result;
		uint32_t len = -1;
		const uint8_t *chararry = 0;
		if(FREGetObjectAsUTF8(valueString, &len, &chararry) == FRE_OK)
			result = _strtoui64((const char *)chararry, NULL, 10);
		return result;
	}
    
    // A native context instance is created
    void ContextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, 
                            uint32_t* numFunctions, const FRENamedFunction** functions) {
        AIRContext = ctx;
        
        *numFunctions = 54;
        
        FRENamedFunction* func = (FRENamedFunction*) malloc(sizeof(FRENamedFunction) * (*numFunctions));
        
        func[0].name = (const uint8_t*) "AIRSteam_Init";
        func[0].functionData = NULL;
        func[0].function = &AIRSteam_Init;
        
        func[1].name = (const uint8_t*) "AIRSteam_RequestStats";
        func[1].functionData = NULL;
        func[1].function = &AIRSteam_RequestStats;
        
        func[2].name = (const uint8_t*) "AIRSteam_SetAchievement";
        func[2].functionData = NULL;
        func[2].function = &AIRSteam_SetAchievement;
        
        func[3].name = (const uint8_t*) "AIRSteam_ClearAchievement";
        func[3].functionData = NULL;
        func[3].function = &AIRSteam_ClearAchievement;
        
        func[4].name = (const uint8_t*) "AIRSteam_IsAchievement";
        func[4].functionData = NULL;
        func[4].function = &AIRSteam_IsAchievement;
        
        func[5].name = (const uint8_t*) "AIRSteam_GetStatInt";
        func[5].functionData = NULL;
        func[5].function = &AIRSteam_GetStatInt;
        
        func[6].name = (const uint8_t*) "AIRSteam_GetStatFloat";
        func[6].functionData = NULL;
        func[6].function = &AIRSteam_GetStatFloat;
        
        func[7].name = (const uint8_t*) "AIRSteam_SetStatInt";
        func[7].functionData = NULL;
        func[7].function = &AIRSteam_SetStatInt;
        
        func[8].name = (const uint8_t*) "AIRSteam_SetStatFloat";
        func[8].functionData = NULL;
        func[8].function = &AIRSteam_SetStatFloat;
        
        func[9].name = (const uint8_t*) "AIRSteam_StoreStats";
        func[9].functionData = NULL;
        func[9].function = &AIRSteam_StoreStats;
        
        func[10].name = (const uint8_t*) "AIRSteam_RunCallbacks";
        func[10].functionData = NULL;
        func[10].function = &AIRSteam_RunCallbacks;
        
        func[11].name = (const uint8_t*) "AIRSteam_ResetAllStats";
        func[11].functionData = NULL;
        func[11].function = &AIRSteam_ResetAllStats;
        
        //SteamRemoteStorage...
        func[12].name = (const uint8_t*) "AIRSteam_GetFileCount";
        func[12].functionData = NULL;
        func[12].function = &AIRSteam_GetFileCount;
        
        func[13].name = (const uint8_t*) "AIRSteam_GetFileSize";
        func[13].functionData = NULL;
        func[13].function = &AIRSteam_GetFileSize;
        
        func[14].name = (const uint8_t*) "AIRSteam_FileExists";
        func[14].functionData = NULL;
        func[14].function = &AIRSteam_FileExists;
        
        func[15].name = (const uint8_t*) "AIRSteam_FileWrite";
        func[15].functionData = NULL;
        func[15].function = &AIRSteam_FileWrite;
        
        func[16].name = (const uint8_t*) "AIRSteam_FileRead";
        func[16].functionData = NULL;
        func[16].function = &AIRSteam_FileRead;
        
        func[17].name = (const uint8_t*) "AIRSteam_FileDelete";
        func[17].functionData = NULL;
        func[17].function = &AIRSteam_FileDelete;
        
        func[18].name = (const uint8_t*) "AIRSteam_IsCloudEnabledForApp";
        func[18].functionData = NULL;
        func[18].function = &AIRSteam_IsCloudEnabledForApp;
        
        func[19].name = (const uint8_t*) "AIRSteam_SetCloudEnabledForApp";
        func[19].functionData = NULL;
        func[19].function = &AIRSteam_SetCloudEnabledForApp;

        func[20].name = (const uint8_t*) "AIRSteam_FileShare";
        func[20].functionData = NULL;
		func[20].function = &AIRSteam_FileShare;
        func[21].name = (const uint8_t*) "AIRSteam_PublishWorkshopFile";
        func[21].functionData = NULL;
		func[21].function = &AIRSteam_PublishWorkshopFile;

        func[22].name = (const uint8_t*) "AIRSteam_EnumeratePublishedWorkshopFiles";
        func[22].functionData = NULL;
		func[22].function = &AIRSteam_EnumeratePublishedWorkshopFiles;

        func[23].name = (const uint8_t*) "AIRSteam_GetEnumeratedWorkshopFilesResult";
        func[23].functionData = NULL;
		func[23].function = &AIRSteam_GetEnumeratedWorkshopFilesResult;
		
        func[24].name = (const uint8_t*) "AIRSteam_GetPublishedFileDetails";
        func[24].functionData = NULL;
		func[24].function = &AIRSteam_GetPublishedFileDetails;

		func[25].name = (const uint8_t*) "AIRSteam_GetPublishedFileDetailsResult";
        func[25].functionData = NULL;
		func[25].function = &AIRSteam_GetPublishedFileDetailsResult;


		func[26].name = (const uint8_t*) "AIRSteam_UGCDownload";
        func[26].functionData = NULL;
		func[26].function = &AIRSteam_UGCDownload;
		
		func[27].name = (const uint8_t*) "AIRSteam_GetUGCDownloadResult";
        func[27].functionData = NULL;
		func[27].function = &AIRSteam_GetUGCDownloadResult;

		func[28].name = (const uint8_t*) "AIRSteam_UGCRead";
        func[28].functionData = NULL;
		func[28].function = &AIRSteam_UGCRead;

		func[29].name = (const uint8_t*) "AIRSteam_EnumerateUserPublishedFiles";
        func[29].functionData = NULL;
		func[29].function = &AIRSteam_EnumerateUserPublishedFiles;
		
		func[30].name = (const uint8_t*) "AIRSteam_EnumerateUserSubscribedFiles";
        func[30].functionData = NULL;
		func[30].function = &AIRSteam_EnumerateUserSubscribedFiles;
		
		func[31].name = (const uint8_t*) "AIRSteam_CreatePublishedFileUpdateRequest";
        func[31].functionData = NULL;
		func[31].function = &AIRSteam_CreatePublishedFileUpdateRequest;
		
		func[32].name = (const uint8_t*) "AIRSteam_UpdatePublishedFileFile";
        func[32].functionData = NULL;
		func[32].function = &AIRSteam_UpdatePublishedFileFile;
		
		func[33].name = (const uint8_t*) "AIRSteam_UpdatePublishedFilePreviewFile";
        func[33].functionData = NULL;
		func[33].function = &AIRSteam_UpdatePublishedFilePreviewFile;
		
		func[34].name = (const uint8_t*) "AIRSteam_UpdatePublishedFileTitle";
        func[34].functionData = NULL;
		func[34].function = &AIRSteam_UpdatePublishedFileTitle;
		
		func[35].name = (const uint8_t*) "AIRSteam_UpdatePublishedFileDescription";
        func[35].functionData = NULL;
		func[35].function = &AIRSteam_UpdatePublishedFileDescription;
		
		func[36].name = (const uint8_t*) "AIRSteam_UpdatePublishedFileVisibility";
        func[36].functionData = NULL;
		func[36].function = &AIRSteam_UpdatePublishedFileVisibility;
		
		func[37].name = (const uint8_t*) "AIRSteam_UpdatePublishedFileTags";
        func[37].functionData = NULL;
		func[37].function = &AIRSteam_UpdatePublishedFileTags;
		
		func[38].name = (const uint8_t*) "AIRSteam_CommitPublishedFileUpdate";
        func[38].functionData = NULL;
		func[38].function = &AIRSteam_CommitPublishedFileUpdate;
		
		func[39].name = (const uint8_t*) "AIRSteam_DeletePublishedFile";
        func[39].functionData = NULL;
		func[39].function = &AIRSteam_DeletePublishedFile;

		func[40].name = (const uint8_t*) "AIRSteam_GetPublishWorkshopFileResult";
        func[40].functionData = NULL;
		func[40].function = &AIRSteam_GetPublishWorkshopFileResult;
		
		func[41].name = (const uint8_t*) "AIRSteam_GetEnumerateUserPublishedFilesResult";
        func[41].functionData = NULL;
		func[41].function = &AIRSteam_GetEnumerateUserPublishedFilesResult;
		
		func[42].name = (const uint8_t*) "AIRSteam_GetEnumerateUserSubscribedFilesResult";
        func[42].functionData = NULL;
		func[42].function = &AIRSteam_GetEnumerateUserSubscribedFilesResult;

		
		func[43].name = (const uint8_t*) "AIRSteam_FindLeadboard";
        func[43].functionData = NULL;
		func[43].function = &AIRSteam_FindLeadboard;
		
		func[44].name = (const uint8_t*) "AIRSteam_GetFindLeadboardResult";
        func[44].functionData = NULL;
		func[44].function = &AIRSteam_GetFindLeadboardResult;
		
		func[45].name = (const uint8_t*) "AIRSteam_UploadLeaderboardScore";
        func[45].functionData = NULL;
		func[45].function = &AIRSteam_UploadLeaderboardScore;

		func[46].name = (const uint8_t*) "AIRSteam_GetLeaderboardScoreUploadedResult";
        func[46].functionData = NULL;
		func[46].function = &AIRSteam_GetLeaderboardScoreUploadedResult;

		func[47].name = (const uint8_t*) "AIRSteam_DownloadLeaderboardEntries";
        func[47].functionData = NULL;
		func[47].function = &AIRSteam_DownloadLeaderboardEntries;
		
		func[48].name = (const uint8_t*) "AIRSteam_GetDownloadedLeaderboardEntryResult";
        func[48].functionData = NULL;
		func[48].function = &AIRSteam_GetDownloadedLeaderboardEntryResult;
		
		func[49].name = (const uint8_t*) "AIRSteam_IsSubscribedApp";
        func[49].functionData = NULL;
		func[49].function = &AIRSteam_IsSubscribedApp;
		
		func[50].name = (const uint8_t*) "AIRSteam_IsDlcInstalled";
        func[50].functionData = NULL;
		func[50].function = &AIRSteam_IsDlcInstalled;

		func[51].name = (const uint8_t*) "AIRSteam_GetFriendCount";
        func[51].functionData = NULL;
		func[51].function = &AIRSteam_GetFriendCount;
		
		func[52].name = (const uint8_t*) "AIRSteam_GetFriendByIndex";
        func[52].functionData = NULL;
		func[52].function = &AIRSteam_GetFriendByIndex;
		
		func[53].name = (const uint8_t*) "AIRSteam_GetFriendPersonaName";
        func[53].functionData = NULL;
		func[53].function = &AIRSteam_GetFriendPersonaName;

        *functions = func;
    }
    
    // A native context instance is disposed
    void ContextFinalizer(FREContext ctx) {
        AIRContext = NULL;
        // Shutdown Steam
        SteamAPI_Shutdown();
        // Delete the SteamAchievements object
        if (g_Steam)
            g_Steam = NULL;
        return;
    }
    
    // Initialization function of each extension
    EXPORT void ExtInitializer(void** extDataToSet, FREContextInitializer* ctxInitializerToSet, 
                               FREContextFinalizer* ctxFinalizerToSet) {
        *extDataToSet = NULL;
        *ctxInitializerToSet = &ContextInitializer;
        *ctxFinalizerToSet = &ContextFinalizer;
    }
    
    // Called when extension is unloaded
    EXPORT void ExtFinalizer(void* extData) {
        return;
    }
}
