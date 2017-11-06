#include "node-xmp.h"

#include "XMP.incl_cpp"		//include in EXACTLY one source file (i.e. main, in Action gets you trouble...)


NAN_METHOD(XmpHello) {
  info.GetReturnValue().Set(Nan::New("world").ToLocalChecked());
}

NAN_MODULE_INIT(InitAll){
  Nan::Set(
    target,
    Nan::New("namespaces").ToLocalChecked(),
    BuildNsMap()
  );
  SXMPMetaWrapper::Init(target);

  SXMPMeta::Initialize();
}


NODE_MODULE(xmp, InitAll)
