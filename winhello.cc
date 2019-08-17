/*
	Windows - hello world.
*/


#include "pathjoin.h"


#include <iostream>


#include <memory>


#include <Windows.h>
#include <sddl.h>


std::wstring GetNameBySid(PSID target_sid) {
  std::wstring result;
  auto user = std::make_unique<WCHAR[]>(kBufferSize);
  DWORD user_len = kBufferSize;
  auto domain = std::make_unique<WCHAR[]>(kBufferSize);
  DWORD domain_len = kBufferSize;
  SID_NAME_USE sid_name_use;
  if (LookupAccountSidW(nullptr, target_sid, user.get(), &user_len, domain.get(), &domain_len, &sid_name_use)) {
    result.assign(user.get()).append(L"@").append(domain.get());
  }
  return result;
}


void ShowCurrentProcessSid() {
  HANDLE token = GetCurrentProcess();
  
  auto token_autoptr = std::unique_ptr<HANDLE, void(*)(HANDLE *)>{ &token,
      [](HANDLE *myhandle) { CloseHandle(*myhandle); } };

  if (!OpenProcessToken(token, TOKEN_READ, &token)) {
    fwprintf(stderr, L"Failed to open process token (%i)\n", GetLastError());
    return;
  }
  DWORD total_return_length = 0;
  auto data_buffer = std::make_unique<BYTE[]>(kBufferSize);
  if (!GetTokenInformation(token, TokenUser, data_buffer.get(), kBufferSize, &total_return_length)) {
    fwprintf(stderr, L"Unable to get token information (%i)\n", GetLastError());
    return;
  }
  
  PTOKEN_USER token_info = (PTOKEN_USER)data_buffer.get();
  LPWSTR string_sid;

  if (!ConvertSidToStringSidW(token_info->User.Sid, &string_sid)) {
    fwprintf(stderr, L"Convert to string sid error (%i)\n", GetLastError());
    return;
  }

  std::wcout << L"Process user sid is " << string_sid << L"(" << GetNameBySid(token_info->User.Sid) << L")" << std::endl;

  LocalFree(string_sid);

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
