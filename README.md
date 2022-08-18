[![Say Thanks!](https://img.shields.io/badge/Say%20Thanks-!-1EAEDB.svg)](https://docs.google.com/forms/d/e/1FAIpQLSfBEe5B_zo69OBk19l3hzvBmz3cOV6ol1ufjh0ER1q3-xd2Rg/viewform)

**ATTENTION!!!: fasmgP is currently still in its primordial experimental stages and neither backwards nor forwards compatibility are guaranteed so that the code base does not constrain itself during development and can achieve the best end result. Because of this, it is recommended that you add fasmgP as a git submodule to your project so it can be more easily managed without breaking anything as well as be both backdated and updated as needed.**

# fasmgP
Collection of fasmg procedures which can be quickly included with any fasmg project to gain out-of-the box procedures for simple memory management, string manipulations, and more, with minimal abstractions and as few system calls as possible in favor of doing as much as possible internally as efficiently and intuitively as possible.

**Design Goals and Philosophy**

The overarching philosphy is that homogenous and intuitive abstractions should be given to the programmer, but not the program. Interrupts (INT), calls (CALL), and multi-cycle instructions (LODSB, SCASB, CMPSB, etc.) are kept to all have a similar feel without needing to code-switch through different abstractions when going between them. For this reason, macros like the PROC macro are intentionally not used in favor of keeping the syntactic structure similar. Using fasmg's unique and powerful macros and conditional assembly, these abstractions then only take effect at assembly time and are not included in the final generated executable. This also means that only the things you actually reference in your code will be included in the final generated executable, as everything in fasmgP is conditional at assembly time, including import table entries. This means you're not just going to get lump sums of generic "objects" or import thousands of system API calls and end up with an executable containing mostly bloat that's never used and only a small fraction of code that's actually ever executed.

**Influences**

fasmgP does not strive to be a stand-alone language, but rather only hopes to be a useful companion package to fasmg. However, since there are no direct predecessor models which are appropriate to go off of to implement fasmgP's unique design goals and philosphy, classic languages such as C and C++ as well as more modern languages like Go have been huge influences. Go has certainly been the largest influence in that things like naming conventions, name spaces, and memory management make Go an extremely intuitive language to jump into and be productive within a very short amount of time. Go's biggest downfall, however, aside from its use of bundling large import packages together into large chunks as most other languages, is its use of Plan 9 assembly. Using Plan 9 assembly, which essentially serves as the core and backbone of the entire language, may have been a hasty decision by the Go creators, simply pulling what they already had on hand from their previous Plan 9 project and assuming an assembler of their own creation to be the best fit for the job. fasmgP directly addresses this problem by simply using fasmg as its core, rather than trying to reinvent the wheel or create something new which would more than likely be worse, as is Plan 9 assembly implemented by the Plan 9 Assembler itself, the GNU Assembler, or anything else intentionally choosing to limit itself to the constraints of Plan 9 assembly. And to be clear, the Plan 9 Assembler is just as unsuitable as the GNU Assembler and others, so this is not meant as an attack against the Plan 9 Assembler itself, merely a difference in alignment with design goals and philosophy. The alignment with and advantages of using fasmg are what enable the very implementation of fasmgP's design goals and philosophy.

**Memory Management**

Memory management structures intentionally never use the call stack, except when using 16-bit real mode where addressing space may be more constrained. Instead, fasmgP uses simple typed and untyped structures for better memory isolation and easier debugging with minimal overhead. The call stack is still used as usual for calling conventions and storing return addresses, etc., but it is just never referenced explicitly in fasmgP and instead only implemented implicitly by the fasmg assembler itself. The thought here is that rather than reserving a single large memory region for the call stack and just throwing everything into the same hole, like putting all your eggs in one basket, it's better to isolate data into multiple baskets by purpose to avoid a single point of failure, such as stack overflow or stack corruption, as well as allow for memory management procedures that are specialized for that particular type of data, as opposed to the call stack which just contains untyped variable-length data and doesn't have any special way to manage it efficiently other than pushing and popping and hoping that nothing corrupted it in between, which is a much higher risk when using the call stack for everything.

The simplest and most basic memory management structure is the auxiliary stack structure. This is a stack structure that can be used to store untyped variable-length data, such as private local variables, or pass data to other internal or external procedures, just like the call stack, but has its own dedicated region of memory which can be adjusted at assembly time separate from the call stack so they each can be sized independently as needed. Because the auxiliary stack is independent from the call stack, call stack frames don't affect the auxiliary stack and you don't need to spend extra time managing call stack frames for data you can throw onto the auxiliary stack without any conflict between the two. Although data from the auxiliary stack can be referenced in external calling conventions, the auxiliary stack should primarily be used as an isolated internal stack structure.

As stated earlier, fasmgP avoids directly referencing the call stack. When needing something like a PUSHA or POPA, rather than using the call stack, a special structure is used specifically to store register data. However, unlike PUSHA or POPA, only a subset of registers are saved. The stack base pointer and stack pointer are both omitted to maintain the call stack's true independence, and the accumulator is omitted since it's used for return values more often than not and can be easily preserved by itself for one-off procedures that absolutely must preserve its value. Also unlike POPA, the register store also gives you the option to pop the same set of register data multiple times without freeing it from the store, so you can easily restore non-preserved registers to their previous state quickly at any point in between system calls or to re-initialize loops, etc., as many times as needed and don't need to use private local variables for such purposes.

Two other special stores are an allocation store for storing allocated pointers and a handle store for storing handles. Both of these stores also have specialized procedures to track allocations or handles, validate whether they have been freed or not, prune them from their respective stores individually as needed if being freed manually, as well as iterate through the entire store and free everything at once, like for making a quick, easy, and clean exit. The only system calls needed are to receive an allocation or handle from the operating system and to request to the operating system that an allocation or handle be freed. All other tracking and validation is done internally without system calls. These procedures and structures allow you the freedom to finely-tune memory and free things manually and/or leave it to the stores to automatically clear out anything remaining before exiting or when otherwise manually triggered to flush their contents. However, it should always be kept in mind that there can never be any replacement for finely tuning your application yourself explicitly, even in fully memory-managed languages like Go.

There is also a common buffer structure, as well, with high and low guards, tracked length, and strict adhering to a maximum capacity which can be adjusted at assembly time to both prevent and detect things like overflow. And just as a quick note, the difference between length and capacity being that length refers to the number of bytes actually in use, or the number of bytes actually written into the buffer, while the capacity refers to the maximum number of bytes allocated to the buffer in total.

**Internal calling conventions**

Calling conventions are dictated by the needs of the partcular procedure and there is no one-size-fits-all standard calling convention. Some of the factors taken into consideration are the inputs of any particular multi-cycle instructions being used as well as the chainability of procedures most commonly chained together using similar inputs. That being said, some common themes are the source index register being used for the source pointer of string procedures (Keep in mind strings are any contiguous expanse of same-sized data in memory, not only text which are contiguos expanses of bytes and only one type of string.), the destination index register being used for the destination pointer of string procedures, the counter register being used to limit procedures to a specific region of memory, and the accumulator register being used for procedures focusing on individual elements of a string. If only some or none of those things are relevant to a particular procedure, then the counter is usually used as the first general argument, followed by the data register for the second general argument.

Due to the afforementioned memory management structures allowing more quick and easy access to additional memory locations and the scope of procedures being intentionally limited to highly focused tasks, there is not often any need to use any additional registers as arguments beyond those four mentioned. The base register is highly discouraged from being used in calling conventions, as it should be kept as a preserved register for other data that needs to persist throughout calls. And also keep in mind these loose rules only apply to calls made to internal procedures and have nothing to do with what or how registers are used internally by those particular called procedures during their lifetime.

# Projects using fasmgP

TinyWinDL:  
https://github.com/ScriptTiger/TinyWinDL

# More About ScriptTiger

For more ScriptTiger scripts and goodies, check out ScriptTiger's GitHub Pages website:  
https://scripttiger.github.io/

[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=MZ4FH4G5XHGZ4)

Donate Monero (XMR): 441LBeQpcSbC1kgangHYkW8Tzo8cunWvtVK4M6QYMcAjdkMmfwe8XzDJr1c4kbLLn3NuZKxzpLTVsgFd7Jh28qipR5rXAjx
