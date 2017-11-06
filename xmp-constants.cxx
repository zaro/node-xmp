#include "node-xmp.h"


#define ADD_NS_TO_MAP_O(name, obj) \
  obj->Set(Nan::New(#name).ToLocalChecked(), Nan::New(kXMP_NS_ ## name).ToLocalChecked())

#define ADD_NS_TO_MAP(name) ADD_NS_TO_MAP_O(name, obj)

v8::Local<v8::Object> BuildNsMap() {
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  // XML namespace constants for standard XMP schema.
  ADD_NS_TO_MAP(XMP);
  ADD_NS_TO_MAP(XMP_Rights);
  ADD_NS_TO_MAP(XMP_MM);
  ADD_NS_TO_MAP(XMP_BJ);

  ADD_NS_TO_MAP(PDF);
  ADD_NS_TO_MAP(Photoshop);
  ADD_NS_TO_MAP(PSAlbum);
  ADD_NS_TO_MAP(EXIF);
  ADD_NS_TO_MAP(EXIF_Aux);
  ADD_NS_TO_MAP(TIFF);
  ADD_NS_TO_MAP(PNG);
  ADD_NS_TO_MAP(SWF);
  ADD_NS_TO_MAP(JPEG);
  ADD_NS_TO_MAP(JP2K);
  ADD_NS_TO_MAP(CameraRaw);
  ADD_NS_TO_MAP(DM);
  ADD_NS_TO_MAP(Script);
  ADD_NS_TO_MAP(ASF);
  ADD_NS_TO_MAP(WAV);
  ADD_NS_TO_MAP(BWF);
  ADD_NS_TO_MAP(AEScart);
  ADD_NS_TO_MAP(RIFFINFO);
  ADD_NS_TO_MAP(iXML);
  ADD_NS_TO_MAP(XMP_Note);
  ADD_NS_TO_MAP(AdobeStockPhoto);
  ADD_NS_TO_MAP(CreatorAtom);
  ADD_NS_TO_MAP(ExifEX);
  // XML namespace constants for qualifiers and structured property fields.
  ADD_NS_TO_MAP(XMP_IdentifierQual);
  ADD_NS_TO_MAP(XMP_Dimensions);
  ADD_NS_TO_MAP(XMP_Text);
  ADD_NS_TO_MAP(XMP_PagedFile);
  ADD_NS_TO_MAP(XMP_Graphics);
  ADD_NS_TO_MAP(XMP_Image);
  ADD_NS_TO_MAP(XMP_Font);
  ADD_NS_TO_MAP(XMP_ResourceEvent);
  ADD_NS_TO_MAP(XMP_ResourceRef);
  ADD_NS_TO_MAP(XMP_ST_Version);
  ADD_NS_TO_MAP(XMP_ST_Job);
  ADD_NS_TO_MAP(XMP_ManifestItem);
  // Deprecated XML namespace constants
  ADD_NS_TO_MAP(XMP_T);
  ADD_NS_TO_MAP(XMP_T_PG);
  ADD_NS_TO_MAP(XMP_G_IMG);
  // XML namespace constants from outside Adobe
  ADD_NS_TO_MAP(DC);
  ADD_NS_TO_MAP(IPTCCore);
  ADD_NS_TO_MAP(IPTCExt);
  ADD_NS_TO_MAP(DICOM);
  ADD_NS_TO_MAP(PLUS);
  ADD_NS_TO_MAP(PDFA_Schema);
  ADD_NS_TO_MAP(PDFA_Property);
  ADD_NS_TO_MAP(PDFA_Type);
  ADD_NS_TO_MAP(PDFA_Field);
  ADD_NS_TO_MAP(PDFA_ID);
  ADD_NS_TO_MAP(PDFA_Extension);
  ADD_NS_TO_MAP(PDFX);
  ADD_NS_TO_MAP(PDFX_ID);

  ADD_NS_TO_MAP(RDF);
  ADD_NS_TO_MAP(XML);

  return obj;
}
