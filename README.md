# Opto

This project doesn't really deserve a readme yet, but here's a brief rundown:

*What does Opto hope to accomplish?*

I want Opto to serve as an environment where I can tinker with any ideas that I feel demonstrate improvements from traditional software patterns.

***Too vague? Agreed. Here's a few examples:***

- A blend between a dynamic runtime and compiler. Full step by step transparency between high level description and target machine code, whether that be executable binaries, LLVM IR, SPIR-V IR for GPU, FPGA bitstreams, etc.
- Lack of data as text or "human readable" data in favor of efficient (dead simple) memory layout for meta-style programming. Think graphs and abstract syntax trees, nearly incompressible source code. Manipulate and view data structures directly when writing source and when stepping through debugging. You know this ***markdown*** I'm using to write this README? or LaTeX? Yeah none of that.
- Deep type analysis including guidable value constraints and guarantees, lifetime analysis, simple set theory applied to its full potential. If it might crash, either leave a formal note or prove that it can't. Domain knowledge should be expressed rigidly, not through comments and intuition.
- Graphical user interface toolbox to rival the current web without the bloat and historical baggage. True declarative interface design, with CAD-like layouting without sacrificing performance. Anti-CSS: layout, animation, all written simply and consistently. The terminal is out, long live concise GUI.
- Portability through smart, lightweight module management. No files, no directories, just data structures. Fine-grained source control with multiple developer awareness rather than blind branches and merges.
- Reactivity, treat the environment like a spreadsheet, nothing goes out of date without warning. Incremental compilation as default, functionally bound data structures, efficient and universal effect propagation.

***And for fun, a few things that Opto is not:***

- Another language that obscures itself in a unique syntax, or makes claims of using certain buzzword "paradigms". This is not a repackaging of the same bored concepts shrouded by "clever" conveniences like implicit data transformations, hidden control flow, etc.
- Unix. Unix provided "files" and the blurred lines around them, treating all programs as bland input-output machines, "processes" as these discrete rule following chunks for computation, a warped perspective on shared machines, and an inkling of an understanding of the complexities of concurrency. In fact, Opto is decently disconnected from the zeitgeist of an OS so I admit it's a bit strange to compare, but running Opto on bare metal, such as on Raspberry Pi, is on the agenda.
- An open source or production aimed project. I'm hesitant to call Opto an "academic" venture, but it may be the most accurate description for something as ambiguous and fluid as this.

Hand waving is tolerated, idealism is necessary, over-engineering is enlightening. Right now this is just a box drawing, exceedingly decoupled, crash prone C++ nightmare. But slowly, bit by bit, it's heading.. in some direction.
