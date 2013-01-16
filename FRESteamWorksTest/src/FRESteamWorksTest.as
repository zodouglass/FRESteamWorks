package 
{
	import com.adobe.images.PNGEncoder;
	import com.amanitadesign.steam.FRESteamWorks;
	import com.amanitadesign.steam.RemoteStoragePublishedFileVisibility;
	import com.amanitadesign.steam.SteamConstants;
	import com.amanitadesign.steam.SteamEvent;
	import com.amanitadesign.steam.WorkshopEnumerationType;
	import flash.display.Bitmap;
	import flash.events.Event;
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
		
		[Embed(source = "../bin/test.png")]
		private var _testImg:Class;
		[Embed(source = "../bin/preview.jpg")]
		private var _previewImg:Class;
		
		
		public function FRESteamWorksTest():void 
		{
			tf = new TextField();
			tf.x = 210;
			tf.width = stage.stageWidth - tf.x;
			tf.height = stage.stageHeight;
			addChild(tf);

			//tf.addEventListener(MouseEvent.MOUSE_DOWN, onClick);
			
			addButton("Toggle Achievement", handleWinAchievement);
			addButton("Write Text to Cloud", handleFileWrite);
			addButton("Write Image to Cloud", handleFileWriteImage);
			addButton("Share Image", handleFileShare);
			addButton("Publish Image", handlePublishWorkshopFile);
			addButton("Enumerate Workshop Files", handleEnumeratePublishedWorkshopFiles);

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
		
		public function handleWinAchievement(e:Event = null):void
		{
			if(Steamworks.isReady){
				if(!Steamworks.isAchievement("50_kills")) {
					log("setAchievement('50_kills') == "+Steamworks.setAchievement("50_kills"));
				} else {
					log("clearAchievement('50_kills') == "+Steamworks.clearAchievement("50_kills"));
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
				if (Steamworks.fileExists('test.png')) {
					log("fileExists test.png");
					log("readFileFromCloud('test.png') == "+readFileFromCloud('test.png') );
				} else {
					var testImg:Bitmap = new _testImg();
					log("writeFileToCloud('test.png','click') == "+writeBitmapToCloud('test.png', testImg));
				}
			}
		}
		
		public function handleEnumeratePublishedWorkshopFiles(e:Event = null):void
		{
			//enumerate published files
			log("Steamworks.enumeratePublishedWorkshopFiles");
				
			Steamworks.enumeratePublishedWorkshopFiles(
				WorkshopEnumerationType.k_EWorkshopEnumerationTypeRecent, 
				0, 
				10, 
				30, 
				null, 
				null)
		}
		
		public function handleFileShare(e:Event = null):void
		{
			//test share
			log("Steamworks.fileShare('test.png')  == " + Steamworks.fileShare('test.png'));
		}
		
		public function handlePublishWorkshopFile(e:Event = null ):void
		{
			
			// public workshop file 
			log("Steamworks.publishWorkshopFile('test.png')  == " 
				+ Steamworks.publishWorkshopFile('test.png',
					"test.png", 
					"Test20130115120pm", 
					"testing workshop", 
					"testing workshop publish", 
					RemoteStoragePublishedFileVisibility.k_ERemoteStoragePublishedFileVisibilityPublic, 
					["test"], 
					0)
				);
				
		}
		
		public function getPublishedFileDetails(enumeratePublishedFileResult:Array):void
		{
			
			/*
			if ( enumeratePublishedFileResult != null )
			{
				for ( var i:int = 0 ; i < enumeratePublishedFileResult.length; i++ )
				{
					//trace(enumeratePublishedFileResult[i]);
					Steamworks.getPublishedFileDetails(enumeratePublishedFileResult[i]);
				}
			}
			*/
			
			//log("getPublishedFileDetails " + Steamworks.getPublishedFileDetails(enumeratePublishedFileResult[0]) );
			log("getPublishedFileDetails " + Steamworks.batchGetPublishedFileDetails(enumeratePublishedFileResult) );
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
				case SteamConstants.RESPONSE_OnUserStatsStored:
					log("RESPONSE_OnUserStatsStored: "+e.response);
					break;
				case SteamConstants.RESPONSE_OnUserStatsReceived:
					log("RESPONSE_OnUserStatsReceived: "+e.response);
					break;
				case SteamConstants.RESPONSE_OnAchievementStored:
					log("RESPONSE_OnAchievementStored: "+e.response);
					break;
				case SteamConstants.RESPONSE_OnUserFileShare:
					log("RESPONSE_OnUserFileShare: "+e.response);
					break;
				case SteamConstants.RESPONSE_EnumeratePublishedWorkshopFiles:
					log("RESPONSE_EnumeratePublishedWorkshopFiles: " + e.data);
					
					//get the file details for the list
					getPublishedFileDetails(e.data);
					break;
				case SteamConstants.RESPONSE_GetPublishedFileDetails:
					log("RESPONSE_GetPublishedFileDetails: "+e.data);
					break;
				case SteamConstants.RESPONSE_BatchGetPublishedFileDetails:
					log("RESPONSE_BatchGetPublishedFileDetails: "+e.data);
				default:
					log("STEAMresponse type:"+e.req_type+" response:"+e.response);
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