# gene
organizzazione files
```
.
├── generic
│   └── simple_test.cpp             :test c++ template and concepts
├── prototipo_v
│   └── tiler.cpp                   :prototipo Valter
├── py_generator
│   ├── extract_gen.py              :extract computational part
│   ├── gene_work_generator.py      :complete python generator
│   └── note.txt
├── README.md
├── set_ver                         :set versions
│   ├── README.md
│   ├── set_gene.cpp
│   └── unordered_set_gene.cpp
└── vector_ver                      :vector version
    ├── gene.cpp                    :copy, frequency
    └── gene.cpp.old
```


# simple performance tests
This benchmark are not well done, for now is only indicative (eg: programs still print in the benchmark)

parameters:
```
iterations = 2
lgn = {22, 33}
tile_size = 30
v_size = 40000
```

python extract generator
```
$ /usr/bin/time -v python extract_gen.py
	Command being timed: "python extract_gen.py"
	User time (seconds): 0.42
	System time (seconds): 0.01
	Percent of CPU this job got: 98%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.44
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 14980
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 0
	Minor (reclaiming a frame) page faults: 4917
	Voluntary context switches: 1
	Involuntary context switches: 1
	Swaps: 0
	File system inputs: 0
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
```

vector version
```
$ /usr/bin/time -v ./a.out
	Command being timed: "./a.out"
	User time (seconds): 0.03
	System time (seconds): 0.00
	Percent of CPU this job got: 92%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.05
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 3608
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 0
	Minor (reclaiming a frame) page faults: 179
	Voluntary context switches: 3
	Involuntary context switches: 26
	Swaps: 0
	File system inputs: 0
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
```

