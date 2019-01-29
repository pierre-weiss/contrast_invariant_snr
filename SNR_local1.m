% function [v,SNR] = SNR_local1(u,u0)
%
% This function solves : 
% min ||h(u)-u0||_2^2, where h is a local contrast change defined through the FLST. 
%
% INPUT : 
% - u0: reference image. 
% - u: image to be compared.
%
% OUTPUT: 
% - v=h(u): optimal contrast changed version of u.
% - SNR: SNR(v,u0)
%
% Developers: Pierre Weiss (08/2018)

function [v,SNR] = SNR_local1(u1,u0)

v=project_llt_mex_double(u1,u0);
SNR=-10*log10( norm(v(:)-u0(:))^2 / (norm(u0(:))^2));

