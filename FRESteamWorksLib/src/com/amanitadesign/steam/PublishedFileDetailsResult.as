package com.amanitadesign.steam 
{
	public class PublishedFileDetailsResult 
	{
		public var publishedFileId 	: String;
		public var creatorAppID 	: uint;
		public var consumerAppID 	: uint;
		public var title 			: String;
		public var description 		: String;
		public var file 			: String;
		public var previewFile 		: String;
		public var steamIDOwner 	: String;
		public var timeCreated 		: uint;
		public var timeUpdated 		: uint;
		public var visibility 		: uint;
		public var banned 			: Boolean;
		public var tags 			: String;
		public var tagsTruncated 	: Boolean;
		public var fileName 		: String;
		public var fileSize			: uint;
		public var previewFileSize 	: uint;
		public var url 				: String;
		public var fileType			: uint;
		
		/**
		 * Map properties from a generic object to a PublishedFileDetailsResult object
		 * @param	obj An Object containing the properties of PublishedFileDetailsResult
		 */
		public function PublishedFileDetailsResult(obj:Object )
		{
			if ( obj != null )
			{
				this.publishedFileId = obj.publishedFileId;
				this.creatorAppID = obj.creatorAppID;
				this.consumerAppID = obj.consumerAppID;
				this.title = obj.title;
				this.description = obj.description;
				this.file = obj.file;
				this.previewFile = obj.previewFile;
				this.steamIDOwner = obj.steamIDOwner;
				this.timeCreated = obj.timeCreated;
				this.timeUpdated = obj.timeUpdated;
				this.visibility = obj.visibility;
				this.banned = obj.banned;
				this.tags = obj.tags;
				this.tagsTruncated = obj.tagsTruncated;
				this.fileName = obj.fileName;
				this.fileSize = obj.fileSize;
				this.previewFileSize = obj.previewFileSize;
				this.fileType = obj.fileType;
			}
		}
		
		public function toString():String
		{
			return title;
		}
	}

}