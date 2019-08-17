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
  PathJoin(std::initializer_list<const std::string> components);
  std::string GetResultPath();

private:
  std::string result_path;
};


bool EndsWith(std::string arg, char sym) {
  return arg[arg.size() - 1] == sym;
}


PathJoin::PathJoin(std::initializer_list<const std::string> components) {
  auto it = components.begin() + 1;
  result_path = *components.begin();
  for (; it + 1 != components.end(); it ++) {
    if (!EndsWith(result_path, '\\')) {
      result_path += "\\";
    }
    result_path += *it;
  }
  if (!EndsWith(result_path, '\\')) {
    result_path += "\\";
  }
  result_path += *it;
}


std::string PathJoin::GetResultPath() {
  return result_path;
}


std::string GetNameBySid(PSID target_sid) {
  std::string result;
  auto user = std::make_unique<char[]>(kBufferSize);
  DWORD user_len = kBufferSize;
  auto domain = std::make_unique<char[]>(kBufferSize);
  DWORD domain_len = kBufferSize;
  SID_NAME_USE sid_name_use;
  if (LookupAccountSid(nullptr, target_sid, user.get(), &user_len, domain.get(), &domain_len, &sid_name_use)) {
    result.assign(user.get()).append("@").append(domain.get());
  }
  return result;
}


void ShowCurrentProcessSid() {
  HANDLE token = GetCurrentProcess();
  
  auto token_autoptr = std::unique_ptr<HANDLE, void(*)(HANDLE *)>{ &token,
      [](HANDLE *myhandle) { CloseHandle(*myhandle); } };

  if (!OpenProcessToken(token, TOKEN_READ, &token)) {
    fprintf(stderr, "Failed to open process token (%i)\n", GetLastError());
    return;
  }
  DWORD total_return_length = 0;
  auto data_buffer = std::make_unique<BYTE[]>(kBufferSize);
  if (!GetTokenInformation(token, TokenUser, data_buffer.get(), kBufferSize, &total_return_length)) {
    fprintf(stderr, "Unable to get token information (%i)\n", GetLastError());
    return;
  }
  
  PTOKEN_USER token_info = (PTOKEN_USER)data_buffer.get();
  LPSTR string_sid;

  if (!ConvertSidToStringSid(token_info->User.Sid, &string_sid)) {
    fprintf(stderr, "Convert to string sid error (%i)\n", GetLastError());
    return;
  }

  std::cout << "Process user sid is " << string_sid << "(" << GetNameBySid(token_info->User.Sid) << ")" << std::endl;

  LocalFree(string_sid);

}


int main() {
  std::cout << "Hello, World!\n";
  std::string x;
  x.assign("test");
  std::cout << "string is " << x << std::endl;
  std::cout << PathJoin( { "D:", "users", "group.txt", x } ).GetResultPath() << std::endl;
  ShowCurrentProcessSid();
  return 0;
}
