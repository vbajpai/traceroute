Compiling
---------	
- - -
	$ make
	
Running
-------	
- - -

Python Implementation:

	$ sudo src/traceroute.py www.google.com
    Warning: www.google.com has multiple addresses; using 74.125.43.104
	1	10.50.255.251 (10.50.255.251)
	…
	11	bw-in-f103.1e100.net (74.125.43.103)


C Implementation:

    $ sudo bin/traceroute www.google.com
    Warning: www.google.com has multiple addresses; using 74.125.43.104
	1	10.50.255.251 (10.50.255.251)
	…
	11	bw-in-f103.1e100.net (74.125.43.104)
