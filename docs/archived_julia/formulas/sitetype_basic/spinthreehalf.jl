using ITensors

function ITensors.siteinds(::TagType"S=3/2",
                           N::Int; kwargs...)
  conserve_qns = get(kwargs,:conserve_qns,false)
  if conserve_qns
    s = Index(QN("Sz",+3)=>1,
              QN("Sz",+1)=>1,
              QN("Sz",-1)=>1,
              QN("Sz",-3)=>1)
    return [sim(s;tags="S=3/2,Site,n=$n") for n=1:N]
  end
  return [Index(4,"S=3/2,Site,n=$n") for n=1:N]
end

function ITensors.op(::TagType"S=3/2",
                     s::Index,
                     opname::AbstractString; kwargs...)

  Op = ITensor(s',dag(s))

  if opname == "Sz"
    Op[s'(1), s(1)] = +3/2
    Op[s'(2), s(2)] = +1/2
    Op[s'(3), s(3)] = -1/2
    Op[s'(4), s(4)] = -3/2
  elseif opname == "S+"
    Op[s'(1),s(2)] = sqrt(3)
    Op[s'(2),s(3)] = 2
    Op[s'(3),s(4)] = sqrt(3)
  elseif opname == "S-"
    Op[s'(2), s(1)] = sqrt(3) 
    Op[s'(3), s(2)] = 2
    Op[s'(4), s(3)] = sqrt(3)
  else
    throw(ArgumentError("Operator name '$opname' not recognized for the \"S=3/2\" tag"))
  end
  return Op
end

