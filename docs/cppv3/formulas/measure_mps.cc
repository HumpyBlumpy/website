#include "itensor/all.h"

using namespace itensor;

int 
main()
    {
    int N = 100;

    auto sites = SpinOne(N);

    auto ampo = AutoMPO(sites);
    for(int j = 1; j < N; ++j)
        {
        ampo += 0.5,"S+",j,"S-",j+1;
        ampo += 0.5,"S-",j,"S+",j+1;
        ampo +=     "Sz",j,"Sz",j+1;
        }
    auto H = toMPO(ampo);

    auto sweeps = Sweeps(5); //number of sweeps is 5
    sweeps.maxdim() = 10,20,100,100,200;
    sweeps.cutoff() = 1E-10;

    auto psi = MPS(sites);

    auto energy = dmrg(psi,H,sweeps,{"Quiet=",true});

    //
    // Measuring Sz
    //

    println("\nj Sz = ");
    for(int j=1; j <= N; ++j) 
        {
        //re-gauge psi to get ready to measure at position j
        psi.position(j);

        ITensor ket = psi(j);
        ITensor bra = dag(prime(ket,"Site"));

        ITensor Szjop = sites.op("Sz",j);

        //take an inner product 
        auto szj = elt(bra*Szjop*ket);
        printfln("%d %.12f",j,szj);
        }

    //
    // Measuring S.S
    //

    //Sum total S.S to check that it's 
    //equal to ground state energy
    Real totalSdS = 0.;

    println("\nj S.S = ");
    for(int b = 1; b < N; ++b) 
        { 
        psi.position(b);

        ITensor bondket = psi(b)*psi(b+1);
        ITensor bondbra = dag(prime(bondket,"Site")); 

        ITensor zzop = sites.op("Sz",b)*sites.op("Sz",b+1); 
        ITensor pmop = 0.5*sites.op("S+",b)*sites.op("S-",b+1); 
        ITensor mpop = 0.5*sites.op("S-",b)*sites.op("S+",b+1); 

        auto zz = elt(bondbra*zzop*bondket);
        auto pm = elt(bondbra*pmop*bondket);
        auto mp = elt(bondbra*mpop*bondket);

        printfln("%d %.12f",b,zz+pm+mp);
        totalSdS += zz+pm+mp;
        }

    printfln("\nSum of S.S = %.12f",totalSdS);
    printfln("Ground state energy from DMRG = %.12f",energy);

    return 0;
    }

