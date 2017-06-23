// Endless Online Awaken v0.0.1

#ifndef CONST_ACCOUNT_HPP_INCLUDED
#define CONST_ACCOUNT_HPP_INCLUDED

enum class AccountReply : short
{
	Exists = 1,
	NotApproved = 2,
	Created = 3,
	ChangeFailed = 5,
	Changed = 6,
	Continue = 1000 // TODO: Check this for the real value
};

enum class LoginReply : short
{
	WrongUser = 1,
	WrongUserPass = 2,
	OK = 3,
	LoggedIn = 5,
	Busy = 6
};

#endif // CONST_ACCOUNT_HPP_INCLUDED
