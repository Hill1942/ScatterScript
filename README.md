ScatterScript
=============

A Simple Script Language

### Usage

- **Compile**: Not implement
  + example: 
- **Assemble**: ss -a `asm-souce-file` `out-file(unnessesary)`
  + example: ss -a test.ssam main  
This will create two file: `main.sse` and `main.sse.info`. `main.sse` is the executable file; `main.sse.info` is the exe file with comments for each instruction. 
- **Run**: ss `sse-file`
  + example: ss main.sse  
this will run executable file main.sse
