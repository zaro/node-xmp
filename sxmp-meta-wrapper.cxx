#include "node-xmp.h"

using namespace std;

NAN_MODULE_INIT(SXMPMetaWrapper::Init) {
  SXMPMeta::Initialize();

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("XMPMeta").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "Serialize", Serialize);
  Nan::SetPrototypeMethod(tpl, "Properties", Properties);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("XMPMeta").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
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

NAN_METHOD(SXMPMetaWrapper::New) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    Nan::MaybeLocal<v8::String> maybeString = Nan::To<v8::String>(info[0]);
    SXMPMetaWrapper *obj;
    if(maybeString.IsEmpty()){
      obj = new SXMPMetaWrapper();
    } else {
      v8::Local<v8::String> value;
      maybeString.ToLocal(&value);
      char buffer[value->Utf8Length() + 1];
      value->WriteUtf8(buffer);
      obj = new SXMPMetaWrapper(buffer, value->Utf8Length());
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
  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  string xmpBuffer;
  obj->meta.SerializeToBuffer(&xmpBuffer);
  Nan::MaybeLocal<v8::String> rval = Nan::New<v8::String>(xmpBuffer);
  v8::Local<v8::Value> value;
  rval.ToLocal(&value);
  info.GetReturnValue().Set(value);
}


NAN_METHOD(SXMPMetaWrapper::Properties) {
  Nan::Maybe<XMP_OptionBits> maybeOpts = Nan::To<XMP_OptionBits>(info[0]);

  SXMPMetaWrapper* obj = Nan::ObjectWrap::Unwrap<SXMPMetaWrapper>(info.This());
  v8::Local<v8::Object> rval = Nan::New<v8::Object>();
  size_t idx = 0;
  SXMPIterator iter( obj->meta , maybeOpts.FromMaybe(0));
  string schemaNS, propPath, propVal;
  while( iter.Next( &schemaNS, &propPath, &propVal )){
    rval->Set(Nan::New(propPath).ToLocalChecked(), Nan::New(propVal).ToLocalChecked());
  }
  info.GetReturnValue().Set(rval);
}
