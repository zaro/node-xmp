const xmp = require('../');

test('namespace/prefix lookup', () => {
  //expect(sum(1, 2)).toBe(3);
  for(let nsUri of Object.values(xmp.namespaces)){
    const prefix = xmp.GetNamespacePrefix(nsUri);
    const uri = xmp.GetNamespaceURI(prefix);
    expect(uri).toBe(nsUri);
  }
});

test('register/delete namespace', () => {
  const nsPrefix = 'testPrefix:';
  const nsUri = 'http://test.uri';
  const prefix = xmp.RegisterNamespace(nsUri, nsPrefix);
  expect( xmp.GetNamespacePrefix(nsUri) ).toBe(nsPrefix);
  expect( xmp.GetNamespaceURI(nsPrefix) ).toBe(nsUri);
  // xmp.DeleteNamespace(nsUri);
  // expect( xmp.GetNamespacePrefix(nsUri) ).toBe(undefined);
});
