# Experiments

if possible, page cache are all dropped first 

```zsh
echo 3 > /proc/sys/vm/drop_caches
```

## Machine Configuration

Desktop

hardware | detail
--- | ---   
disk read | 190MB/s
memory | 32GB
cpu | 8 logical threads, hyper-threading, 3.4GHz

Cluster: luocpu8

hardware | detail
--- | ---
memory | 47GB
cpu | 16 logical threads, hyper-threading, 2.4GHz, 2 NUMA nodes

Cluster: gpu-23

hardware | detail
--- | ---
memory | 64GB
cpu | 40 logical threads, hyper-threading, 2.3GHz, 2 NUMA nodes 2 sockets
