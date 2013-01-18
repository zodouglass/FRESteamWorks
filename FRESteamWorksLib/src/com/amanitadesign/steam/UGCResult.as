package com.amanitadesign.steam 
{
	
	public class UGCResult 
	{
		public var file 		:String;
		public var appID  		:String;
		public var sizeInBytes 	:uint;
		public var fileName  	:String;
		public var steamIDOwner :String;
		
		/**
		 * Map properties from a generic object to a UGCResult object
		 * @param	obj An Object containing the properties of UGCResult
		 */
		public function UGCResult(obj:Object) 
		{
			if ( obj != null )
			{
				this.file = obj.file;
				this.appID = obj.appID;
				this.sizeInBytes = obj.sizeInBytes;
				this.fileName = obj.fileName;
				this.steamIDOwner = obj.steamIDOwner;
				
			}
		}
		
		public function toString():String
		{
			return fileName;
		}
		
	}

}