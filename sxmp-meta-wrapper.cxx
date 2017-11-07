#include "node-xmp.h"

using namespace std;

struct XmpProperty {
  string ns, path, value;
  XmpProperty(const string &schemaNS, const string &propPath, const string &propVal)
  : ns(schemaNS), path(propPath), value(propVal) {

  }

  v8::Local<v8::Object> ToObject() {
    v8::Local<v8::Object> rval = Nan::New<v8::Object>();
    rval->Set(Nan::New("ns").ToLocalChecked(), Nan::New(ns).ToLocalChecked());
    rval->Set(Nan::New("path").ToLocalChecked(), Nan::New(path).ToLocalChecked());
    rval->Set(Nan::New("value").ToLocalChecked(), Nan::New(value).ToLocalChecked());
    return rval;
  }
};

#define ARG_LEN_CHECK(methodName, numArgs) \
  if (info.Length() < numArgs) { Nan::ThrowError(methodName " must be called with " #numArgs " argument(s)"); }


NAN_MODULE_INIT(SXMPMetaWrapper::Init) {
  SXMPMeta::Initialize();

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("XMPMeta").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "Sort", Sort);
  Nan::SetPrototypeMethod(tpl, "Erase", Erase);

  Nan::SetPrototypeMethod(tpl, "Serialize", Serialize);
  Nan::SetPrototypeMethod(tpl, "ListProperties", ListProperties);

  Nan::SetPrototypeMethod(tpl, "GetProperty", GetProperty);
  Nan::SetPrototypeMethod(tpl, "GetArrayItem", GetArrayItem);
  Nan::SetPrototypeMethod(tpl, "GetStructField", GetStructField);
  Nan::SetPrototypeMethod(tpl, "GetQualifier", GetQualifier);

  Nan::SetPrototypeMethod(tpl, "SetProperty", SetProperty);
  Nan::SetPrototypeMethod(tpl, "SetArrayItem", SetArrayItem);
  Nan::SetPrototypeMethod(tpl, "SetStructField", SetStructField);
  Nan::SetPrototypeMethod(tpl, "SetQualifier", SetQualifier);

  Nan::SetPrototypeMethod(tpl, "DeleteProperty", DeleteProperty);
  Nan::SetPrototypeMethod(tpl, "DeleteArrayItem", DeleteArrayItem);
  Nan::SetPrototypeMethod(tpl, "DeleteStructField", DeleteStructField);
  Nan::SetPrototypeMethod(tpl, "DeleteQualifier", DeleteQualifier);

  Nan::SetPrototypeMethod(tpl, "DoesPropertyExist", DoesPropertyExist);
  Nan::SetPrototypeMethod(tpl, "DoesArrayItemExist", DoesArrayItemExist);
  Nan::SetPrototypeMethod(tpl, "DoesStructFieldExist", DoesStructFieldExist);
  Nan::SetPrototypeMethod(tpl, "DoesQualifierExist", DoesQualifierExist);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("XMPMeta").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());

  Nan::SetMethod(target, "RegisterNamespace", RegisterNamespace);
  Nan::SetMethod(target, "GetNamespacePrefix", GetNamespacePrefix);
  Nan::SetMethod(target, "GetNamespaceURI", GetNamespaceURI);
  Nan::SetMethod(target, "DeleteNamespace", DeleteNamespace);

}

Nan::Persistent<v8::Function> SXMPMetaWrapper::constructor;

SXMPMetaWrapper::SXMPMetaWrapper(){
}

SXMPMetaWrapper::SXMPMetaWrapper(const char *buffer, size_t buffer_len)
  : meta(buffer, buffer_len) {
}

SXMPMetaWrapper::SXMPMetaWrapper(const SXMPMeta& other)
  : meta(other) {
}


SXMPMetaWrapper::~SXMPMetaWrapper() {
}

NAN_METHOD(SXMPMetaWrapper::RegisterNamespace){
  ARG_LEN_CHECK("RegisterNamespace", 2);
  v8::String::Utf8Value namespaceURI(Nan::To<v8::String>(info[0]).ToLocalChecked());
  v8::String::Utf8Value suggestedPrefix(Nan::To<v8::String>(info[1]).ToLocalChecked());

  string registered;
  SXMPMeta::RegisterNamespace(*namespaceURI, *suggestedPrefix, &registered);
  info.GetReturnValue().Set(Nan::New<v8::String>(registered).ToLocalChecked());
}

NAN_METHOD(SXMPMetaWrapper::GetNamespacePrefix){
  ARG_LEN_CHECK("GetNamespacePrefix", 1);
  v8::String::Utf8Value namespaceURI(Nan::To<v8::String>(info[0]).ToLocalChecked());

  string prefix;
  bool registered = SXMPMeta::GetNamespacePrefix(*namespaceURI, &prefix);
  if (registered){
    info.GetReturnValue().Set(Nan::New<v8::String>(prefix).ToLocalChecked());
  }
}

NAN_METHOD(SXMPMetaWrapper::GetNamespaceURI){
  ARG_LEN_CHECK("GetNamespaceURI", 1);
  v8::String::Utf8Value namespacePrefix(Nan::To<v8::String>(info[0]).ToLocalChecked());

  string uri;
  bool registered = SXMPMeta::GetNamespaceURI(*namespacePrefix, &uri);
  if (registered){
    info.GetReturnValue().Set(Nan::New<v8::String>(uri).ToLocalChecked());
  }
}

NAN_METHOD(SXMPMetaWrapper::DeleteNamespace){
  ARG_LEN_CHECK("DeleteNamespace", 1);
  v8::String::Utf8Value namespaceURI(Nan::To<v8::String>(info[0]).ToLocalChecked());

  try {
    SXMPMeta::DeleteNamespace(*namespaceURI);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}


NAN_METHOD(SXMPMetaWrapper::New) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    Nan::MaybeLocal<v8::String> maybeString = Nan::To<v8::String>(info[0]);
    SXMPMetaWrapper *obj;
    if(maybeString.IsEmpty()){
      obj = new SXMPMetaWrapper();
    } else {
      v8::String::Utf8Value filename(maybeString.ToLocalChecked());
      obj = new SXMPMetaWrapper(*filename, filename.length());
    }
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

NAN_METHOD(SXMPMetaWrapper::Serialize) {
  XMP_OptionBits serializeOpts = Nan::To<XMP_OptionBits>(info[0]).FromMaybe(0);
  XMP_StringLen padding = Nan::To<XMP_OptionBits>(info[1]).FromMaybe(0);

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  string xmpBuffer;
  obj->meta.SerializeToBuffer(&xmpBuffer, serializeOpts, padding);
  info.GetReturnValue().Set(Nan::New<v8::String>(xmpBuffer).ToLocalChecked());
}


NAN_METHOD(SXMPMetaWrapper::ListProperties) {
  Nan::Maybe<XMP_OptionBits> maybeOpts = Nan::To<XMP_OptionBits>(info[0]);

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  std::vector<XmpProperty> propList;

  SXMPIterator iter( obj->meta , maybeOpts.FromMaybe(0));
  string schemaNS, propPath, propVal;
  while( iter.Next( &schemaNS, &propPath, &propVal )){
    if(propPath.size() == 0 && propVal.size() == 0){
      continue;
    }
    propList.push_back({schemaNS, propPath, propVal});
  }
  v8::Local<v8::Array> rval = Nan::New<v8::Array>(propList.size());
  for(int i=0; i < propList.size(); ++i) {
    Nan::Set(rval, i, propList[i].ToObject());
  }
  info.GetReturnValue().Set(rval);
}

#define PROPERTY_ARGS(methodName) \
  ARG_LEN_CHECK(methodName, 2)\
  v8::String::Utf8Value schemaNS(Nan::To<v8::String>(info[0]).ToLocalChecked());\
  v8::String::Utf8Value propName(Nan::To<v8::String>(info[1]).ToLocalChecked())

#define ARRAYITEM_ARGS(methodName) \
  ARG_LEN_CHECK(methodName, 3)\
  v8::String::Utf8Value schemaNS(Nan::To<v8::String>(info[0]).ToLocalChecked()); \
  v8::String::Utf8Value arrayName(Nan::To<v8::String>(info[1]).ToLocalChecked()); \
  XMP_Index itemIndex = Nan::To<XMP_OptionBits>(info[2]).FromMaybe(-1);

#define STRUCTFIELD_ARGS(methodName) \
  ARG_LEN_CHECK(methodName, 4)\
  v8::String::Utf8Value schemaNS(Nan::To<v8::String>(info[0]).ToLocalChecked()); \
  v8::String::Utf8Value structName(Nan::To<v8::String>(info[1]).ToLocalChecked()); \
  v8::String::Utf8Value fieldNS(Nan::To<v8::String>(info[2]).ToLocalChecked()); \
  v8::String::Utf8Value fieldName(Nan::To<v8::String>(info[3]).ToLocalChecked())

#define QUALIFIER_ARGS(methodName) \
  ARG_LEN_CHECK(methodName, 4)\
  v8::String::Utf8Value schemaNS(Nan::To<v8::String>(info[0]).ToLocalChecked()); \
  v8::String::Utf8Value propName(Nan::To<v8::String>(info[1]).ToLocalChecked()); \
  v8::String::Utf8Value qualNS(Nan::To<v8::String>(info[2]).ToLocalChecked()); \
  v8::String::Utf8Value qualName(Nan::To<v8::String>(info[3]).ToLocalChecked())

// GET
NAN_METHOD(SXMPMetaWrapper::GetProperty) {
  PROPERTY_ARGS("GetProperty");

  std::string value;
  XMP_OptionBits options;
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    bool exists = obj->meta.GetProperty(*schemaNS,*propName, &value, &options);
    if( exists ){
      info.GetReturnValue().Set(Nan::New(value).ToLocalChecked());
    }
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::GetArrayItem) {
  ARRAYITEM_ARGS("GetArrayItem");

  std::string value;
  XMP_OptionBits options;
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    bool exists = obj->meta.GetArrayItem(*schemaNS,*arrayName, itemIndex, &value, &options);
    if( exists ){
      info.GetReturnValue().Set(Nan::New(value).ToLocalChecked());
    }
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::GetStructField) {
  STRUCTFIELD_ARGS("GetStructField");

  std::string value;
  XMP_OptionBits options;
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    bool exists = obj->meta.GetStructField(*schemaNS,*structName, *fieldNS, *fieldName, &value, &options);
    if( exists ){
      info.GetReturnValue().Set(Nan::New(value).ToLocalChecked());
    }
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::GetQualifier) {
  QUALIFIER_ARGS("GetQualifier");

  std::string value;
  XMP_OptionBits options;
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    bool exists = obj->meta.GetQualifier(*schemaNS,*propName, *qualNS, *qualName, &value, &options);
    if( exists ){
      info.GetReturnValue().Set(Nan::New(value).ToLocalChecked());
    }
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

// SET
NAN_METHOD(SXMPMetaWrapper::SetProperty) {
  PROPERTY_ARGS("SetProperty");
  v8::String::Utf8Value propValue(Nan::To<v8::String>(info[2]).ToLocalChecked());

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    obj->meta.SetProperty(*schemaNS,*propName, *propValue);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::SetArrayItem) {
  ARRAYITEM_ARGS("SetArrayItem");
  v8::String::Utf8Value itemValue(Nan::To<v8::String>(info[3]).ToLocalChecked());

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    obj->meta.SetArrayItem(*schemaNS,*arrayName, itemIndex, *itemValue);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::AppendArrayItem) {
  ARG_LEN_CHECK("AppendArrayItem", 3);
  v8::String::Utf8Value schemaNS(Nan::To<v8::String>(info[0]).ToLocalChecked());
  v8::String::Utf8Value arrayName(Nan::To<v8::String>(info[1]).ToLocalChecked());
  v8::String::Utf8Value itemValue(Nan::To<v8::String>(info[2]).ToLocalChecked());

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    obj->meta.AppendArrayItem(*schemaNS,*arrayName, 0, *itemValue);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::SetStructField) {
  STRUCTFIELD_ARGS("SetStructField");
  v8::String::Utf8Value fieldValue(Nan::To<v8::String>(info[4]).ToLocalChecked());

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    obj->meta.SetStructField(*schemaNS,*structName, *fieldNS, *fieldName, *fieldValue);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::SetQualifier) {
  QUALIFIER_ARGS("SetQualifier");
  v8::String::Utf8Value qualValue(Nan::To<v8::String>(info[4]).ToLocalChecked());

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    obj->meta.SetQualifier(*schemaNS,*propName, *qualNS, *qualName, *qualValue);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}


// DELETE
NAN_METHOD(SXMPMetaWrapper::DeleteProperty) {
  PROPERTY_ARGS("DeleteProperty");

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    obj->meta.DeleteProperty(*schemaNS,*propName);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::DeleteArrayItem) {
  ARRAYITEM_ARGS("DeleteArrayItem");

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    obj->meta.DeleteArrayItem(*schemaNS,*arrayName, itemIndex);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::DeleteStructField) {
  STRUCTFIELD_ARGS("DeleteStructField");

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    obj->meta.DeleteStructField(*schemaNS,*structName, *fieldNS, *fieldName);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::DeleteQualifier) {
  QUALIFIER_ARGS("DeleteQualifier");

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    obj->meta.DeleteQualifier(*schemaNS,*propName, *qualNS, *qualName);
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

// Exist
NAN_METHOD(SXMPMetaWrapper::DoesPropertyExist) {
  PROPERTY_ARGS("DoesPropertyExist");

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    bool exists = obj->meta.DoesPropertyExist(*schemaNS,*propName);
    info.GetReturnValue().Set(Nan::New(exists));
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::DoesArrayItemExist) {
  ARRAYITEM_ARGS("DoesArrayItemExist");

  std::string value;
  XMP_OptionBits options;
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    bool exists = obj->meta.DoesArrayItemExist(*schemaNS,*arrayName, itemIndex);
    info.GetReturnValue().Set(Nan::New(exists));
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::DoesStructFieldExist) {
  STRUCTFIELD_ARGS("DoesStructFieldExist");

  std::string value;
  XMP_OptionBits options;
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    bool exists = obj->meta.DoesStructFieldExist(*schemaNS,*structName, *fieldNS, *fieldName);
    info.GetReturnValue().Set(Nan::New(exists));
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}

NAN_METHOD(SXMPMetaWrapper::DoesQualifierExist) {
  QUALIFIER_ARGS("DoesQualifierExist");

  std::string value;
  XMP_OptionBits options;
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  try {
    bool exists = obj->meta.DoesQualifierExist(*schemaNS,*propName, *qualNS, *qualName);
    info.GetReturnValue().Set(Nan::New(exists));
  } catch(const XMP_Error &e){
    Nan::ThrowError(e.GetErrMsg());
  }
}



NAN_METHOD(SXMPMetaWrapper::Sort) {
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  obj->meta.Sort();
}

NAN_METHOD(SXMPMetaWrapper::Erase) {
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  obj->meta.Erase();
}
