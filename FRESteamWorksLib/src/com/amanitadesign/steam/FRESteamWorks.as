package com.amanitadesign.steam
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.events.StatusEvent;
	import flash.external.ExtensionContext;
	import flash.utils.ByteArray;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	
	public class FRESteamWorks extends EventDispatcher
	{
		[Event(name="steam_response", type="com.amanitadesign.steam.SteamEvent")]
		
		private var _ExtensionContext:ExtensionContext;
		private var _tm:int;
		
		public var isReady:Boolean = false;
		
		public function FRESteamWorks(target:IEventDispatcher=null)
		{
			_ExtensionContext = ExtensionContext.createExtensionContext("com.amanitadesign.steam.FRESteamWorks", null);
			_ExtensionContext.addEventListener(StatusEvent.STATUS, handleStatusEvent);
			super(target);
		}
		
		private function handleStatusEvent(event:StatusEvent):void{
			//_ExtensionContext.removeEventListener(StatusEvent.STATUS, handleStatusEvent);
			var req_type:int = new int(event.code);
			var response:int = new int(event.level);
			var sEvent:SteamEvent = new SteamEvent(SteamEvent.STEAM_RESPONSE, req_type, response);
			//trace("handleStatusEvent: "+req_type+" "+response + " " + SteamResult.getMessage(response));
			switch(req_type)
			{
				case SteamConstants.RESPONSE_LeaderboardScoresDownloaded:
					sEvent.data = this.getDownloadedLeaderboardEntryResult();
					break;
				case SteamConstants.RESPONSE_LeaderboardFindResult:
					sEvent.data = this.getFindLeaderboardResult();
					break;
				case SteamConstants.RESPONSE_LeaderboardScoreUploaded:
					sEvent.data = new LeaderboardScoreUploaded(this.getLeaderboardScoreUploadedResult());
					break;
				case SteamConstants.RESPONSE_OnUserStatsReceived:
					trace("RESPONSE_OnUserStatsReceived");
					break;
				case SteamConstants.RESPONSE_OnAchievementStored:
					trace("RESPONSE_OnAchievementStored");
					break;
				case SteamConstants.RESPONSE_OnPublishWorkshopFile:
					sEvent.data = this.getPublishWorkshopFileResult();
					break;
				case SteamConstants.RESPONSE_EnumeratePublishedWorkshopFiles:
					trace("RESPONSE_EnumeratePublishedWorkshopFiles setting data");
					sEvent.data = new EnumerateWorkshopFilesResult(this.getEnumeratedWorkshopFilesResult());
					break;
				case SteamConstants.RESPONSE_EnumerateUserPublishedFiles:
					trace("RESPONSE_EnumerateUserPublishedFiles setting data");
					sEvent.data = new EnumerateWorkshopFilesResult(this.getEnumeratedUserPublishedFilesResult());
					break;
				case SteamConstants.RESPONSE_EnumerateUserSubscribedFiles:
					trace("RESPONSE_EnumerateUserSubscribedFiles setting data");
					sEvent.data = new EnumerateWorkshopFilesResult(this.getEnumeratedUserSubscribedFilesResult());
					break;
				case SteamConstants.RESPONSE_GetPublishedFileDetails:
					trace("RESPONSE_GetPublishedFileDetails setting data");
					
					if ( isBatch )
					{
						batchPublishedFileDetails.push( new PublishedFileDetailsResult(this.getPublishedFileDetailsResult()) );
						currentBatchIndex++;
						if ( currentBatchIndex < batchPublishedFileIds.length )
						{
							getPublishedFileDetails(batchPublishedFileIds[currentBatchIndex]);
							return; //dont dispatch an individual get published file details during batch get
						}
						else
						{
							isBatch = false;
							sEvent = new SteamEvent(SteamEvent.STEAM_RESPONSE,  SteamConstants.RESPONSE_BatchGetPublishedFileDetails, response);
							sEvent.data = batchPublishedFileDetails;
						}
					}
					else
						sEvent.data = new PublishedFileDetailsResult(this.getPublishedFileDetailsResult());
					break;
				case SteamConstants.RESPONSE_UGCDownload:
					trace("RESPONSE_UGCDownload setting data");
					sEvent.data = new UGCResult(this.getUGCDownloadResult());
					break;
			}
			dispatchEvent(sEvent);
		}
		
		public function dispose():void
		{
			clearInterval(_tm);
			_ExtensionContext.removeEventListener(StatusEvent.STATUS, handleStatusEvent);
			_ExtensionContext.dispose();
		}

		public function init():Boolean
		{
			isReady = _ExtensionContext.call("AIRSteam_Init") as Boolean;
			if(isReady)	_tm = setInterval(runCallbacks, 100);
			return isReady;
		}
		
		//leaderboards
		
		public function findLeaderboard(leaderboardName:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_FindLeadboard", leaderboardName) as Boolean;
		}
		
		public function uploadLeaderboardScore(leaderboardHandle:String, score:int, leaderboardUploadScoreMethod:uint=0, scoreDetails:Array = null):Boolean
		{
			if ( scoreDetails == null )
				scoreDetails = [];
			return _ExtensionContext.call("AIRSteam_UploadLeaderboardScore", leaderboardHandle, score, leaderboardUploadScoreMethod, scoreDetails, scoreDetails.length) as Boolean;
		}
		
		/**
			* Asks the Steam back-end for a set of rows in the leaderboard.
			* This call is asynchronous, with the result returned in event data property
			* You can ask for more entries than exist, and it will return as many as do exist.
		 * @param	hSteamLeaderboard Steam leaderboard handle
		 * @param	eLeaderboardDataRequest Type of request data. 0: Global, 1: GlobalAroundUser, 2: Friends, 3: Users. 
		 * @param	nRangeStart The start range of the leaderboard.  Example,  you can display the top 10 on a leaderboard for your game by setting start to 1 and end to 10 (with the request type as Global).  If you are requesting entries around a user, start range can be negative to get entries before the user. For example, if the current user is #5 on a given leaderboard, setting start to -2 and end to 2 will return 5 entries: 3 through 7. If there are not enough entries in the leaderboard before or after the user's entry, Steam will adjust the range to try to return the number of entries requested. For example, if the user is #1 on the leaderboard, start is set to -2, and end is set to 2, Steam will return the first 5 entries in the leaderboard.
		 * @param	nRangeEnd
		 * @return
		 */
		public function downloadLeaderboardEntries(hSteamLeaderboard:String, eLeaderboardDataRequest:int=0, nRangeStart:int=0, nRangeEnd:int=10):Boolean
		{
			return _ExtensionContext.call("AIRSteam_DownloadLeaderboardEntries", hSteamLeaderboard, eLeaderboardDataRequest, nRangeStart, nRangeEnd) as Boolean;
		}
		
		
		//stats
		public function requestStats():Boolean
		{
			return _ExtensionContext.call("AIRSteam_RequestStats") as Boolean;
		}
		
		public function runCallbacks():Boolean
		{
			return _ExtensionContext.call("AIRSteam_RunCallbacks") as Boolean;
		}
		
		public function setAchievement(id:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_SetAchievement", id) as Boolean;
		}
		
		public function clearAchievement(id:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_ClearAchievement", id) as Boolean;
		}
		
		public function isAchievement(id:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_IsAchievement", id) as Boolean;
		}
		
		public function getStatInt(id:String):int
		{
			return _ExtensionContext.call("AIRSteam_GetStatInt", id) as int;
		}
		
		public function getStatFloat(id:String):Number
		{
			return _ExtensionContext.call("AIRSteam_GetStatFloat", id) as Number;
		}
		
		public function setStatInt(id:String, value:int):Boolean
		{
			return _ExtensionContext.call("AIRSteam_SetStatInt", id, value) as Boolean;
		}
		
		public function setStatFloat(id:String, value:Number):Boolean
		{
			return _ExtensionContext.call("AIRSteam_SetStatFloat", id, value) as Boolean;
		}
		
		public function storeStats():Boolean
		{
			return _ExtensionContext.call("AIRSteam_StoreStats") as Boolean;
		}
		
		public function resetAllStats(bAchievementsToo:Boolean):Boolean
		{
			return _ExtensionContext.call("AIRSteam_ResetAllStats", bAchievementsToo) as Boolean;
		}
		
		
		//local file storage
		
		public function getFileCount():int
		{
			return _ExtensionContext.call("AIRSteam_GetFileCount") as int;
		}
		
		public function getFileSize(fileName:String):int
		{
			return _ExtensionContext.call("AIRSteam_GetFileSize", fileName) as int;
		}
		
		public function fileExists(fileName:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_FileExists", fileName) as Boolean;
		}
		
		public function fileWrite(fileName:String, data:ByteArray):Boolean
		{
			return _ExtensionContext.call("AIRSteam_FileWrite", fileName, data) as Boolean;
		}
		
		public function fileRead(fileName:String, data:ByteArray):Boolean
		{
			return _ExtensionContext.call("AIRSteam_FileRead", fileName, data) as Boolean;
		}
		
		public function fileDelete(fileName:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_FileDelete", fileName) as Boolean;
		}
		
		//cloud storage
		
		public function isCloudEnabledForApp():Boolean
		{
			return _ExtensionContext.call("AIRSteam_IsCloudEnabledForApp") as Boolean;
		}
		
		public function setCloudEnabledForApp(enabled:Boolean):Boolean
		{
			return _ExtensionContext.call("AIRSteam_SetCloudEnabledForApp", enabled) as Boolean;
		}
		
		//WORKSHOP functions
		
		public function fileShare(fileName:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_FileShare", fileName) as Boolean;
		}
		
		public function publishWorkshopFile(fileName:String, previewFile:String, title:String, description:String, longDescription:String, visibility:uint, tags:Array, type:uint):Boolean
		{
			return _ExtensionContext.call("AIRSteam_PublishWorkshopFile", fileName, previewFile, title, description, longDescription, visibility,tags,type) as Boolean;
		}
		
		/**
		 * Enumerates publically shared Workshop files that match filter criteria.
		 * For beta testing, you must change the beta-testing visibility under the Workshop tab for results to show up.
		 * @param	enumerationType WorkshopEnumerationType
		 * @param	startIndex
		 * @param	count
		 * @param	days
		 * @param	pTags
		 * @param	uTags
		 * @return
		 */
		public function enumeratePublishedWorkshopFiles(enumerationType:uint, startIndex:uint, count:uint, days:uint, pTags:Array,uTags:Array):Boolean
		{
			return _ExtensionContext.call("AIRSteam_EnumeratePublishedWorkshopFiles", enumerationType, startIndex, count, days, pTags, uTags) as Boolean;
		}
		
		public function enumerateUserPublishedFiles(startIndex:uint):Boolean
		{
			return _ExtensionContext.call("AIRSteam_EnumerateUserPublishedFiles", startIndex) as Boolean;
		}
		public function enumerateUserSubscribedFiles(startIndex:uint):Boolean
		{
			return _ExtensionContext.call("AIRSteam_EnumerateUserSubscribedFiles", startIndex) as Boolean;
		}
		
		public function getPublishedFileDetails(publishedFileId:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_GetPublishedFileDetails", publishedFileId) as Boolean;
		}
		
		private var batchPublishedFileIds:Array;
		private var batchPublishedFileDetails:Array;
		private var currentBatchIndex:int = 0;
		private var isBatch:Boolean = false;
		
		public function batchGetPublishedFileDetails(publishedFileIds:Array):void
		{
			batchPublishedFileIds = publishedFileIds;
			batchPublishedFileDetails = [];
			currentBatchIndex = 0;
			isBatch = true;
			getPublishedFileDetails(batchPublishedFileIds[currentBatchIndex]);
		}
		
		public function UGCDownload(ugcHandle:String, priority:uint=0):Boolean
		{
			return _ExtensionContext.call("AIRSteam_UGCDownload", ugcHandle, priority) as Boolean;
		}
		public function UGCRead(ugcHandle:String, byteArray:ByteArray, fileSize:uint, offset:uint=0):int
		{
			return _ExtensionContext.call("AIRSteam_UGCRead", ugcHandle, byteArray, fileSize, offset) as int;
		}
		
		//update published file
		
		public function createPublishedFileUpdateRequest(publishFileId:String):String
		{
			return _ExtensionContext.call("AIRSteam_CreatePublishedFileUpdateRequest", publishFileId) as String; 
		}
		
		public function updatePublishedFileFile(updateHandle:String, fileName:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_UpdatePublishedFileFile", updateHandle, fileName) as Boolean; 
		}
		
		public function updatePublishedFilePreviewFile(updateHandle:String, previewFileName:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_UpdatePublishedFilePreviewFile", updateHandle, previewFileName) as Boolean; 
		}
		
		public function updatePublishedFileTitle(updateHandle:String, title:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_UpdatePublishedFileTitle", updateHandle, title) as Boolean; 
		}
		
		public function updatePublishedFileDescription(updateHandle:String, desc:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_UpdatePublishedFileDescription", updateHandle, desc) as Boolean; 
		}
		
		public function updatePublishedFileVisibility(updateHandle:String, visibility:uint):Boolean
		{
			return _ExtensionContext.call("AIRSteam_UpdatePublishedFileVisibility", updateHandle, visibility) as Boolean; 
		}
		
		public function updatePublishedFileTags(updateHandle:String, tags:Array):Boolean
		{
			return _ExtensionContext.call("AIRSteam_UpdatePublishedFileTags", updateHandle, tags) as Boolean; 
		}
		
		public function commitPublishedFileUpdate(updateHandle:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_CommitPublishedFileUpdate", updateHandle) as Boolean; 
		}
		
		public function deletePublishedFile(publishFileId:String):Boolean
		{
			return _ExtensionContext.call("AIRSteam_DeletePublishedFile", publishFileId) as Boolean; 
		}
		
		//protected functions to get the result of async call results
		protected function getLeaderboardScoreUploadedResult():Object
		{
			return _ExtensionContext.call("AIRSteam_GetLeaderboardScoreUploadedResult") as Object;
		}
		
		protected function getDownloadedLeaderboardEntryResult():Object
		{
			return _ExtensionContext.call("AIRSteam_GetDownloadedLeaderboardEntryResult") as Object;
		}
		protected function getFindLeaderboardResult():String
		{
			return _ExtensionContext.call("AIRSteam_GetFindLeadboardResult") as String;
		}
		protected function getPublishWorkshopFileResult():String
		{
			return _ExtensionContext.call("AIRSteam_GetPublishWorkshopFileResult") as String;
		}
		protected function getEnumeratedWorkshopFilesResult():Object
		{
			return _ExtensionContext.call("AIRSteam_GetEnumeratedWorkshopFilesResult") as Object;
		}
		protected function getEnumeratedUserPublishedFilesResult():Object
		{
			return _ExtensionContext.call("AIRSteam_GetEnumerateUserPublishedFilesResult") as Object;
		}
		protected function getEnumeratedUserSubscribedFilesResult():Object
		{
			return _ExtensionContext.call("AIRSteam_GetEnumerateUserSubscribedFilesResult") as Object;
		}
		protected function getPublishedFileDetailsResult():Object
		{
			return _ExtensionContext.call("AIRSteam_GetPublishedFileDetailsResult") as Object;
		}
		protected function getUGCDownloadResult():Object
		{
			return _ExtensionContext.call("AIRSteam_GetUGCDownloadResult") as Object;
		}
	}
}