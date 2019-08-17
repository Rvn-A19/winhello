// UserInformation - implementation.


#include "userinformation.h"


UserInformation::UserInformation() {
  process_token = NULL;
  token_user = NULL;
  
  no_error = OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &process_token);
  if (!no_error) {
    process_token = NULL;
    return;
  }

  sid_data = std::make_unique<BYTE[]>(kBufferSize);
  no_error = GetTokenInformation(process_token, TokenUser, sid_data.get(), kBufferSize, &sid_data_len);
  if (no_error)
    token_user = (PTOKEN_USER)sid_data.get();
}


bool UserInformation::NoError() {
  return no_error == TRUE;
}


unsigned long UserInformation::ErrorCode() {
  return GetLastError();
}


const PSID UserInformation::BinarySid() {
  if (token_user) {
    return token_user->User.Sid;
  }
  return nullptr;
}


void UserInformation::GetUserNames() {
  if (!token_user) return;
  std::wstring result;
  auto user = std::make_unique<WCHAR[]>(kBufferSize);
  DWORD user_len = kBufferSize;
  auto domain = std::make_unique<WCHAR[]>(kBufferSize);
  DWORD domain_len = kBufferSize;
  SID_NAME_USE sid_name_use;
  no_error = LookupAccountSidW(nullptr, token_user->User.Sid, user.get(), &user_len, domain.get(), &domain_len, &sid_name_use);
  if (no_error) {
    user_name.assign(user.get());
    user_domain.assign(domain.get());
  }
}


std::wstring UserInformation::UserName() {
  if (user_name.empty()) {
    GetUserNames();
    if (!no_error) return L"";
  }
  return user_name;
}


std::wstring UserInformation::UserDomain() {
  if (user_domain.empty()) {
    GetUserNames();
    if (!no_error) return L"";
  }
  return user_domain;
}


std::wstring UserInformation::StringSid() {
  if (!token_user) return L"";
  if (string_sid.empty()) {
    LPWSTR local_string_sid;
    no_error = ConvertSidToStringSidW(token_user->User.Sid, &local_string_sid);
    if (no_error) {
      string_sid.assign(local_string_sid);
      LocalFree(local_string_sid);
    }
  }
  return string_sid;
}


UserInformation::~UserInformation() {
  if (process_token) {
    CloseHandle(process_token);
  }
}
