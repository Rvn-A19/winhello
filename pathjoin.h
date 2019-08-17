// Example sample class. Use it to build windows path from strings.


#ifndef PATHJOIN_H_
#define PATHJOIN_H_


#include "bydefault.h"


#include <string>


class PathJoin {
public:
  PathJoin(std::initializer_list<const std::wstring> components);
  std::wstring GetResultPath();

private:
  std::wstring result_path;
};


#endif // PATHJOIN_H_
