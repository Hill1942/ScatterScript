ScatterScript
=============

A Simple Script Language

### Grammar

- Like C language, you can use control structures: **if...else**, **while**, **continue**, **break**, but **<span style="color:red;">no</span>** **for** and **switch**

- Like C language, you have a entry point: main function. In ScatterScript, the entry point the **_main function**

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



### Usage

- **<span style="font-size:16px">CompileToExe</span>**: <span style="color: red; font-weight:bold">ss</span> <span style="color: blue; font-weight:bold">-c</span> `souce-file` `out-file(optional)`
  + example: <span style="color:#B51C1C">ss -c test.ss</span>  
This will create the final byte-code **test.sse**

- **<span style="font-size:16px">CompileToAsm</span>**: <span style="color: red; font-weight:bold">ss</span> <span style="color: blue; font-weight:bold">-s</span> `souce-file` `out-file(optional)`
  + example: <span style="color:#B51C1C">ss -s test.ss</span>  
This will create the equivalent assemble file **test.ssam**

- **<span style="font-size:16px">Assemble</span>**: <span style="color: red; font-weight:bold">ss</span> <span style="color: blue; font-weight:bold">-a</span> `asm-souce-file` `out-file(optional)`
  + example: <span style="color:#B51C1C">ss -a test.ssam main</span>  
This will convert assemble file to the final byte-code **main.sse**. 

- **<span style="font-size:16px">Run</span>**: <span style="color: red; font-weight:bold">ss</span> `sse-file`
  + example: <span style="color: #B51C1C">ss main.sse</span>   
This will run executable file **main.sse**
