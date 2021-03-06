### reduction factor

eps | instr# | mem load# | time
--- | --- | --- | ---
0.1 | 2.411 | 2.630 | 1.942
0.2 | 2.242 | 2.403 | 1.657
0.3 | 2.133 | 2.373 | 1.523
0.4 | 2.092 | 2.224 | 1.471
0.5 | 2.068 | 2.257 | 1.455
0.6 | 2.061 | 2.144 | 1.451
0.7 | 2.049 | 2.177 | 1.444
0.8 | 2.023 | 2.384 | 1.454
0.9 | 1.991 | 2.073 | 1.414

### reduction detail

eps | opt instr# | prev instr# | opt mem load# | prev mem load# | opt time | prev time
--- | --- | --- | --- | --- | --- | ---
0.1 | 4705.260b | 11342.740b | 600.268b | 1578.617b | 1180.937 | 2293.642
0.2 | 6934.980b | 15549.900b | 909.117b | 2184.486b | 2189.445 | 3627.329
0.3 | 5449.860b | 11625.280b | 697.361b | 1654.640b | 1964.866 | 2992.854
0.4 | 3566.260b | 7461.300b | 472.864b | 1051.642b | 1371.150 | 2017.014
0.5 | 2186.880b | 4522.340b | 288.759b | 651.650b | 869.544 | 1264.955
0.6 | 1272.960b | 2623.100b | 176.545b | 378.571b | 510.298 | 740.671
0.7 | 683.740b | 1400.800b | 90.163b | 196.286b | 271.233 | 391.714
0.8 | 306.340b | 619.820b | 36.261b | 86.453b | 120.681 | 175.427
0.9 | 72.080b | 143.480b | 8.680b | 17.991b | 29.671 | 41.959

### ppscan0(opt) bound ratio

eps | front-end | bad-spec | back-end-core | back-end-mem | retired-instr
--- | --- | --- | --- | --- | ---
0.1 | 0.225 | 0.316 | 0.041 | 0.021 | 0.398
0.2 | 0.231 | 0.401 | 0.036 | 0.014 | 0.318
0.3 | 0.238 | 0.437 | 0.035 | 0.013 | 0.277
0.4 | 0.242 | 0.449 | 0.036 | 0.013 | 0.261
0.5 | 0.246 | 0.454 | 0.037 | 0.013 | 0.250
0.6 | 0.250 | 0.451 | 0.037 | 0.014 | 0.248
0.7 | 0.257 | 0.441 | 0.037 | 0.015 | 0.251
0.8 | 0.255 | 0.424 | 0.052 | 0.022 | 0.248
0.9 | 0.252 | 0.418 | 0.073 | 0.039 | 0.217

### ppscan1(prev) bound ratio

eps | front-end | bad-spec | back-end-core | back-end-mem | retired-instr
--- | --- | --- | --- | --- | ---
0.1 | 0.163 | 0.206 | 0.066 | 0.024 | 0.541
0.2 | 0.193 | 0.301 | 0.040 | 0.012 | 0.453
0.3 | 0.193 | 0.339 | 0.037 | 0.010 | 0.421
0.4 | 0.192 | 0.356 | 0.037 | 0.010 | 0.405
0.5 | 0.192 | 0.368 | 0.037 | 0.010 | 0.393
0.6 | 0.193 | 0.370 | 0.039 | 0.011 | 0.387
0.7 | 0.193 | 0.369 | 0.036 | 0.011 | 0.391
0.8 | 0.197 | 0.371 | 0.033 | 0.011 | 0.388
0.9 | 0.194 | 0.364 | 0.056 | 0.022 | 0.364

