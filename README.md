ScatterScript
=============

A Simple Script Language

### Build

- For Windows users: just open `ScatterScript.sln` in Visual Studio 2012 or higher 
version, and select **Build Solution** in menu **BUILD**  
And the development environment is Windows 7 Professional
- For Linux users: run the following 4 command in terminal

  ```
  mkdir build
  cd build
  cmake ..
  make
  ```

- For Mac OS X users: Open in Xcode 6.0 or higher, and build!

### Grammar

- Like C language, you can use control structures: **if...else**, **while**, **continue**, **break**, but `no` **for** and **switch**

- In C language, you have a entry point: main function. It's the same in ScatterScript, but the entry point is **_main** function, which takes no parameter!

- Like JavScript, you can declare a variable by using keyword **var**, and define a fuction by using keyword **func**

- Data Type: Integer, Float-point number, String and Array  
Note: Array can not be initialized when you declare it. You can only initialize an array by doing so:
  ```
  var a[3];
  a[0] = 0;
  a[1] = "ddd";
  a[2] = 34;  
  // or using a while loop
  ```

- Example:
  ```
  func sum(end)
  {
	  var start = 0;
	  while (end > 0)
	  {
		  start += end;
		  end -= 1;
	  }
	  return start;
  }

  func _main()
  {
	  var out = sum(10);
	  return 0;
  }
  ```

### Built-In Function

- `ssprint( out )`  
print **out** to console. **out** can be a integer, float-number or string.  
hello-world example:
    ```
    func _main() {
        var out = "hello, world!";
        ssprint(out);

        return 0;
    }
    ```
- `ssfopen( filename, mode)`   
Like fopen in C language, **filename** is the file to open, mode is the open mode, which can be: "r", "r+", "w", "w+", "a", "a+", etc
- `ssfgetc( file )`  
read a character from the **file**  
- `ssfgets( file, count)`  
read a line from **file**, or read **count-1** characters  
- `ssfclose( file)`  
close file
- `ssfwrite( file, out)`  
write **out** to **file**  
 example:
    ```
    func _main() {
        var file = ssfopen("data", "r");
        var c = ssfgetc(file);
        while (c != false) {
            ssprint(c);
            c = ssfgetc(file);
        }
        return 0;
    }
    ```
    
    This example shows how to read a file and print out all its content to console

### Usage

- **<span style="font-size:16px">CompileToExe</span>**:ss -c(-d) `souce-file` `out-file(optional)`
  + example: ss -c test.ss  
This will create the final byte-code **test.sse**  
Option `-d` means debug mode: generate a **test.sse.info**, which contains comment info for sse file

- **<span style="font-size:16px">CompileToAsm</span>**: ss -s `souce-file` `out-file(optional)`
  + example: ss -s test.ss  
This will create the equivalent assemble file **test.ssam**

- **<span style="font-size:16px">Assemble</span>**: ss -a `asm-souce-file` `out-file(optional)`
  + example: ss -a test.ssam main  
This will convert assemble file to the final byte-code **main.sse**. 


- **<span style="font-size:16px">Run</span>**: ss `sse-file`
  + example:  ss main.sse   
This will run executable file **main.sse**

### Finally:

ScatterScript is just a **toy language**, when I try to get the prime number from 1-250000 by using the following code:
```
func _main() {
	var i = 2;
	var a = 2;
	var top = 250000;
	var cop = 500;
	while (i <= top) {
		a = 2;
		while (a <= cop) {
			if ((i % a == 0) && (i != a)) {
				a+=1;
				break;
			} else {
				if (a == cop) {
					ssprint(i);
					ssprint("\n");
				}
			}
			a+=1;
		}
		i+=1;
	}
	return 0;
}
```

ScatterScript takes 139.70s, however, CPython takes 7.17s and Nodejs only takes 1.36s

