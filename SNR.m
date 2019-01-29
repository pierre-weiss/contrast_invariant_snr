function val=SNR(u,u0)

val=-10*log10(norm(u(:)-u0(:))^2/(norm(u0(:))^2));