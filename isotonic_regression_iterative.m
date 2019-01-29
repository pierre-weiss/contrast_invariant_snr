% function [alpha,lambda]=isotonic_regression_iterative(A,W,beta,eps,nit)
%
% This function solves min ||W^{1/2}(alpha-beta)||_2 s.t. A alpha>=eps using an accelerated gradient descent on the dual.
%
% INPUT:
% - beta: input sequence of size n.
% - eps: nonnegative real.
% - A: mxn matrix.
% - W: vector of size nx1.
% - nit: number of iterations.
%
% OUTPUT:
% - alpha: primal variable.
% - lambda: dual variable.
%
% Developers: Paul Escande and Pierre Weiss (02/2016)
function [alpha,lambda]=isotonic_regression_iterative(A,W,beta,eps,nit)

m=size(A,1);
p=size(A,2);
one=ones(m,1);

W_=1./W;

Abeta=A*beta;
AT=A';
lambda=zeros(m,1);
mu=lambda;
lambdap=lambda;
tol=1e-4;
L=normest_2(spdiags(W.^(-0.5),0,p,p)*AT,tol)^2*1.2;
t=1/L;
for k=1:nit
    gradmu=-A*(W_.*(AT*mu))+Abeta-eps*one;
    lambda=mu+t*gradmu;
    lambda(lambda>0)=0;
    
    mu=lambda+0.99*(lambda-lambdap);
    lambdap=lambda;   
end

alpha=beta-W_.*(AT*lambda);

end
