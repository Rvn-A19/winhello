// PathJoin sample implementation.


#include "pathjoin.h"


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
