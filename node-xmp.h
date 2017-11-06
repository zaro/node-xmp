#include <nan.h>
#define TXMP_STRING_TYPE	std::string
#define XMP_INCLUDE_XMPFILES 1

#include "XMP.hpp"
#include "XMP_Const.h"


class SXMPMetaWrapper : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);

  private:
    SXMPMetaWrapper();
    SXMPMetaWrapper(const char *buffer, size_t buffer_len);
    ~SXMPMetaWrapper();

    static NAN_METHOD(New);
    static NAN_METHOD(Serialize);
    static Nan::Persistent<v8::Function> constructor;
    SXMPMeta meta;
};

v8::Local<v8::Object> BuildNsMap();
