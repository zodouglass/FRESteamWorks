package 
{
	import com.adobe.images.PNGEncoder;
	import com.amanitadesign.steam.EnumerateWorkshopFilesResult;
	import com.amanitadesign.steam.FRESteamWorks;
	import com.amanitadesign.steam.LeaderboardScoreUploaded;
	import com.amanitadesign.steam.RemoteStoragePublishedFileVisibility;
	import com.amanitadesign.steam.SteamConstants;
	import com.amanitadesign.steam.SteamEvent;
	import com.amanitadesign.steam.SteamResult;
	import com.amanitadesign.steam.UGCResult;
	import com.amanitadesign.steam.WorkshopEnumerationType;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.SecurityErrorEvent;
	import flash.net.navigateToURL;
	import flash.net.URLRequest;
	import flash.text.TextFormat;

	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.utils.ByteArray;
	
	public class FRESteamWorksTest extends Sprite 
	{
		private var Steamworks:FRESteamWorks = new FRESteamWorks();
		public var tf:TextField;
		
		[Embed(source = "../bin/mylevel1.png")]
		private var _testImg:Class;
		[Embed(source = "../bin/preview.jpg")]
		private var _previewImg:Class;
		
		//private static var TEST_FILE:String = "mylevel1.png";
		private static var TEST_FILE:String = "test.txt";
		
		private var publishedWorkshopFileId:String; //result returned fro PublishWOrkshopFile
		
		private var publishedFileDetails:Array; //result from batchGetPublishedFileDetails
		private var ugcDownloadResult:UGCResult;
		
		private var leaderboardHandle:String;
		
		public function FRESteamWorksTest():void 
		{
			tf = new TextField();
			tf.x = 210;
			tf.width = stage.stageWidth - tf.x;
			tf.height = stage.stageHeight;
			addChild(tf);

			//tf.addEventListener(MouseEvent.MOUSE_DOWN, onClick);
			
			addButton("Find Leaderboard", handleFindLeaderboard);
			addButton("Submit Leaderboard Score", handleSubmitLeaderboardScore);
			addButton("SteamUserStats()->DownloadLeaderboardEntries", handleDownloadLeaderboardEntries);
			addButton("SteamUserStats()->GetLeaderboardName()", handleGetLeaderboardName);
			addButton("SteamUserStats()->GetLeaderboardEntryCount()", handleGetLeaderboardEntryCount);
			addButton("SteamUserStats()->GetLeaderboardSortMethod()", handleGetLeaderboardSortMethod);
			addButton("SteamUserStats()->GetLeaderboardDisplayType()", handleGetLeaderboardDisplayType);
			addButton("Toggle Achievement", handleWinAchievement);
			addButton("Write Text to Cloud", handleFileWrite);
			addButton("Write Image to Cloud", handleFileWriteImage);
			addButton("Share File", handleFileShare);
			addButton("Publish File", handlePublishWorkshopFile);
			addButton("Change Pub. File Content", handleChangePublishedFile);
			addButton("Update Published File", handleUpdatePublishedFile);
			
			addButton("Enumerate Workshop Files", handleEnumeratePublishedWorkshopFiles);
			addButton("Enumerate User Files", handleEnumerateUserPublishedFiles);
			addButton("Enumerate Subscribed Files", handleEnumerateUserSubscribedFiles);
			addButton("Dowload UGC File", handleUGCDownload);
			addButton("Read UGC File", handleUGCRead);
			addButton("Delete Published File", handleDeletePublishedFile);
			addButton("Open Workshop", handleOpenWorkshop);

			Steamworks.addEventListener(SteamEvent.STEAM_RESPONSE, onSteamResponse);
			
			if(Steamworks.init()){
				log("STEAMWORKS API is available\n");

				//comment.. current stats and achievement ids are from steam example app which is provided with their SDK
				//log("isAchievement('ACH_WIN_ONE_GAME') == "+Steamworks.isAchievement("ACH_WIN_ONE_GAME"));
				//log("isAchievement('ACH_TRAVEL_FAR_SINGLE') == "+Steamworks.isAchievement("ACH_TRAVEL_FAR_SINGLE"));
				//log("setStatFloat('FeetTraveled') == "+Steamworks.setStatFloat('FeetTraveled', 21.3));
				//log("setStatInt('NumGames', 2) == "+Steamworks.setStatInt('NumGames', 2));
				//Steamworks.storeStats();
				//log("getStatInt('NumGames') == "+Steamworks.getStatInt('NumGames'));
				//log("getStatFloat('FeetTraveled') == "+Steamworks.getStatFloat('FeetTraveled'));

			//	log("setCloudEnabledForApp(false) == "+Steamworks.setCloudEnabledForApp(false) );
				//log("setCloudEnabledForApp(true) == "+Steamworks.setCloudEnabledForApp(true) );
				log("isCloudEnabledForApp() == "+Steamworks.isCloudEnabledForApp() );
				//log("getFileCount() == "+Steamworks.getFileCount() );
			//	log("fileExists('test.txt') == "+Steamworks.fileExists('test.txt') );

				//comment.. writing to app with id 480 is somehow not working, but works with our real appId
				//log("writeFileToCloud('test.txt','hello steam') == "+writeFileToCloud('test.txt','hello steam'));
				//log("readFileFromCloud('test.txt') == "+readFileFromCloud('test.txt') );
			//	//-----------

				//Steamworks.requestStats();
				Steamworks.resetAllStats(true);
			}else {
				tf.appendText("STEAMWORKS API is NOT available\n");
			}

		}
		private function log(value:String):void{
			tf.appendText(value+"\n");
			tf.scrollV = tf.maxScrollV;
		}
		public function writeFileToCloud(fileName:String, data:String):Boolean {
			var dataOut:ByteArray = new ByteArray();
			dataOut.writeUTFBytes(data);
			return Steamworks.fileWrite(fileName, dataOut);
		}
		
		public function writeBitmapToCloud(fileName:String, data:Bitmap):Boolean {
			var dataOut:ByteArray = PNGEncoder.encode(data.bitmapData);
			return Steamworks.fileWrite(fileName, dataOut);
		}

		public function readFileFromCloud(fileName:String):String {
			var dataIn:ByteArray = new ByteArray();
			var result:String;
			dataIn.position = 0;
			dataIn.length = Steamworks.getFileSize(fileName);

			if(dataIn.length>0 && Steamworks.fileRead(fileName,dataIn)){
				result = dataIn.readUTFBytes(dataIn.length);
			}
			return result;
		}
		
		public function handleFindLeaderboard(e:Event = null):void
		{
			if (Steamworks.isReady)
			{
				log("findLeaderboard('Test Board') == " + Steamworks.findLeaderboard("Testing LeaderBoard") );
			}
		}
		
		
		
		public function handleDownloadLeaderboardEntries(e:Event = null):void
		{
			if (Steamworks.isReady)
			{
				log("DownloadLeaderboardEntries");
				if ( leaderboardHandle == null ){
					log("You need a leaderboard handle to download the entires of a leaderboard!");
					return;
				}
				
				var k_ELeaderboardDataRequestGlobal:int = 0,
					k_ELeaderboardDataRequestGlobalAroundUser:int = 1,
					k_ELeaderboardDataRequestFriends:int = 2,
					k_ELeaderboardDataRequestUsers:int = 3;
					
				var startRange:int = 0;
				var endRange:int = 10;
				
				//async call to download leaderboard entries.  results are returned in the event data object from SteamConstants.RESPONSE_LeaderboardScoresDownloaded
				Steamworks.downloadLeaderboardEntries(leaderboardHandle, k_ELeaderboardDataRequestGlobal, startRange, endRange );
			}
		}
		public function handleGetLeaderboardName(e:Event = null):void
		{
			
		}
		public function handleGetLeaderboardEntryCount(e:Event = null):void
		{
			
		}
		public function handleGetLeaderboardSortMethod(e:Event = null):void
		{
			
		}
		public function handleGetLeaderboardDisplayType(e:Event = null):void
		{
			
		}
		public function handleSubmitLeaderboardScore(e:Event = null):void
		{
			if (Steamworks.isReady)
			{
				if ( leaderboardHandle == null )
				{
					log("leaderboardHandle is required.  use findLeaderboard to retrieve the handle based on the leaderboard name");
					return
				}
				
				var k_ELeaderboardUploadScoreMethodNone:int = 0,
					k_ELeaderboardUploadScoreMethodKeepBest:int = 1,	// Leaderboard will keep user's best score
					k_ELeaderboardUploadScoreMethodForceUpdate:int = 2;	// Leaderboard will always replace score with specified
				
				
				var score:int = int(Math.random() * 1000);
				Steamworks.uploadLeaderboardScore(leaderboardHandle, score, k_ELeaderboardUploadScoreMethodKeepBest );
			}
		}
		
		public function handleWinAchievement(e:Event = null):void
		{
			if (Steamworks.isReady) {
				var acheivement:String = "unbound";  //50_kills
				if(!Steamworks.isAchievement(acheivement)) { 
					log("setAchievement("+acheivement+") == "+Steamworks.setAchievement(acheivement));
				} else {
					log("clearAchievement("+acheivement+") == "+Steamworks.clearAchievement(acheivement));
				}
			}
		}
		
		public function handleFileWrite(e:Event = null):void
		{
			if (Steamworks.isReady) {
				//log("setCloudEnabledForApp(false) == "+Steamworks.setCloudEnabledForApp(false) );
				log("setCloudEnabledForApp(true) == "+Steamworks.setCloudEnabledForApp(true) );
				log("isCloudEnabledForApp() == "+Steamworks.isCloudEnabledForApp() );
				if (Steamworks.fileExists('test.txt')) {
					log("fileExists test.txt");
					log("readFileFromCloud('test.txt') == "+readFileFromCloud('test.txt') );
					//lo
				} else {
					log("writeFileToCloud('test.txt','click') == "+writeFileToCloud('test.txt','click'));
				}
			}
		}
		
		public function handleFileWriteImage(e:Event = null):void
		{
			if (Steamworks.isReady) {
				//log("setCloudEnabledForApp(false) == "+Steamworks.setCloudEnabledForApp(false) );
				log("setCloudEnabledForApp(true) == "+Steamworks.setCloudEnabledForApp(true) );
				log("isCloudEnabledForApp() == "+Steamworks.isCloudEnabledForApp() );
				if (Steamworks.fileExists(TEST_FILE)) {
					log("fileExists " + TEST_FILE);
					log("readFileFromCloud(" + TEST_FILE + ") == "+readFileFromCloud(TEST_FILE) );
				} else {
					var testImg:Bitmap = new _testImg();
					log("writeBitmapToCloud(" + TEST_FILE + ") == "+writeBitmapToCloud(TEST_FILE, testImg));
				}
			}
		}
		
		public function handleEnumeratePublishedWorkshopFiles(e:Event = null):void
		{
			//enumerate published files
			log("Steamworks.enumeratePublishedWorkshopFiles" +
				
			Steamworks.enumeratePublishedWorkshopFiles(
				WorkshopEnumerationType.k_EWorkshopEnumerationTypeRecent, 
				0, 
				2, 
				30, 
				null, 
				null)
				+ " ...");
		}
		
		public function handleEnumerateUserPublishedFiles(e:Event = null):void
		{
			//enumerate published files
			log("Steamworks.enumerateUserPublishedFiles" + Steamworks.enumerateUserPublishedFiles(0) + " ..." );
		}
		public function handleEnumerateUserSubscribedFiles(e:Event = null):void
		{
			//enumerate published files
			log("Steamworks.enumerateUserSubscribedFiles" + Steamworks.enumerateUserSubscribedFiles(0) + " ..." );
		}
		
		public function handleUGCDownload(e:Event = null):void
		{
			if ( publishedFileDetails == null)
			{
				log("You must enumerate files first");
				return;
			}
			if ( publishedFileDetails.length == 0 )
			{
				log("The Enumeration returned 0 results, nothing to dowload");
				return;
			}
				
			log("handleUGCDownload " + publishedFileDetails[0].file + " " +
				Steamworks.UGCDownload(publishedFileDetails[0].file) + " ..."
			);
		}
		
		private var loader:Loader = new Loader();
		public function handleUGCRead(e:Event = null):void
		{
			if ( ugcDownloadResult == null)
			{
				log("You must download the file first");
				return;
			}
			var ba:ByteArray = new ByteArray();
			ba.position = 0;
			ba.length = ugcDownloadResult.sizeInBytes;
			
			log("handleUGCRead " + ugcDownloadResult.fileName + " " +
				Steamworks.UGCRead(ugcDownloadResult.file, ba, ugcDownloadResult.sizeInBytes)
			);
			
			//this is an example to read a PNG/JPG in using Loader loadBytes method
			
			loader.contentLoaderInfo.addEventListener(Event.INIT, onBytesLoaded);
			loader.loadBytes(ba);
			addChild(loader);
		}
		private function onBytesLoaded(e:Event):void {
		   log(loader.width +" " + loader.height);
		   // bitmapData.draw(loader);
		}
		
		public function handleFileShare(e:Event = null):void
		{
			//test share
			log("Steamworks.fileShare(" + TEST_FILE + ")  == " + Steamworks.fileShare(TEST_FILE) + " ...");
		}
		
		public function handleChangePublishedFile(e:Event = null):void
		{
			//change the cloud file to an updated one
			log("handleChangePublishedFile" + writeFileToCloud(TEST_FILE, "new updated data" + new Date().date + "-" + new Date().hours +  "-" + new Date().minutes ) );
		}
		
		public function handleOpenWorkshop(e:Event = null):void
		{
			//navigateToURL(new URLRequest("steam://appnews/480"));
			navigateToURL(new URLRequest("steam://friends/"));
		}
		
		public function handleDeletePublishedFile(e:Event = null):void
		{
			//test upload
			if ( !publishedWorkshopFileId )
			{
				log("ERROR - publish a file first to get a reference to the published file id");
				return
			}
			
			log("handleDeletePublishedFile " + Steamworks.deletePublishedFile(publishedWorkshopFileId) + " ...");
		}
		
		public function handleUpdatePublishedFile(e:Event = null):void
		{
			//test upload
			if ( !publishedWorkshopFileId )
			{
				log("ERROR - publish a file first to get a reference to the published file id");
				return
			}
			log("handleUpdatePublishedFile:");
			var publishFileId:String = this.publishedWorkshopFileId;
			var updateHandle:String = Steamworks.createPublishedFileUpdateRequest(publishFileId);
			log(" createPublishedFileUpdateRequest " + updateHandle);
			log(" updatePublishedFileTitle " + Steamworks.updatePublishedFileTitle(updateHandle, "updated file title"));
			log(" updatePublishedFileDescription " + Steamworks.updatePublishedFileDescription(updateHandle, "Updated file description") );
			
			//the updated file needs to be writted/shared to the cloud first using FileWrite and FileShare
			log(" updatePublishedFileDescription " + Steamworks.updatePublishedFileFile(updateHandle, TEST_FILE) );
			
			log(" commitPublishedFileUpdate " + Steamworks.commitPublishedFileUpdate(updateHandle) + " ..." );
		}
		
		public function handlePublishWorkshopFile(e:Event = null ):void
		{
			
			// public workshop file 
			log("Steamworks.publishWorkshopFile(" + TEST_FILE + ")  == " 
				+ Steamworks.publishWorkshopFile(TEST_FILE,
					TEST_FILE, 
					"Test pub file " + new Date().date + "-" + new Date().hours +  "-" + new Date().minutes, 
					"Test publish file", 
					"Testing publish file", 
					RemoteStoragePublishedFileVisibility.k_ERemoteStoragePublishedFileVisibilityPublic, 
					["Map"], 
					0)
				+ " ...");
				
		}
		
		public function getPublishedFileDetails(enumeratePublishedFileResult:EnumerateWorkshopFilesResult):void
		{
			
			//log("getPublishedFileDetails " + Steamworks.getPublishedFileDetails(enumeratePublishedFileResult[0]) );
			log("getPublishedFileDetails " + Steamworks.batchGetPublishedFileDetails(enumeratePublishedFileResult.publishedFileId) );
			
		}

		public function onClick(e:MouseEvent):void{
			log("--click--");
			if(Steamworks.isReady){
				
				//Steamworks.storeStats();
			} else {
				log("not able to set achievement\n");
			}
		}
		

		public function onSteamResponse(e:SteamEvent):void{
			switch(e.req_type){
				case SteamConstants.RESPONSE_LeaderboardFindResult:
					leaderboardHandle = String(e.data);
					log("RESPONSE_LeaderboardFindResult: " + e.response + " " + SteamResult.getMessage(e.response) + " " + e.data + ( e.data == "0" ? " Leaderboard not found, check the name" : " found"));
					break;
				case SteamConstants.RESPONSE_LeaderboardScoreUploaded:
					var leaderboardScoreUploaded:LeaderboardScoreUploaded = e.data as LeaderboardScoreUploaded;
					log("RESPONSE_LeaderboardScoreUploaded: " + e.response + " " + SteamResult.getMessage(e.response) + " " + e.data );
					if ( leaderboardScoreUploaded != null )
						log("         : " + leaderboardScoreUploaded.steamLeaderboard + 
						" success: " + leaderboardScoreUploaded.success + 
						" scoreChanged: " + leaderboardScoreUploaded.scoreChanged + 
						" score: " + leaderboardScoreUploaded.score + 
						" rankNew:" + leaderboardScoreUploaded.globalRankNew + 
						" rankPrevious:"+ leaderboardScoreUploaded.globalRankPrevious);
					break;
				case SteamConstants.RESPONSE_LeaderboardScoresDownloaded:
					log("RESPONSE_LeaderboardScoresDownloaded: " + e.response + " " + SteamResult.getMessage(e.response) + " " + e.data );
					if ( e.data && e.data.entries )
					{
						var entries:Array = e.data.entries as Array;
						for ( var i:int = 0; i < entries.length; i++ )
						{
							var entry:Object = entries[i];
							log(entry.globalRank + ": " + entry.personaName + " " + entry.score );
						}
					}
					break;
				case SteamConstants.RESPONSE_OnUserStatsStored:
					log("RESPONSE_OnUserStatsStored: "+e.response + " " + SteamResult.getMessage(e.response));
					break;
				case SteamConstants.RESPONSE_OnUserStatsReceived:
					log("RESPONSE_OnUserStatsReceived: "+e.response + " " + SteamResult.getMessage(e.response));
					break;
				case SteamConstants.RESPONSE_OnAchievementStored:
					log("RESPONSE_OnAchievementStored: "+e.response + " " + SteamResult.getMessage(e.response));
					break;
				case SteamConstants.RESPONSE_OnUserFileShare:
					log("RESPONSE_OnUserFileShare: "+e.response + " " + SteamResult.getMessage(e.response));
					break;
				case SteamConstants.RESPONSE_OnPublishWorkshopFile:
					log("RESPONSE_OnPublishWorkshopFile: " + e.data + " " + SteamResult.getMessage(e.response));
					publishedWorkshopFileId = String(e.data);
					break;
				case SteamConstants.RESPONSE_OnDeletePublishedFile:
					log("RESPONSE_OnDeletePublishedFile: " + e.response + " " + SteamResult.getMessage(e.response));
					break;
				case SteamConstants.RESPONSE_EnumeratePublishedWorkshopFiles:
				case SteamConstants.RESPONSE_EnumerateUserPublishedFiles:
				case SteamConstants.RESPONSE_EnumerateUserSubscribedFiles:
					log("RESPONSE_EnumeratePublishedWorkshopFiles: " + e.data + " " + e.response + " "  + SteamResult.getMessage(e.response));
					
					//get the file details for the list
					var filesResult:EnumerateWorkshopFilesResult = e.data as EnumerateWorkshopFilesResult;
					if( filesResult.publishedFileId.length > 0 )
						getPublishedFileDetails(filesResult);
					break;
				case SteamConstants.RESPONSE_GetPublishedFileDetails:
					log("RESPONSE_GetPublishedFileDetails: "+e.data + " " + SteamResult.getMessage(e.response));
					break;
				case SteamConstants.RESPONSE_BatchGetPublishedFileDetails:
					log("RESPONSE_BatchGetPublishedFileDetails: " + e.data) + " " + SteamResult.getMessage(e.response);
					publishedFileDetails = e.data;
					break;
				case SteamConstants.RESPONSE_UGCDownload:
					log("RESPONSE_UGCDownload: " + e.data + " " + SteamResult.getMessage(e.response));
					ugcDownloadResult = e.data as UGCResult;
					break;
				default:
			log("STEAMresponse type:"+e.req_type+" response:"+e.response + " " + SteamResult.getMessage(e.response));
			}
		}
		
		private var prevY:int = 10;
		private function addButton(label:String, onClick:Function):void
		{
			var tf:TextField = new TextField();
			tf.text = label;
			tf.width = 190;
			tf.height = 25;
			tf.x = 5;
			tf.y = 5;
			tf.selectable = false;
			tf.mouseEnabled = false;
			tf.setTextFormat(new TextFormat("Arial", 14, 0x000000, null, null, null, null, null, "center") );
			
			var s:Sprite = new Sprite();
			s.graphics.beginFill(0xcccccc);
			s.graphics.drawRoundRect(0, 0, 200, 30, 6, 6);
			s.graphics.endFill();
			s.addChild(tf);
			s.useHandCursor = true;
			s.buttonMode = true;
			s.addEventListener( MouseEvent.CLICK, onClick );
			s.x = 5;
			s.y = prevY;
			prevY += s.height + 5;
			addChild( s );
		}
		
	}
	
}