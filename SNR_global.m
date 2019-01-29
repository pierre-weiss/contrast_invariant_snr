% function [gu, g, snr] = SNR_global(u,u0)
%
% This function finds the minimizer of:
% min_{g non decreasing} 1/2 || g(u) - u0 ||_2^2
%
% INPUT:
% - u0 : reference image.
% - u : image to map to u.
%
% OUTPUT:
% - snr : snr between gu and u0
% - gu : g(u)
% - g : monotone function
%
% Developer: Pierre Weiss, 2018
function [gu, g,snr] = SNR_global(u,u0)

n = numel(u0) ;

[levels, ~, IC] = unique(u(:)) ;
N = length(levels) ;
I = cell(N,1) ;

S = zeros(N,1) ;
g0 = zeros(N,1) ;

% Computing levels
for i = 1:n
    I{ IC(i) } = [I{ IC(i)}, i] ;
end
for k = 1:N
    S(k) = numel(I{k}) ;
    g0(k) = sum(u0(I{k})) / S(k) ;
end

% Optimization
g=isotonic_chain(g0);

% Setting the result
gu = zeros(size(u0)) ;
for k = 1:N
    gu( I{k} ) = g(k) ;
end

snr = -10*log10( norm(gu(:) - u0(:))^2 / norm(u0(:))^2) ;



