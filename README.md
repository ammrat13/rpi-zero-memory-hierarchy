# Memory Hierarchy Experimentation

This repository holds some bare-metal code to benchmark the memory hierarchy on
the Raspberry Pi Zero. The build produces a `.bin` file intended to be used with
the bootloader from [CS 140E][1] and [CS 240LX][2]. It outputs the data needed
to make a graph similar to Figure 2.33 in [Hennessy and Patterson][3].

## Output
* `arr_size`: The size of the test array in bytes
* `arr_stride`: How many bytes consecutive reads were spaced apart by
* `step_count`: How much data was moved in multiples of 256 MiB
* `elapsed_usec`: How long it took to transfer that data in us

[1]: https://github.com/dddrrreee/cs140e-24win
[2]: https://github.com/dddrrreee/cs240lx-24spr
[3]: https://shop.elsevier.com/books/computer-architecture/hennessy/978-0-12-811905-1
