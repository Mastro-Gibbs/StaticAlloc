
	███████╗████████╗ █████╗ ████████╗██╗ ██████╗ █████╗ ██╗     ██╗      ██████╗  ██████╗
	██╔════╝╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔════╝██╔══██╗██║     ██║     ██╔═══██╗██╔════╝
	███████╗   ██║   ███████║   ██║   ██║██║     ███████║██║     ██║     ██║   ██║██║     
	╚════██║   ██║   ██╔══██║   ██║   ██║██║     ██╔══██║██║     ██║     ██║   ██║██║     
	███████║   ██║   ██║  ██║   ██║   ██║╚██████╗██║  ██║███████╗███████╗╚██████╔╝╚██████╗
	╚══════╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝ ╚═════╝  ╚═════╝
                                                                                      
[![forthebadge](https://img.shields.io/badge/BASED-gray?style=for-the-badge&logo=c&labelColor=546CAF)]()  
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
  
  
**To run the tests in debug mode** run:
```bash
    $ make clean
```
  if you had already compiled everything, following by  
```bash
    $ make DEBUG=1
```
