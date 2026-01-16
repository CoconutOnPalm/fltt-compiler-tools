# fltt-compiler-tools
Tools for the [FLTT Compiler](https://github.com/CoconutOnPalm/fltt-compiler) project

# installation:
requirements: `c++23-capable compiler`, `cmake`, `flex`, `bison`   
dependencies: [KEUL](https://github.com/CoconutOnPalm/KEUL.git) (shipped), [argparse](https://github.com/p-ranav/argparse), [ftxui](https://github.com/ArthurSonzogni/FTXUI)

```sh
# clone repo
git clone https://github.com/CoconutOnPalm/fltt-compiler-tools.git

# compile
cd ./fltt-compiler-tools
mkdir build
cmake -S . -B ./build
make -C ./build
```

# Debugger
The toolset contains a simple tui debugger that runs a modified virtual machine and shows instructions, registers and memory.  
The original virtual machine was made by [prof. Maciej Gębala](https://cs.pwr.edu.pl/gebala/) for the JFTT2025 compiler project.

## runing:
```sh
# cd fltt-compiler-tools
./build/debugger yout-assembly-program.mr
```

## instructions: 
 - `n` `next` `\return` - next line
 - `q` `quit` - exit the debugger


## important notes:
The input assembly file must contain the specified instructions line-by-line and must not contain any comments

example:  
**good:**
```
RST a
INC a
SHL a
INC a
SHL a
SHL a
WRITE
```
**bad**
```
RST a INC a # bad - two instructions in one line
SHL a
INC a
# bad - comment spanning the entire line
SHL a
SHL a
WRITE
```

the debugger will of course work with the "bad" programs, but the current line highliter will get shifted.