/*
	Windows - sample tests and examples.
*/


#include "pathjoin.h"
#include "userinformation.h"


#include <iostream>


void ShowCurrentProcessSid() {
  UserInformation user_info{ };
  if (user_info.NoError()) {
    auto sid = user_info.StringSid();
    if (!user_info.NoError()) {
      fwprintf(stderr, L"Failed to get string sid (%i)\n", user_info.ErrorCode());
      return;
    }
    auto name = user_info.UserName();
    if (!user_info.NoError()) {
      fwprintf(stderr, L"Failed to get name (%i)\n", user_info.ErrorCode());
      return;
    }
    std::wcout << L"User sid is " << sid << L"(" << name << L"@" << user_info.UserDomain() << L")\n";
  } else {
    fwprintf(stderr, L"Failed to initialize UserInfo (%i)\n", user_info.ErrorCode());
  }
}


int main() {
  std::wcout << L"Hello, World!\n";
  std::wstring x;
  x.assign(L"test");
  std::wcout << L"string is " << x << std::endl;
  std::wcout << PathJoin( { L"D:", L"users", L"group.txt", x } ).GetResultPath() << std::endl;
  ShowCurrentProcessSid();
  return 0;
}
