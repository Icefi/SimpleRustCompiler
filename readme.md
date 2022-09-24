# Rust compiler #

## Dependencies ##

    1) cmake 3.22
    2) g++ (GCC) 11.2.0
    3) java openjdk 17.0.3
    4) clang 13.0.1


## Installing dependencies ##

### Debian / Ubuntu / Linux mint ###

    sudo apt install cmake g++ openjdk-18-jre

### Arch / Garuda / Endeavour OS ###

    sudo pacman -S cmake g++ jre-openjdk

### Manjaro ###

    sudo pamac install cmake g++ jre-openjdk

### Fedora ###

    sudo dnf install cmake g++ java-1.8.0-openjdk

### Cent OS / RHEL ###

    sudo yum install cmake g++ java-1.8.0-openjdk-devel

## Building ##

    cd compilers-1
    mkdir build
    cd build
    cmake ..
    cmake --build .
    cd ..
    ln -s ./build/bin/app ./c-rust

## Testing ##

    cd build/bin/
    ./rust_lang_test

## Usage ##

    ./c-rust [options] <file.rs>

    examples:
        To get version:
            ./c-rust --version

        To just compile use:
            ./c-rust --file examples/test.rs

        To compile and print token list
        ./c-rust --dump-tokens --file examples/test.rs -o test

        To compile and get ast.xml
        ./c-rust --dump-ast --file examples/test.rs -o test

        To compile and get symbol table
        ./c-rust --dump-table --file examples/test.rs -o test

        To compile and get generated assembler code
        ./c-rust --dump-asm --file examples/test.rs -o test

        To compile and get generated LLVM IR code
        ./c-rust --emit-llvm --file examples/test.rs -o test


    options:
        -V / --version:     print compiler version
        -o / --output:      specify output file
        -t / --dump-tokens: print token list
        -a / --dump-ast:    write in ast.xml abstract syntax tree
        -s / --dump-table:  write in table.txt symbol table
        -S / --dump-asm:    output assembler code
        -e / --emit-llvm:   output LLVM IR code

## Let's code ##

### Function definition ###

    To define a function you need to write something like this:

    fn func1() {
        return;
    }

    In this case you define a function that does not have arguments and returns nothing.

    If you wish to function returns some value you need to specify returns value:

    fn func2() -> i32 {
        0 // <- you can return value without keyword 'return' and ';'
    }

    If in the function body does not have a return statement, then it automatically returns 0 or nothing (if return type is not specified).

    To specify argument list you need to enumerate them like this:

    fn func3(x:i32, y:i32) -> i32 { x + y }

### Variable definition ###

    To define a variable you need to write it's name after keyword 'let' and if you wish to specify it's type. For example:

    let number: i32 = 0;

### Input / Output ###

    If you wish to input value from keyboard you can use function 'scan':

    let a = 0;
    scan("%d", a); // input text from keyboard, for example 10

    You can use function 'print' to print text, numbers or strings:

    print("Hello %d\n", a); // output: Hello 10

### Arithmetic operations ###

    Nowadays compiler is able to compute following operations: + - * / % & | (&& and || with limitations) ,
    but you can use unlimited number of operations: a = 12 / (123 + 56 * a + 90); .

### Branching operator ###

    For a while you can use IF or IF-ELSE constructs.
    For example:

    let a = 0;
    scan("%d", a);

    if a == 0 && a < 10 {
        print("IF SCOPE\n");
    } else { // you can delete this part
        print("ELSE SCOPE\n");
    }

    Now you can use a while loop without operator 'break':

    while a < 10 {
        print("A is %d", a);
        a = a + 1;
    }

### ARRAYS ###

    You can define and initialise array like that:

    let arr = [1, 2, 3];

    You also can specify array type:

    let arr: [i32; 3] = [1, 2, 3];

    You can get access to an element with operator '[ ]':

    arr[2] = 4;
    print("%d", arr[2]); // output: 4
