#include <nan.h>
#define TXMP_STRING_TYPE	std::string
#define XMP_INCLUDE_XMPFILES 1

#include "XMP.hpp"
#include "XMP_Const.h"


class SXMPMetaWrapper : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);
    static Nan::Persistent<v8::Function> constructor;
    SXMPMeta meta;

  private:
    SXMPMetaWrapper();
    SXMPMetaWrapper(const char *buffer, size_t buffer_len);
    SXMPMetaWrapper(const SXMPMeta& other);

    ~SXMPMetaWrapper();

    static NAN_METHOD(New);
    static NAN_METHOD(Serialize);
    static NAN_METHOD(Properties);
};

class SXMPFilesWrapper : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);
    static Nan::Persistent<v8::Function> constructor;
    SXMPFiles files;

  private:
    SXMPFilesWrapper();
    ~SXMPFilesWrapper();

    static NAN_METHOD(New);
    static NAN_METHOD(OpenFile);
    static NAN_METHOD(GetFileInfo);
    static NAN_METHOD(GetXMP);
};

v8::Local<v8::Object> BuildNsMap();
v8::Local<v8::Object> BuildIteratorOptions();
