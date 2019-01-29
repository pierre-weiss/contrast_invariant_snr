% function [v,SNR] = SNR_local2(u,u0,eps,nit)
%
% This function solves : 
% min ||h(u)-u0||_2^2, where h is a local contrast change. 
% This is solved by numerically involved procedures.
%
% INPUT : 
% - u0: reference image. 
% - u: image to be compared.
% - eps: to ensure strict monotonicity.
% - nit: number of iterations.
%
% OUTPUT: 
% - v=h(u): optimal contrast changed version of u.
% - SNR: SNR(v,u0)
%
% Developers: Gabriel Bathie, Paul Escande and Pierre Weiss (07/2018)

function [v,SNR] = SNR_local2(u,u0,eps,nit)

% Graph construction
[List,A,W,~]=make_graph(u);

% Here, v0 is the optimal contrast change without adjacency constraints
v0=zeros(size(u0));
beta=zeros(length(List),1);
for i=1:length(List)
    beta(i)=mean(u0(List(i).PixelIdxList));
    v0(List(i).PixelIdxList)=beta(i);
end

[alpha,~]=isotonic_regression_iterative(A,W,beta,eps,nit);

v=zeros(size(u));
for i=1:length(List)
    v(List(i).PixelIdxList)=alpha(i);
end

SNR=-10*log10( norm(v(:)-u0(:))^2 / (norm(u0(:))^2));