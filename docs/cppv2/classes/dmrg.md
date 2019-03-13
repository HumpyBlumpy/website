# DMRG Interface

The density matrix renormalization group (DMRG) is a very powerful algorithm
for optimizing matrix product states (MPS) for ground states and low-lying 
excited states of one-dimensional (and quasi-one-dimensional) Hamiltonians.

The ITensor DMRG code works with Hamiltonians in matrix product
operator (MPO) form so that a single DMRG code can work for a very wide 
range of different Hamiltonians and local Hilbert spaces.

ITensor also offers DMRG routines for more general optimization tasks, including
- Hamiltonians which are an (implicit) sum of multiple MPOs
- minimizing the energy of an MPS, while constraining it to remain orthogonal
  to a collection of other MPS (for targeting excited states)
- "restricted" sweeping over a sub-region of a system (such as a unit cell)
  within an environment defined by a "frozen" MPS basis for the rest of the system


## Synopsis

    int N = 100;
    auto sites = SpinOne(N);

    auto psi = MPS(sites);

    auto ampo = AutoMPO(sites);
    for(int j = 1; j < N; ++j)
        {
        ampo += "Sz",j,"Sz",j+1;
        ampo += 0.5,"S+",j,"S-",j+1;
        ampo += 0.5,"S-",j,"S+",j+1;
        }
    auto H = MPO(ampo);

    auto sweeps = Sweeps(5);
    sweeps.maxm() = 10,40,100,200,200;
    sweeps.cutoff() = 1E-8;

    auto energy = dmrg(psi,H,sweeps,{"Quiet",true});
    //                  ^ psi passed by reference,
    //                    can measure properties afterwards

    printfln("Ground state energy = %.20f",energy);


## Basic DMRG interface

* ```
  dmrg(MPS & psi,
       MPO H,
       Sweeps sweeps,
       Args args = Args::global()) -> Real
  ```

  ```
  dmrg(IQMPS & psi,
       IQMPO H,
       Sweeps sweeps,
       Args args = Args::global()) -> Real
  ```

  Run a DMRG calculation to find the ground state of the Hamiltonian (MPO or IQMPO) H 
  using the initial state given by the MPS or IQMPS psi. The variable psi is
  overwritten to hold the final wavefunction upon return.

  The number of sweeps and DMRG accuracy parameters are controlled by a [[Sweeps|classes/sweeps]]
  object.

  After the DMRG calculation completes, it returns the final ground state energy.

  The `dmrg` function also accepts the following optional named arguments:

  - "Quiet" &mdash; suppress most output except a short summary of the result of each DMRG sweep
  - "Silent" &mdash; suppress *all* output
  - "WriteM" &mdash; if WriteM is defined, upon beginning a sweep number n such that sweeps.maxm(n) value 
    exceeds WriteM, the MPS and MPO will be set to "write-to-disk" mode such that all but a few "core"
    tensors will remain on the hard drive to save on RAM
  - "DebugLevel" &mdash; non-negative integer telling the internal Davidson eigensolver how
    much extra information to print out


## Generalized DMRG interfaces

* ```
  dmrg(MPS & psi,
       std::vector<MPO> H,
       Sweeps sweeps,
       Args args = Args::global()) -> Real
  ```

  ```
  dmrg(IQMPS & psi,
       std::vector<IQMPO> H,
       Sweeps sweeps,
       Args args = Args::global()) -> Real
  ```

  Run a DMRG calculation using a collection of MPOs provided in a std::vector object (0-indexed).

  The MPOs will be treated as if they are summed. That is, the Hamiltonian is defined as the sum of 
  the MPOs provided. However, no actual sum of the MPOs will be performed, such that the cost of the algorithm
  is the sum of the costs of using each MPO individually.


* ```
  dmrg(MPS & psi,
       MPO H,
       std::vector<MPS> wfs,
       Sweeps sweeps,
       Args args = Args::global()) -> Real
  ```

  ```
  dmrg(IQMPS & psi,
       IQMPO H,
       std::vector<IQMPS> wfs,
       Sweeps sweeps,
       Args args = Args::global()) -> Real
  ```

  Run a DMRG calculation to find the lowest energy eigenstate of the Hamiltonian H,
  but with the constraint that psi should be orthogonal to all of the MPS provided
  in the (0-indexed) std::vector wfs.

  This code works by putting in an energy penalty for psi to have any overlap with 
  the MPS in wfs. This penalty is the product of the overlap of psi with each
  state times a "Weight", whose default value is 1.0 but which can be 
  adjusted by providing the named argument "Weight" as part of the args (last
  argument of type Args).

  To see a detailed example of using this excited-state DMRG code, see
  the code formula on [[computing excited states|formulas/excited_dmrg]].


* ```
  dmrg(MPS & psi,
       MPO H,
       ITensor LH,
       ITensor RH
       Sweeps sweeps,
       Args args = Args::global()) -> Real
  ```

  ```
  dmrg(IQMPS & psi,
       IQMPO H,
       IQTensor LH,
       IQTensor RH
       Sweeps sweeps,
       Args args = Args::global()) -> Real
  ```

  Run a DMRG calculation to find the ground state of the Hamiltonian H using the initial
  state psi, using a "boundary environment" defined by the tensors LH and RH.

  The boundary environments LH and RH (e.g. representing the projection of a Hamiltonian for a larger
  system into a fixed "MPS basis") should be tensors with three indices. One index connects
  to the bond index of the Hamiltonian MPO H at the left and right edge. The other two indices
  are i and dag(i') where i is the virtual (bond) index of the MPS psi at the left or right edge
  (and dag(i') is i with prime level 1 and reversed arrow in the case of an IQIndex).



