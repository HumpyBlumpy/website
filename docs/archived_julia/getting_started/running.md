
# Running ITensor Codes

After [[installing the ITensor library|getting_started/installing]] the
next step is creating and running codes.
We will look at a minimal Julia code based on ITensor and various ways to run it.

## Basic Code Outline

The basic outline of a code which uses the ITensor library is as follows

    using ITensors

    let
      # ... your own code goes here ...
      # For example:
      i = Index(2,"i")
      j = Index(3,"j")
      T = randomITensor(i,j)
      @show T
    end

The reason we recommend the `let...end` block is that global scope 
 in Julia can have some surprising behaviors, at least through Julia version 1.4. 
Putting your code into a `let` block avoids these issues.

## Running a Script

Now say you put the above code into a file named `code.jl`. Then you can run
this code on the command line as follows

    $ julia code.jl

This script-like mode of running Julia is convenient for running longer jobs,
such as on a cluster.

## Running Interactively

However, sometimes you want to do rapid development when first writing and 
testing a code. For this kind of work, the long startup and compilation times
currently incurred by the 1.x versions of Julia can be a nuisance. Fortunately
there is a nice solution: repeatedly load your code into a running Julia session.


To set up this kind of session, take the following steps:

1. Enter the interactive mode of Julia, by inputting the command `julia` on the 
command line. You will now be in the Julia "REPL" (read-eval-print loop) with the
prompt `julia>` on the left of your screen.
2. To run a code such as the `code.jl` file discussed above, input the command

        julia> include("code.jl")

  Note that you must be in the same folder as `code.jl` for this to work; otherwise
input the entire path to the `code.jl` file. The code will run and you will see its output in the REPL.
3. Now say you want to modify and re-run the code. To do this, just edit the file in another terminal window or editor program, without closing your Julia session. Now run the command 

        julia> include("code.jl")

   again and your updated code will run, but this time skipping any of the precompilation overhead incurred on previous steps.

The above steps to running a code interactively has a big advantage that you only have to pay the startup time of compiling ITensor and other libraries you are using once. Further changes to your code only incur very small extra compilation times, facilitating rapid development.

## Other Ways of Running ITensor

The ITensors.jl library should be runnable through any type of setting that runs
Julia code, such as Jupyter notebooks. If you encounter a mode
of running ITensor that doesn't work, please contact us by emailing <i>support -at- itensor.org</i>.

## Why Are There Pauses?

You may often notice that your Julia code will run very quickly through one section, then take a long time to run a certain function. For example, while running an ITensor DMRG calculation, the first sweep is often reported as taking 10 or more seconds while the second and third sweeps take only a second. The reason for this is that Julia is a _just in time (JIT) compiled_ language. So instead of compiling at the beginning like a C++ code, the compilation happens at the very last minute when you call a function for the first time. As of Julia version 1.4, the JIT time for Julia can be quite long, but it is expected to get much better with future releases of Julia. Also we are working on various precompilation strategies with ITensors.jl to make it so that more code only has to be compiled once, until you upgrade ITensors.jl again. 

For strategies to avoid the JIT compilation overhead and reduced code pauses, see 
[[this page on compilation|getting_started/compilation]].

