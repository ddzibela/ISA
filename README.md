# ISA project, winter semester 2021/22

Demo aplication for secure transfer of files, tunneled over ICMP echo packets.

## Usage

to compile simply run  ```make build``` 

to send a file (may require eleveated privileges)
```
secret -r file_path -s host_name
```

start in server mode ```secret -l```
