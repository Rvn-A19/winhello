/*
	Windows - hello world.
*/


#include <iostream>
#include <string>
#include <memory>


#include <Windows.h>
#include <sddl.h>


const DWORD kBufferSize = 512;


class PathJoin {
public:
  PathJoin(std::initializer_list<const std::wstring> components);
  std::wstring GetResultPath();

private:
  std::wstring result_path;
};


bool EndsWith(std::wstring arg, wchar_t sym) {
  return arg[arg.size() - 1] == sym;
}


PathJoin::PathJoin(std::initializer_list<const std::wstring> components) {
  auto it = components.begin() + 1;
  result_path = *components.begin();
  for (; it + 1 != components.end(); it ++) {
    if (!EndsWith(result_path, L'\\')) {
      result_path += L"\\";
    }
    result_path += *it;
  }
  if (!EndsWith(result_path, L'\\')) {
    result_path += L"\\";
  }
  result_path += *it;
}


std::wstring PathJoin::GetResultPath() {
  return result_path;
}


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
    fprintf(stderr, "Convert to string sid error (%i)\n", GetLastError());
    return;
  }

  std::wcout << "Process user sid is " << string_sid << "(" << GetNameBySid(token_info->User.Sid) << ")" << std::endl;

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
