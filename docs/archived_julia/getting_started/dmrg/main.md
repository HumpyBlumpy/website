# Getting Started with DMRG

The [density matrix renormalization group (DMRG)](https://tensornetwork.org/mps/algorithms/dmrg/)
is an algorithm for computing eigenstates
of Hamiltonians (or extremal eigenvectors of large, Hermitian matrices). 
It computes these eigenstates in the 
[matrix product state (MPS)](https://tensornetwork.org/mps/) format.


Let's see how to set up and run a DMRG calculation using the ITensor library.
We will be interested in finding the ground state of the quantum Hamiltonian
@@H@@ given by:
$$
H = \sum_{j=1}^{N-1} \mathbf{S}_{j} \cdot \mathbf{S}_{j+1} = \sum_{j=1}^{N-1} S^z_{j} S^z_{j+1} + \frac{1}{2} S^+_{j} S^-_{j+1} + \frac{1}{2} S^-_{j} S^+_{j+1}
$$ 
This Hamiltonian is known as the one-dimensional Heisenberg model and we will
take the spins to be @@S=1@@ spins (spin-one spins). We will consider
the case of @@N=100@@ and plan to do five sweeps of DMRG (five passes over the system).

## ITensor DMRG Code

Let's look at an entire, working ITensor code that will do this calculation then
discuss the main steps. (If you need help running the code below, see the getting
started page on [[running ITensor codes|getting_started/running]].)

    include:docs/VERSION/getting_started/dmrg/dmrg.jl

<img class="icon" src="docs/VERSION/install.png"/>&nbsp;<a href="docs/VERSION/getting_started/dmrg/dmrg.jl">Download this example code</a>


## Steps of The Code

The first two lines

    N = 100
    sites = siteinds("S=1",N)

tells the function `siteinds` to make an array of ITensor [Index](https://itensor.github.io/ITensors.jl/stable/IndexType.html) objects which
have the properties of @@S=1@@ spins. This means their dimension will be 3 and 
they will carry the `"S=1"` tag, which will enable the next part of the code to know
how to make appropriate operators for them.

_Try printing out some of these indices to verify their properties_

    @show sites[1]
    @show sites[2]

The next part of the code builds the Hamiltonian:

    ampo = AutoMPO()
    for j=1:N-1
      ampo += "Sz",j,"Sz",j+1
      ampo += 1/2,"S+",j,"S-",j+1
      ampo += 1/2,"S-",j,"S+",j+1
    end
    H = MPO(ampo,sites)

An `AutoMPO` is an object which accumulates Hamiltonian terms such as `"Sz",1,"Sz",2`
so that they can be summed afterward into a matrix product operator (MPO) tensor network. 
The line of code `H = MPO(ampo,sites)` constructs the Hamiltonian in the MPO format, with
physical indices given by the array `sites`.

The line

    psi0 = randomMPS(sites,10)

constructs an MPS `psi0` which has the physical indices `sites` and a bond dimension of 10.
It is made by a random quantum circuit that is reshaped into an MPS, so that it will have as generic and unbiased properties as an MPS of that size can have.
This choice can help prevent the DMRG calculation from getting stuck in a local minimum.

The lines

    sweeps = Sweeps(5)
    maxdim!(sweeps, 10,20,100,100,200)
    cutoff!(sweeps, 1E-10)

construct a `Sweeps` objects which is initialized to define 5 sweeps of DMRG. The
call to `maxdim!` sets the maximum dimension allowed for each sweep and the call
to `cutoff!` sets the truncation error goal of each sweep (if fewer values are
specified than sweeps, the last value is used for all remaining sweeps).

Finally the call 

    energy, psi = dmrg(H,psi0,sweeps)

runs the DMRG algorithm included in ITensor, using `psi0` as an
initial guess for the ground state wavefunction. The optimized MPS `psi` and
its eigenvalue `energy` are returned.

After the `dmrg` function returns, you can take the returned MPS `psi` and do further calculations with it, such as [[measuring local operators|formulas/measure_mps]] or [[computing entanglement entropy|formulas/entanglement]].



