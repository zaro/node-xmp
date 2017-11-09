#include <dlfcn.h>

string getModuleDir(){
  Dl_info dl_info;
  dladdr((void *)SXMPFilesWrapper::Init, &dl_info);
  string moduleDir(dl_info.dli_fname);
  size_t lastSlash = moduleDir.rfind('/');
  moduleDir.erase(lastSlash + 1);
  return moduleDir;
}
