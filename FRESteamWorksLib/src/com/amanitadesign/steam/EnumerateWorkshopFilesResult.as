package com.amanitadesign.steam 
{
	/**
	 * Result of Enumerate Workshop Files
	 */
	public class EnumerateWorkshopFilesResult 
	{
		
		public var resultsReturned:uint;
		public var totalResultCount:uint;
		public var publishedFileId:Array;
		public var score:Array;
		
		public function EnumerateWorkshopFilesResult( value:Object )
		{
			this.resultsReturned = value.resultsReturned;
			this.totalResultCount = value.totalResultCount;
			this.publishedFileId = value.publishedFileId;
			this.score = value.score;
		}
		
		public function toString():String
		{
			var l:int = publishedFileId == null ? 0 : publishedFileId.length;
			return "resultsReturned " + resultsReturned + ", totalResultCount " + totalResultCount + ", length " + l + " " + publishedFileId + " scores " + score;
		}
	}

}