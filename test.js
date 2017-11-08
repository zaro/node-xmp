const xmp  = require('./');

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

em = xmp.XMPMeta()
em.DeleteProperty()
console.log('CanPutXMP=', f.CanPutXMP(em));
f.PutXMP(em)
f.CloseFile()

// for(let v in xmp.iteratorOptions){
//   console.log(v);
//   console.log(m.ListProperties(xmp.iteratorOptions[v]));
// }

//console.log(xmp.namespaces);
