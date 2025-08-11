#simple_shell

A custom command-line interpreter for Linux, built from scratch in C. This project simulates the core functionalities of a standard shell like Bash, demonstrating an understanding of low-level systems programming concepts.


core functionalities:



* **Command Execution**: Supports launching and executing external programs using `fork()` and `execvp()`.
* **Built-in Commands**: Includes custom implementations for `cd`, `exit`, and `help`.
* **I/O Redirection**: Handles input (`<`) and output (`>`) redirection to and from files.
* **Pipes**: Implements inter-process communication using `|` to chain commands together (e.g., `ls -l | grep .c`).
* **Background Processes**: Supports non-blocking execution of commands with the `&` operator.
* **Memory Management**: Implements manual memory allocation and deallocation to prevent leaks.

How to run the shell:

### How to Build and Run

1.  **Clone the repository:**
    `git clone https://github.com/your-username/simple_shell.git`

2.  **Navigate to the project directory:**
    `cd simple_shell`

3.  **Compile the source code:**
    `make`

4.  **Run the shell:**
    `./simple_shell`

[Simple-C-shell GitHub Repository](https://github.com/Blitzlasher1/Simple-C-shell)