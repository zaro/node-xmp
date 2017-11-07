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

#define ADD_ITER_OPT_TO_MAP_O(name, obj) \
  obj->Set(Nan::New(#name).ToLocalChecked(), Nan::New(kXMP_Iter ## name))

#define ADD_ITER_OPT_TO_MAP(name) ADD_ITER_OPT_TO_MAP_O(name, obj)

v8::Local<v8::Object> BuildIteratorOptions() {
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  ADD_ITER_OPT_TO_MAP(JustChildren);
  ADD_ITER_OPT_TO_MAP(JustLeafNodes);
  ADD_ITER_OPT_TO_MAP(JustLeafName);
  ADD_ITER_OPT_TO_MAP(OmitQualifiers);

  return obj;
}

#define ADD_SER_OPT_TO_MAP_O(name, obj) \
  obj->Set(Nan::New(#name).ToLocalChecked(), Nan::New(kXMP_ ## name))

#define ADD_SER_OPT_TO_MAP(name) ADD_SER_OPT_TO_MAP_O(name, obj)

v8::Local<v8::Object> BuildSerializeOptions() {
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  /// Omit the XML packet wrapper.
  ADD_SER_OPT_TO_MAP(OmitPacketWrapper);
  /// Default is a writeable packet.
  ADD_SER_OPT_TO_MAP(ReadOnlyPacket);
  /// Use a compact form of RDF.
  ADD_SER_OPT_TO_MAP(UseCompactFormat);
  /// Use a canonical form of RDF.
  ADD_SER_OPT_TO_MAP(UseCanonicalFormat);
  /// Include a padding allowance for a thumbnail image.
  ADD_SER_OPT_TO_MAP(IncludeThumbnailPad);
  /// The padding parameter is the overall packet length.
  ADD_SER_OPT_TO_MAP(ExactPacketLength);
  /// Omit all formatting whitespace.
  ADD_SER_OPT_TO_MAP(OmitAllFormatting);
  /// Omit the x:xmpmeta element surrounding the rdf:RDF element.
  ADD_SER_OPT_TO_MAP(OmitXMPMetaElement);
  /// Include a rdf Hash and Merged flag in x:xmpmeta element.
  ADD_SER_OPT_TO_MAP(IncludeRDFHash);
  /// Use UTF8 encoding
  ADD_SER_OPT_TO_MAP(EncodeUTF8);
  /// Use UTF16 big-endian encoding
  ADD_SER_OPT_TO_MAP(EncodeUTF16Big);
  /// Use UTF16 little-endian encoding
  ADD_SER_OPT_TO_MAP(EncodeUTF16Little);
  /// Use UTF32 big-endian encoding
  ADD_SER_OPT_TO_MAP(EncodeUTF32Big);
  /// Use UTF13 little-endian encoding
  ADD_SER_OPT_TO_MAP(EncodeUTF32Little);

  return obj;
}


#define ADD_OPT_BIT_TO_MAP_O(name, obj) \
  obj->Set(Nan::New(#name).ToLocalChecked(), Nan::New(kXMPFiles_ ## name))

#define ADD_OPT_BIT_TO_MAP(name) ADD_OPT_BIT_TO_MAP_O(name, obj)

v8::Local<v8::Object> BuildFileOptions() {
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();

  /// Open for read-only access.
  ADD_OPT_BIT_TO_MAP(OpenForRead);
  /// Open for reading and writing.
  ADD_OPT_BIT_TO_MAP(OpenForUpdate);
  /// Only the XMP is wanted, allows space/time optimizations.
  ADD_OPT_BIT_TO_MAP(OpenOnlyXMP);
  /// Force use of the given handler (format), do not even verify the format.
  ADD_OPT_BIT_TO_MAP(ForceGivenHandler);
  /// Be strict about only attempting to use the designated file handler, no fallback to other handlers.
  ADD_OPT_BIT_TO_MAP(OpenStrictly);
  /// Require the use of a smart handler.
  ADD_OPT_BIT_TO_MAP(OpenUseSmartHandler);
  /// Force packet scanning, do not use a smart handler.
  ADD_OPT_BIT_TO_MAP(OpenUsePacketScanning);
  /// Only packet scan files "known" to need scanning.
  ADD_OPT_BIT_TO_MAP(OpenLimitedScanning);
  /// Attempt to repair a file opened for update, default is to not open (throw an exception).
  ADD_OPT_BIT_TO_MAP(OpenRepairFile);
  /// When updating a file, spend the effort necessary to optimize file layout.
  ADD_OPT_BIT_TO_MAP(OptimizeFileLayout);
  /// Write into a temporary file and swap for crash safety.
  ADD_OPT_BIT_TO_MAP(UpdateSafely);

  return obj;
}
