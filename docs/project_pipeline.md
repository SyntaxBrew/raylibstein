High Level Structure:

	CMake
	↓
	Build System
	↓
	Preprocessor
	↓
	Compiler
	↓
	Assembler
	↓
	Linker
	↓
	Executable

---

CMake
- Build system generator (meta-build system)
- Configures how project is built using the CMakeLists.txt file
- Tells the build system what source files to compile, what executable to make, external libraries, etc.
- Delegates tasks to the build system

↓

Build System (**Ninja**, GNU Make)
- Reads the configuration
- Orchestrates the preprocessor, compiler, assembler, and linker
- Handles file dependencies

↓

Preprocessor
- Transforms the source code before handing it off to the compiler
- Expands headers (literally copies the code content and pastes it at that specific line)

↓

Compiler
- Parses C++ syntax, and checks for any typos or errors
- Converts your C++ source code into lower-level assembly code

↓

Assembler
- Converts your lower-level assembly code into machine code
- Each object file has its own symbol table, which maps every function and global variable to a specific memory address (the address the CPU to jump to when the function is called)
- If the function isn't defined (only called) in that particular file it will have an unresolved/undefined address that the linker has to deal with

↓

Linker
- Reads every object file's symbol table
- Checks for any unresolved memory addresses, patches them up with the actual addresses from the file where the function was actually defined
- Combines everything into one binary/executable file

↓

Executable File