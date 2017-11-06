#include "node-xmp.h"
#include <dlfcn.h>

using namespace std;

string getModuleDir(){
  Dl_info dl_info;
  dladdr((void *)SXMPFilesWrapper::Init, &dl_info);
  string moduleDir(dl_info.dli_fname);
  size_t lastSlash = moduleDir.rfind('/');
  moduleDir.erase(lastSlash + 1);
  return moduleDir;
}

NAN_MODULE_INIT(SXMPFilesWrapper::Init) {
  string moduleDir = getModuleDir();
  moduleDir += "xfplugins/";
  SXMPFiles::Initialize(moduleDir.data());

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("XMPFile").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "OpenFile", OpenFile);
  Nan::SetPrototypeMethod(tpl, "GetFileInfo", GetFileInfo);
  Nan::SetPrototypeMethod(tpl, "GetXMP", GetXMP);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("XMPFile").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Nan::Persistent<v8::Function> SXMPFilesWrapper::constructor;

SXMPFilesWrapper::SXMPFilesWrapper(){
}

SXMPFilesWrapper::~SXMPFilesWrapper() {
}

NAN_METHOD(SXMPFilesWrapper::New) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    SXMPFilesWrapper *obj;
    obj = new SXMPFilesWrapper();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 0;
    v8::Local<v8::Value> argv[argc] = {};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

NAN_METHOD(SXMPFilesWrapper::OpenFile) {
  v8::Local<v8::String> filenameArg = Nan::To<v8::String>(info[0]).ToLocalChecked();
  char filename[filenameArg->Utf8Length() + 1];
  filenameArg->WriteUtf8(filename);

  SXMPFilesWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPFilesWrapper>(info.This());
  bool r = obj->files.OpenFile(filename);
  info.GetReturnValue().Set(Nan::New(r));
}


NAN_METHOD(SXMPFilesWrapper::GetFileInfo) {
  XMP_FileFormat format;
  XMP_OptionBits openFlags, handlerFlags;
  SXMPFilesWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPFilesWrapper>(info.This());
  obj->files.GetFileInfo ( 0, &openFlags, &format, &handlerFlags );

  v8::Local<v8::Object> rval = Nan::New<v8::Object>();
  rval->Set(Nan::New("handlerFlags").ToLocalChecked(), Nan::New(handlerFlags));
  rval->Set(Nan::New("openFlags").ToLocalChecked(), Nan::New(openFlags));
  char sformat[6];
  sprintf( sformat, "%.4s", reinterpret_cast<char *>(&format));
  swap(sformat[0], sformat[3]);
  swap(sformat[1], sformat[2]);
  for(int i=3; sformat[i] == 0x20 ; --i){
    sformat[i] = 0;
  }
  rval->Set(Nan::New("format").ToLocalChecked(), Nan::New(sformat).ToLocalChecked());
  rval->Set(Nan::New("formatCode").ToLocalChecked(), Nan::New(format));


  info.GetReturnValue().Set(rval);
}

NAN_METHOD(SXMPFilesWrapper::GetXMP) {
  v8::Local<v8::Value> argv[0] = {};
  v8::Local<v8::Function> cons = Nan::New(SXMPMetaWrapper::constructor);
  v8::Local<v8::Object> metaWrapped = cons->NewInstance(0, argv);
  SXMPMetaWrapper* xmpMetaWrapper = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(metaWrapped);

  XMP_PacketInfo xmpPacket;

  SXMPFilesWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPFilesWrapper>(info.This());
  bool ok = obj->files.GetXMP ( &xmpMetaWrapper->meta, 0, 0 );

  if(ok){
    info.GetReturnValue().Set(metaWrapped);
  } else {
    info.GetReturnValue().Set(Nan::Null());
  }
}
