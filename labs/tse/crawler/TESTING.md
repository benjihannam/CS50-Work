Testing file for crawler.c

I tested my program on both http://old-www.cs.dartmouth.edu/~cs50/index.html and http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html. I ran the tester to depths 0,1 and 2. Running at depth two took a very long time so I decided it would be best not to run at depth 3 as I was sufficiently happy with what I saw at the other depths. I then went in and checked select files (1 for the seed URL, around 7/8/9 for the depth 1 URLs and then the highest numbered file for the other max depth URLS.) 

PROBLEM:
Because there are invalid URLs in the first seed URL I had memory leaks which I did my best to fix. I could free everything except the 'url' variable which is malloc'ed in the inner while loop. This is still placed into the hashtable and so I thought that when hashtable_delete was called that it would be freed like all the other urls. However this was not the case and despite hours of attempting to find out how to free the memory I could not figure it out. I recognise that this is a memory leak but it was one I was not able to fix within the time I had. 

NOTE:
There were also two other memory leaks but these were cause by the web.c file that we were provided with resulting in 64 bytes still being reachable.