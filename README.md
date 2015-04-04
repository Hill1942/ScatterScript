ScatterScript
=============

A Simple Script Language

### Grammar

- Like C language, you can use control structures: **if...else**, **while**, **continue**, **break**, but `no` **for** and **switch**

- In C language, you have a entry point: main function. It's the same in ScatterScript, but the entry point is **_main** function, which takes no parameter!

- Like JavScript, you can declare a variable by using keyword **var**, and define a fuction by using keyword **func**

- You must first declare a variable, and then assign value
     ```
     var aaa;
     aaa = 10;
     ```
- Data Type: Integer, Float-point number, String

- Example:
  ```
  func sum(end)
  {
	  var start;
	  start = 0;
	  while (end > 0)
	  {
		  start += end;
		  end -= 1;
	  }
	  return start;
  }

  func _main()
  {
	  var out;
	  out = sum(10);
	  return 0;
  }
  ```

### Built-In Function

- `sprint( out )`  
print **out** to console. **out** can be a integer, float-number or string.  
hello-world example:
    ```
    func _main() {
        var out;
        out = "hello, world!";
        ssprint(out);

        return 0;
    }
    ```

### Usage

- **<span style="font-size:16px">CompileToExe</span>**:ss -c `souce-file` `out-file(optional)`
  + example: ss -c test.ss  
This will create the final byte-code **test.sse**

- **<span style="font-size:16px">CompileToAsm</span>**: ss -s `souce-file` `out-file(optional)`
  + example: ss -s test.ss  
This will create the equivalent assemble file **test.ssam**

- **<span style="font-size:16px">Assemble</span>**: ss -a `asm-souce-file` `out-file(optional)`
  + example: ss -a test.ssam main  
This will convert assemble file to the final byte-code **main.sse**. 

- **<span style="font-size:16px">Run</span>**: ss `sse-file`
  + example:  ss main.sse   
This will run executable file **main.sse**
