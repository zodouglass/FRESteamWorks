package com.amanitadesign.steam 
{
	public class LeaderboardScoreUploaded 
	{
		public var steamLeaderboard:String;
		public var success:Boolean;
		public var scoreChanged:Boolean;
		public var score:int;
		public var globalRankNew:int;
		public var globalRankPrevious:int;
		
		public function LeaderboardScoreUploaded(initObj:Object )
		{
			if ( initObj != null )
			{
				this.steamLeaderboard 	= initObj.steamLeaderboard;
				this.success 			= initObj.success;
				this.scoreChanged		= initObj.scoreChanged;
				this.score 				= initObj.score;
				this.globalRankNew 		= initObj.globalRankNew;
				this.globalRankPrevious = initObj.globalRankPrevious;
			}
		}
		
	}

}