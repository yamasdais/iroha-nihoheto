from functools import partial, reduce
texts = ['abc', 'd\nef', 'ghi']
s = (c for c in texts)

def splitline(src):
    return reduce(lambda f, s : f + s, src).splitlines()


for x in splitline(s):
    print("L:{0}".format(x))

#print("{0}".format(splitline(texts)))
