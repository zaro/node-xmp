const xmp = require('../');
const sf = xmp.serializeFlags.OmitAllFormatting
         | xmp.serializeFlags.OmitXMPMetaElement
         | xmp.serializeFlags.OmitPacketWrapper;

const EmptyRDF = "<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\"/> </rdf:RDF> ";
const SampleRDF=`
  <rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
    <rdf:Description rdf:about="" xmlns:dc="http://purl.org/dc/elements/1.1/">
                  <dc:title>
                     <rdf:Alt>
                       <rdf:li xml:lang="x-default">An English Title</rdf:li>
                       <rdf:li xml:lang="en-US">An English Title</rdf:li>
                       <rdf:li xml:lang="fr-FR">Un Titre Francais</rdf:li>
                     </rdf:Alt>
                  </dc:title>
                  <dc:description>
                   <rdf:Alt>
                     <rdf:li xml:lang="x-default">Green Bush</rdf:li>
                   </rdf:Alt>
                  </dc:description>
                  <dc:format>image/jpeg</dc:format>
                  <dc:creator>
                   <rdf:Seq>
                     <rdf:li>Author Name</rdf:li>
                   </rdf:Seq>
                  </dc:creator>
    </rdf:Description>
  </rdf:RDF>
`

test('empty XMPMeta, no properties', () => {
  const m = xmp.XMPMeta();
  expect(m.ListProperties()).toEqual([]);
});

test('empty XMPMeta, serialize', () => {
  const m = xmp.XMPMeta();
  expect(m.Serialize(sf, 1)).toBe(EmptyRDF);
});

test('empty XMPMeta, set/get/delete property, serialize', () => {
  const m = xmp.XMPMeta();
  m.SetProperty(xmp.namespaces.DC, 'title', 'title1');
  expect(m.GetProperty(xmp.namespaces.DC, 'title')).toBe('title1');
  expect(m.ListProperties()).toEqual( [{
    "ns": "http://purl.org/dc/elements/1.1/",
    "path": "dc:title",
    "value": "title1",
  }]);
  expect(m.Serialize(sf, 1)).toEqual(
    "<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\"> <dc:title>title1</dc:title> </rdf:Description> </rdf:RDF> "
  );
  m.DeleteProperty(xmp.namespaces.DC, 'title');
  expect(m.DoesPropertyExist(xmp.namespaces.DC, 'title')).toBe(false);
  expect(m.GetProperty(xmp.namespaces.DC, 'title')).toBe(undefined);
  expect(m.Serialize(sf, 1)).toBe(EmptyRDF);
});

test('parse RDF', () => {
  const m = xmp.XMPMeta(SampleRDF);
  expect(m.GetArrayItem(xmp.namespaces.DC, 'title', 1)).toBe('An English Title');
});
