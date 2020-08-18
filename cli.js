#!/usr/bin/env node

const xmp = require('./');
const commandLineArgs = require('command-line-args')

const optionDefinitions = [
  {name:'files', multiple: true, defaultOption: true}
]

const options = commandLineArgs(optionDefinitions);
console.log(options);
for(const filename of options.files) {
  f = xmp.XMPFile()
  console.log(f)
  f.OpenFile(filename, xmp.fileFlags.OpenForUpdate)
  m = f.GetXMP()
  console.log(m)

  props = m.ListProperties()
  for(let prop of props){
      let v = m.GetProperty(prop.ns, prop.path);
      console.log(prop.path, '=',v);
  }
  // em = xmp.XMPMeta()
  // console.log('CanPutXMP=', f.CanPutXMP(em));
  // f.PutXMP(em)
  // f.CloseFile()
}
