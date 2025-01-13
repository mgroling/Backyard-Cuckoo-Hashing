# Backyard-Cuckoo-Hashing

This project was done in the course [DD2467](https://www.kth.se/student/kurser/kurs/DD2467?l=en) as part of my studies at [KTH](https://www.kth.se/).

Dynamic dictionaries (e.g. Hash Tables) are a data structure that can maintain a dynamic set under insertions, deletions, and lookups. Backyard Cuckoo Hashing [[1]](#1) is a two-level variant of cuckoo hashing that implements the dynamic dictionary data structure and promises great performance in terms of time and space complexity. In the project, I thoroughly went through the theory of the construction and implemented the data structure in C++. As the construction has many parameters that need to be chosen, I decided to conduct experiments on these can be chosen for good practical performance.

An overview of the construction can be seen below (taken from [[1]](#1)). In the first level, $m$ bins are used that can each store $d$ elements, which are mapped to a bin using a hash function. If an element can't be placed into its corresponding bin, then it is instead stored in the backyard. This is only a very high level description of the construction, but if it sounds interesting, I recommend you to check out my [report](https://github.com/mgroling/Backyard-Cuckoo-Hashing/blob/8eb5496f1e783532dc51e464057493219ca8eec6/report/report.pdf) or the original paper itself. I think that it was very interesting to look at how a state-of-the-art data structure works and which tricks it uses. I didn't really get to see these things in other courses.

![BackyardConstruction](/report/backyard-construction.PNG)

## Acknowledgements

Special thanks to my supervisor Ioana for answering all of my silly questions and guiding me throughout the project!

## References
<a id="1">[1]</a>
Y. Arbitman, M. Naor, and G. Segev, “Backyard cuckoo hashing: Constant worst-case operations with a succinct representation,” in 2010 IEEE 51st Annual symposium on foundations of computer science, pp. 787–796, IEEE, 2010.
