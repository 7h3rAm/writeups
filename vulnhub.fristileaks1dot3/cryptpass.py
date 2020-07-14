import base64,codecs,sys

def encodeString(str):
    base64string=base64.b64encode(str)
    return codecs.encode(base64string[::-1], 'rot13')

def decodeString(str):
    return base64.b64decode(codecs.encode(str[::-1], 'rot13'))

print encodeString(sys.argv[1])
print decodeString(sys.argv[1])
