# bth : Bytes To Header in C/C++ Utility

#### Command Help
```
bth --help
Usage: bth -i <input> -o <output> -f <name> -c <count>

Options:
  --help, -h       Display this help and exit
  --version, -V    Display version information and exit
  -c <n>           Number of bytes to read
  -f <name>        Name of generated array
  -i <file>        Input file
  -o <file>        Output file

HomePage: <https://github.com/MrR736>
```

#### Command
```
bth -i Input.txt -o Output.h -f NAME -c 14
```

#### Input.txt
```
Hello, World!
```

#### Output.h
```
/* Generated with Bytes To Header 1.1-rc1 */

unsigned char NAME[] = {
  0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 
  0x21, 0x0a
};

/* Bytes : 14 */
```
