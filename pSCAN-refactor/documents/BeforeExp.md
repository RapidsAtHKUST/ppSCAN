# Optional: Before Experiments

clear page cache    

```zsh
sudo -i
echo 3 > /proc/sys/vm/drop_caches
```

make sure that you have already clear page cache before checking. check disk r/w.

```zsh
sudo time dd if=/dev/sda5  of=/dev/null bs=8k count=1400000
```

my desktop info

```zsh
1400000+0 records in
1400000+0 records out
11468800000 bytes (11 GB, 11 GiB) copied, 59.5983 s, 192 MB/s
0.24user 6.33system 0:59.62elapsed 11%CPU (0avgtext+0avgdata 1928maxresident)k
22399088inputs+0outputs (1major+78minor)pagefaults 0swaps
```