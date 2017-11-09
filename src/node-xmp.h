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
    SXMPMetaWrapper(const char *buffer, uint32_t buffer_len);
    SXMPMetaWrapper(const SXMPMeta& other);

    ~SXMPMetaWrapper();

    static NAN_METHOD(RegisterNamespace);
    static NAN_METHOD(GetNamespacePrefix);
    static NAN_METHOD(GetNamespaceURI);
    static NAN_METHOD(DeleteNamespace);

    static NAN_METHOD(New);
    static NAN_METHOD(Serialize);
    static NAN_METHOD(ListProperties);

    static NAN_METHOD(GetProperty);
    static NAN_METHOD(GetArrayItem);
    static NAN_METHOD(GetStructField);
    static NAN_METHOD(GetQualifier);

    static NAN_METHOD(SetProperty);
    static NAN_METHOD(SetArrayItem);
    static NAN_METHOD(AppendArrayItem);
    static NAN_METHOD(SetStructField);
    static NAN_METHOD(SetQualifier);

    static NAN_METHOD(DeleteProperty);
    static NAN_METHOD(DeleteArrayItem);
    static NAN_METHOD(DeleteStructField);
    static NAN_METHOD(DeleteQualifier);

    static NAN_METHOD(DoesPropertyExist);
    static NAN_METHOD(DoesArrayItemExist);
    static NAN_METHOD(DoesStructFieldExist);
    static NAN_METHOD(DoesQualifierExist);

    static NAN_METHOD(Sort);
    static NAN_METHOD(Erase);

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
    static NAN_METHOD(IsMetadataWritable);
    static NAN_METHOD(CloseFile);
    static NAN_METHOD(GetFileInfo);
    static NAN_METHOD(GetXMP);
    static NAN_METHOD(PutXMP);
    static NAN_METHOD(CanPutXMP);
};

v8::Local<v8::Object> BuildNsMap();
v8::Local<v8::Object> BuildIteratorOptions();
v8::Local<v8::Object> BuildFileOptions();
v8::Local<v8::Object> BuildSerializeOptions();
