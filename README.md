# bth : Bytes To Header in C/C++ Utility

#### Command
```
bth -c 14 -i Input.txt -o Output.h -f NAME
```

#### Input.txt
```
Hello, World!
```

#### Output.h
```
/* Generated with Bytes To Header */

unsigned char NAME[] = {
  0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 
  0x21, 0x0a
};
```
