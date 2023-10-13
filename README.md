
	███████╗████████╗ █████╗ ████████╗██╗ ██████╗ █████╗ ██╗     ██╗      ██████╗  ██████╗
	██╔════╝╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔════╝██╔══██╗██║     ██║     ██╔═══██╗██╔════╝
	███████╗   ██║   ███████║   ██║   ██║██║     ███████║██║     ██║     ██║   ██║██║     
	╚════██║   ██║   ██╔══██║   ██║   ██║██║     ██╔══██║██║     ██║     ██║   ██║██║     
	███████║   ██║   ██║  ██║   ██║   ██║╚██████╗██║  ██║███████╗███████╗╚██████╔╝╚██████╗
	╚══════╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝ ╚═════╝  ╚═════╝
                                                                                      

---

Static Alloc allows you to allocate memory statically, unlike Malloc()/Calloc()/Realloc().  
If you forget to call free(), that's no problem.  
The memory will be freed by the operating system.  

---

### Get this tool
```bash
    $ git clone https://github.com/Mastro-Gibbs/StaticAlloc.git
    $ cd StaticAlloc
```
---

### How to
Include the contents or the include/ and src/ folders in your project  
Include the unique "static_alloc.h" header in your point of interest.  
  
See the sources in the test/ folder for how to use the library.  
  
Optionally invoke where you cloned the repository from
```bash
    $ ./configure
    $ make
``` 
to compile the testing sources in the test/ directory.


