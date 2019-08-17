// UserInfo example class.


#ifndef USERINFORMATION_H_
#define USERINFORMATION_H_


#include "bydefault.h"


#include <memory>
#include <string>


#include <Windows.h>
#include <sddl.h>


class UserInformation {
public:
  UserInformation();
  bool NoError();
  unsigned long ErrorCode();
  const PSID BinarySid();
  std::wstring StringSid();
  std::wstring UserName();
  std::wstring UserDomain();
  ~UserInformation();

private:
  HANDLE process_token;
  BOOL no_error;
  
  PTOKEN_USER token_user;

  std::unique_ptr<BYTE[]> sid_data;
  DWORD sid_data_len;

  std::wstring string_sid;
  std::wstring user_domain;
  std::wstring user_name;

  void GetUserNames();

};


#endif // USERINFORMATION_H_
