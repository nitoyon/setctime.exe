setctime.exe
============

The simplest command line tool to set file creation time on Windows

Usage:

    setctime.exe path1 [path2 [path3 ...]]

This tool copies modification time to creation time. An exit code of 0 means success, and non-zero means failure.


Download
--------

https://github.com/nitoyon/setctime.exe/releases


Sample
------

Ruby 2.0

```ruby
t = Time.now
File::utime(t, t, 'test.txt')
system 'setctime.exe test.txt' or abort 'failed'
```

Python 2.7

```python
import os
import sys
import time

t = time.time()
os.utime('test.txt', (t, t))
if os.system('setctime.exe test.txt') != 0:
    sys.exit('failed')
```

Perl 5.16

```perl
utime undef, 0, 'test.txt';
system('setctime test.txt') == 0 or die "failed";
```


License
-------

MIT
