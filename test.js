const xmp  = require('./');

const rdf=`
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

// m = xmp.XMPMeta(rdf)
// console.log(m.Serialize());
// console.log(m.Properties());
// for(let v in xmp.iteratorOptions){
//   console.log(v);
//   console.log(m.Properties(xmp.iteratorOptions[v]));
// }
filename = 'XMPSpecificationPart1.pdf'
f = xmp.XMPFile()
//console.log('IsMetadataWritable=', f.IsMetadataWritable(filename))
f.OpenFile(filename, xmp.fileFlags.OpenForUpdate)
//f.OpenFile('XMP-Toolkit-SDK-CC201607/docs/API/arrowdown.png')
console.log(f.GetFileInfo());
m = f.GetXMP()
//console.log(m.Serialize());
props = m.ListProperties()
for(let prop of props){
    let v = m.GetProperty(prop.ns, prop.path);
    console.log(prop.path, v, prop.value === v);
    let match = prop.path.match(/\[(\d+)\]$/);
    if(match){
      let v = m.GetArrayItem(prop.ns, prop.path.replace(/\[(\d+)\]/, ''), match[1]);
      console.log('GetArrayItem =', v);
    }
}

// em = xmp.XMPMeta()
// console.log('CanPutXMP=', f.CanPutXMP(em));
// f.PutXMP(em)
// f.CloseFile()

// for(let v in xmp.iteratorOptions){
//   console.log(v);
//   console.log(m.ListProperties(xmp.iteratorOptions[v]));
// }

//console.log(xmp.namespaces);
