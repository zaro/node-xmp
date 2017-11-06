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
                 <dc:format>image/jpeg</dc:format>
                 <dc:creator>
                   <rdf:Seq>
                     <rdf:li>Author Name</rdf:li>
                   </rdf:Seq>
                 </dc:creator>
               </rdf:Description>
       </rdf:RDF>
`

m = xmp.XMPMeta(rdf)
console.log(m.Serialize());

console.log(xmp.namespaces);
