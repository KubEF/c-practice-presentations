#!/usr/bin/env python3
import re, sys
d = open(sys.argv[1],'rb').read().decode('utf-8','replace')
d = re.sub(r'\x1b\[[0-9;?]*[ -/]*[@-~]', '', d)
d = re.sub(r'\x1b\][^\x07\x1b]*(\x07|\x1b\\)', '', d)
d = re.sub(r'\x1b[_P^][^\x1b]*\x1b\\', '', d)
d = re.sub(r'\x1b[@-Z\\-_]', '', d)
d = d.replace('\r','')
if '--zruns' in sys.argv:
    for m in re.finditer(r'z+', d):
        print(len(m.group(0)))
else:
    sys.stdout.write(d)
