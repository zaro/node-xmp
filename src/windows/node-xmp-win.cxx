#include "node-xmp-os.h"
#include <windows.h>
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

std::string getModuleDir(){
  CHAR   DllPath[MAX_PATH] = {0};
  GetModuleFileNameA((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
  return DllPath;
}
