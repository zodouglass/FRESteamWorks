package com.amanitadesign.steam 
{
	/**
	 * Steam EResult codes
	 */
	public class SteamResult 
	{
		public static var k_EResultOK:uint	= 1,							// success
		k_EResultFail:uint = 2,							// generic failure 
		k_EResultNoConnection:uint = 3,					// no/failed network connection
	//	k_EResultNoConnectionRetry = 4,				// OBSOLETE - removed
		k_EResultInvalidPassword:uint = 5,				// password/ticket is invalid
		k_EResultLoggedInElsewhere:uint = 6,				// same user logged in elsewhere
		k_EResultInvalidProtocolVer:uint = 7,			// protocol version is incorrect
		k_EResultInvalidParam:uint = 8,					// a parameter is incorrect
		k_EResultFileNotFound :uint= 9,					// file was not found
		k_EResultBusy:uint = 10,							// called method busy - action not taken
		k_EResultInvalidState:uint = 11,					// called object was in an invalid state
		k_EResultInvalidName:uint = 12,					// name is invalid
		k_EResultInvalidEmail:uint = 13,					// email is invalid
		k_EResultDuplicateName:uint = 14,				// name is not unique
		k_EResultAccessDenied:uint = 15,					// access is denied
		k_EResultTimeout:uint = 16,						// operation timed out
		k_EResultBanned:uint = 17,						// VAC2 banned
		k_EResultAccountNotFound:uint = 18,				// account not found
		k_EResultInvalidSteamID:uint = 19,				// steamID is invalid
		k_EResultServiceUnavailable:uint = 20,			// The requested service is currently unavailable
		k_EResultNotLoggedOn:uint = 21,					// The user is not logged on
		k_EResultPending:uint = 22,						// Request is pending (may be in process, or waiting on third party)
		k_EResultEncryptionFailure:uint = 23,			// Encryption or Decryption failed
		k_EResultInsufficientPrivilege:uint = 24,		// Insufficient privilege
		k_EResultLimitExceeded:uint = 25,				// Too much of a good thing
		k_EResultRevoked:uint = 26,						// Access has been revoked (used for revoked guest passes)
		k_EResultExpired:uint = 27,						// License/Guest pass the user is trying to access is expired
		k_EResultAlreadyRedeemed:uint = 28,				// Guest pass has already been redeemed by account, cannot be acked again
		k_EResultDuplicateRequest:uint = 29,				// The request is a duplicate and the action has already occurred in the past, ignored this time
		k_EResultAlreadyOwned:uint = 30,					// All the games in this guest pass redemption request are already owned by the user
		k_EResultIPNotFound:uint = 31,					// IP address not found
		k_EResultPersistFailed:uint = 32,				// failed to write change to the data store
		k_EResultLockingFailed:uint = 33,				// failed to acquire access lock for this operation
		k_EResultLogonSessionReplaced:uint = 34,
		k_EResultConnectFailed:uint = 35,
		k_EResultHandshakeFailed:uint = 36,
		k_EResultIOFailure:uint = 37,
		k_EResultRemoteDisconnect:uint = 38,
		k_EResultShoppingCartNotFound:uint = 39,			// failed to find the shopping cart requested
		k_EResultBlocked:uint = 40,						// a user didn't allow it
		k_EResultIgnored:uint = 41,						// target is ignoring sender
		k_EResultNoMatch:uint = 42,						// nothing matching the request found
		k_EResultAccountDisabled:uint = 43,
		k_EResultServiceReadOnly:uint = 44,				// this service is not accepting content changes right now
		k_EResultAccountNotFeatured:uint = 45,			// account doesn't have value, so this feature isn't available
		k_EResultAdministratorOK:uint = 46,				// allowed to take this action, but only because requester is admin
		k_EResultContentVersion:uint = 47,				// A Version mismatch in content transmitted within the Steam protocol.
		k_EResultTryAnotherCM:uint = 48,					// The current CM can't service the user making a request, user should try another.
		k_EResultPasswordRequiredToKickSession:uint = 49,// You are already logged in elsewhere, this cached credential login has failed.
		k_EResultAlreadyLoggedInElsewhere:uint = 50,		// You are already logged in elsewhere, you must wait
		k_EResultSuspended:uint = 51,					// Long running operation (content download) suspended/paused
		k_EResultCancelled:uint = 52,					// Operation canceled (typically by user: content download)
		k_EResultDataCorruption:uint = 53,				// Operation canceled because data is ill formed or unrecoverable
		k_EResultDiskFull:uint = 54,						// Operation canceled - not enough disk space.
		k_EResultRemoteCallFailed:uint = 55,				// an remote call or IPC call failed
		k_EResultPasswordUnset:uint = 56,				// Password could not be verified as it's unset server side
		k_EResultExternalAccountUnlinked:uint = 57,		// External account (PSN, Facebook...) is not linked to a Steam account
		k_EResultPSNTicketInvalid:uint = 58,				// PSN ticket was invalid
		k_EResultExternalAccountAlreadyLinked:uint = 59,	// External account (PSN, Facebook...) is already linked to some other account, must explicitly request to replace/delete the link first
		k_EResultRemoteFileConflict:uint = 60,			// The sync cannot resume due to a conflict between the local and remote files
		k_EResultIllegalPassword:uint = 61,				// The requested new password is not legal
		k_EResultSameAsPreviousValue:uint = 62,			// new value is the same as the old one ( secret question and answer )
		k_EResultAccountLogonDenied:uint = 63,			// account login denied due to 2nd factor authentication failure
		k_EResultCannotUseOldPassword:uint = 64,			// The requested new password is not legal
		k_EResultInvalidLoginAuthCode:uint = 65,			// account login denied due to auth code invalid
		k_EResultAccountLogonDeniedNoMail:uint = 66,		// account login denied due to 2nd factor auth failure - and no mail has been sent
		k_EResultHardwareNotCapableOfIPT:uint = 67,		// 
		k_EResultIPTInitError:uint = 68,					// 
		k_EResultParentalControlRestricted:uint = 69,	// operation failed due to parental control restrictions for current user
		k_EResultFacebookQueryError:uint = 70,			// Facebook query returned an error
		k_EResultExpiredLoginAuthCode:uint = 71,			// account login denied due to auth code expired
		k_EResultIPLoginRestrictionFailed:uint = 72,
		k_EResultAccountLockedDown:uint = 73,
		k_EResultAccountLogonDeniedVerifiedEmailRequired:uint = 74,
		k_EResultNoMatchingURL:uint = 75,
		k_EResultBadResponse:uint = 76,					// parse failure, missing field, etc.
		k_EResultRequirePasswordReEntry:uint = 77;
		
		public static var responseMessages:Array = [
			"", //
			"success", // k_EResultOK:uint	= 1,							// success
			"generic failure ", //k_EResultFail:uint = 2,							// generic failure 
			"no/failed network connection", //k_EResultNoConnection:uint = 3,					// no/failed network connection
			"", ////	k_EResultNoConnectionRetry = 4,				// OBSOLETE - removed
			"password/ticket is invalid", //k_EResultInvalidPassword:uint = 5,				// password/ticket is invalid
			"same user logged in elsewhere", //k_EResultLoggedInElsewhere:uint = 6,				// same user logged in elsewhere
			"protocol version is incorrect", //k_EResultInvalidProtocolVer:uint = 7,			// protocol version is incorrect
			"a parameter is incorrect", //k_EResultInvalidParam:uint = 8,					// a parameter is incorrect
			"file was not found", //k_EResultFileNotFound :uint= 9,					// file was not found
			"called method busy - action not taken", //k_EResultBusy:uint = 10,							// called method busy - action not taken
			"called object was in an invalid state", //k_EResultInvalidState:uint = 11,					// called object was in an invalid state
			"name is invalid", //k_EResultInvalidName:uint = 12,					// name is invalid
			"email is invalid", //k_EResultInvalidEmail:uint = 13,					// email is invalid
			"name is not unique", //k_EResultDuplicateName:uint = 14,				// name is not unique
			"access is denied", //k_EResultAccessDenied:uint = 15,					// access is denied
			"operation timed out", //k_EResultTimeout:uint = 16,						// operation timed out
			"VAC2 banned", //k_EResultBanned:uint = 17,						// VAC2 banned
			"account not found", //k_EResultAccountNotFound:uint = 18,				// account not found
			"steamID is invalid", //k_EResultInvalidSteamID:uint = 19,				// steamID is invalid
			"The requested service is currently unavailable", //k_EResultServiceUnavailable:uint = 20,			// The requested service is currently unavailable
			"The user is not logged on", //k_EResultNotLoggedOn:uint = 21,					// The user is not logged on
			"Request is pending (may be in process, or waiting on third party)", //k_EResultPending:uint = 22,						// Request is pending (may be in process, or waiting on third party)
			"Encryption or Decryption failed", //k_EResultEncryptionFailure:uint = 23,			// Encryption or Decryption failed
			"Insufficient privilege", //k_EResultInsufficientPrivilege:uint = 24,		// Insufficient privilege
			"LimitExceeded: Too much of a good thing", //k_EResultLimitExceeded:uint = 25,				// Too much of a good thing
			"Access has been revoked (used for revoked guest passes)", //k_EResultRevoked:uint = 26,						// Access has been revoked (used for revoked guest passes)
			"License/Guest pass the user is trying to access is expired", //k_EResultExpired:uint = 27,						// License/Guest pass the user is trying to access is expired
			"Guest pass has already been redeemed by account, cannot be acked again", //k_EResultAlreadyRedeemed:uint = 28,				// Guest pass has already been redeemed by account, cannot be acked again
			"The request is a duplicate and the action has already occurred in the past, ignored this time", //k_EResultDuplicateRequest:uint = 29,				// The request is a duplicate and the action has already occurred in the past, ignored this time
			"All the games in this guest pass redemption request are already owned by the user", //k_EResultAlreadyOwned:uint = 30,					// All the games in this guest pass redemption request are already owned by the user
			"IP address not found", //k_EResultIPNotFound:uint = 31,					// IP address not found
			"failed to write change to the data store", //k_EResultPersistFailed:uint = 32,				// failed to write change to the data store
			"failed to acquire access lock for this operation", //k_EResultLockingFailed:uint = 33,				// failed to acquire access lock for this operation
			"LogonSessionReplaced", //k_EResultLogonSessionReplaced:uint = 34,
			"ConnectFailed", //k_EResultConnectFailed:uint = 35,
			"HandshakeFailed", //k_EResultHandshakeFailed:uint = 36,
			"IOFailure", //k_EResultIOFailure:uint = 37,
			"RemoteDisconnect", //k_EResultRemoteDisconnect:uint = 38,
			"failed to find the shopping cart requested", //k_EResultShoppingCartNotFound:uint = 39,			// failed to find the shopping cart requested
			"a user didn't allow it", //k_EResultBlocked:uint = 40,						// a user didn't allow it
			"target is ignoring sender", //k_EResultIgnored:uint = 41,						// target is ignoring sender
			"nothing matching the request found", //k_EResultNoMatch:uint = 42,						// nothing matching the request found
			"AccountDisabled", //k_EResultAccountDisabled:uint = 43,
			"this service is not accepting content changes right now", //k_EResultServiceReadOnly:uint = 44,				// this service is not accepting content changes right now
			"account doesn't have value, so this feature isn't available", //k_EResultAccountNotFeatured:uint = 45,			// account doesn't have value, so this feature isn't available
			"allowed to take this action, but only because requester is admin", //k_EResultAdministratorOK:uint = 46,				// allowed to take this action, but only because requester is admin
			"A Version mismatch in content transmitted within the Steam protocol.", //k_EResultContentVersion:uint = 47,				// A Version mismatch in content transmitted within the Steam protocol.
			"The current CM can't service the user making a request, user should try another.", //k_EResultTryAnotherCM:uint = 48,					// The current CM can't service the user making a request, user should try another.
			"You are already logged in elsewhere, this cached credential login has failed.", //k_EResultPasswordRequiredToKickSession:uint = 49,// You are already logged in elsewhere, this cached credential login has failed.
			"You are already logged in elsewhere, you must wait", //k_EResultAlreadyLoggedInElsewhere:uint = 50,		// You are already logged in elsewhere, you must wait
			"Long running operation (content download) suspended/paused", //k_EResultSuspended:uint = 51,					// Long running operation (content download) suspended/paused
			"Operation canceled (typically by user: content download)", //k_EResultCancelled:uint = 52,					// Operation canceled (typically by user: content download)
			"Operation canceled because data is ill formed or unrecoverable", //k_EResultDataCorruption:uint = 53,				// Operation canceled because data is ill formed or unrecoverable
			"Operation canceled - not enough disk space.", //k_EResultDiskFull:uint = 54,						// Operation canceled - not enough disk space.
			"an remote call or IPC call failed", //k_EResultRemoteCallFailed:uint = 55,				// an remote call or IPC call failed
			"Password could not be verified as it's unset server side", //k_EResultPasswordUnset:uint = 56,				// Password could not be verified as it's unset server side
			"External account (PSN, Facebook...) is not linked to a Steam account", //k_EResultExternalAccountUnlinked:uint = 57,		// External account (PSN, Facebook...) is not linked to a Steam account
			"PSN ticket was invalid", //k_EResultPSNTicketInvalid:uint = 58,				// PSN ticket was invalid
			"External account (PSN, Facebook...) is already linked to some other account, must explicitly request to replace/delete the link first", //k_EResultExternalAccountAlreadyLinked:uint = 59,	// External account (PSN, Facebook...) is already linked to some other account, must explicitly request to replace/delete the link first
			"The sync cannot resume due to a conflict between the local and remote files", //k_EResultRemoteFileConflict:uint = 60,			// The sync cannot resume due to a conflict between the local and remote files
			"The requested new password is not legal", //k_EResultIllegalPassword:uint = 61,				// The requested new password is not legal
			"new value is the same as the old one ( secret question and answer )", //k_EResultSameAsPreviousValue:uint = 62,			// new value is the same as the old one ( secret question and answer )
			"account login denied due to 2nd factor authentication failure", //k_EResultAccountLogonDenied:uint = 63,			// account login denied due to 2nd factor authentication failure
			"The requested new password is not legal", //k_EResultCannotUseOldPassword:uint = 64,			// The requested new password is not legal
			"account login denied due to auth code invalid", //k_EResultInvalidLoginAuthCode:uint = 65,			// account login denied due to auth code invalid
			"account login denied due to 2nd factor auth failure - and no mail has been sent", //k_EResultAccountLogonDeniedNoMail:uint = 66,		// account login denied due to 2nd factor auth failure - and no mail has been sent
			"HardwareNotCapableOfIPT", //k_EResultHardwareNotCapableOfIPT:uint = 67,		// 
			"IPTInitError", //k_EResultIPTInitError:uint = 68,					// 
			"operation failed due to parental control restrictions for current user", //k_EResultParentalControlRestricted:uint = 69,	// operation failed due to parental control restrictions for current user
			"Facebook query returned an error", //k_EResultFacebookQueryError:uint = 70,			// Facebook query returned an error
			"account login denied due to auth code expired", //k_EResultExpiredLoginAuthCode:uint = 71,			// account login denied due to auth code expired
			"IPLoginRestrictionFailed", //k_EResultIPLoginRestrictionFailed:uint = 72,
			"AccountLockedDown", //k_EResultAccountLockedDown:uint = 73,
			"AccountLogonDeniedVerifiedEmailRequired", //k_EResultAccountLogonDeniedVerifiedEmailRequired:uint = 74,
			"NoMatchingURL", //k_EResultNoMatchingURL:uint = 75,
			"parse failure, missing field, etc.", //k_EResultBadResponse:uint = 76,					// parse failure, missing field, etc.
			"RequirePasswordReEntry", //k_EResultRequirePasswordReEntry:uint = 77;
		];
		
		public static function getMessage(response:uint):String
		{
			return responseMessages[response];
		}
		
	}

}