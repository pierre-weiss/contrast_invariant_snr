% function g=isotonic_chain(g0)
%
% This function solves:
% min_{g'>=0} ||g-g0||_2^2
%
% using an O(m) algorithm, where length(g0)=m
% Developers: Pierre Weiss & Yiqiu Dong, 2018
function g=isotonic_chain(g0)

J=[0];
jm=0;
m=length(g0);

cs=[0;cumsum(g0)];

for k=1:m-1
    if (cs(k+1)-cs(jm+1))/(k-jm) <= g0(k+1)
        J=[J,k];
        jm=k;
    else
        while jm>0 && (cs(k+2)-cs(jm+1))/(k+1-jm) <= (cs(J(end)+1)-cs(J(end-1)+1))/(J(end) - J(end-1))
            J=J(1:end-1);
            jm=J(end);
        end
    end
end

g=zeros(length(g0),1);
J=[0,J,m];

for j=1:length(J)-1
    g(J(j)+1:J(j+1))= (cs(J(j+1)+1) - cs(J(j)+1)) / (J(j+1) - J(j));
end
end