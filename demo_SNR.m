% This is a script to test the locally invariant to contrasts SNR.
% You can play with different images given in the toolbox.
%
% Developper: Pierre Weiss, 2019.

addpath(genpath('./'))

%% Loads two images
u0=double(imread('S2_1.jpg'));
u0=imresize(u0(:,:,2),[256,256]); % Make it gray scale
u0=floor(u0); % quantization to avoid stack overflow

u=double(imread('S2_5.jpg'));
u=imresize(u(:,:,2),[256,256]); % Make it gray scale
u=floor(u); % quantization to avoid stack overflow

%% Finds best contrast change
% Finds best global contrast change
disp('Global contrast change')
tic;[v_glo,~,SNR_glo] = SNR_global(u,u0);toc;

% Finds best local contrast change of type 1
disp('Local contrast change of type 1')
tic;[v_loc1,SNR_loc1] = SNR_local1(u,u0);toc;

% Finds best local contrast change of type 2
disp('Local contrast change of type 2 (may take time)')
tic;
eps=0;nit=5000;
[v_loc2,SNR_loc2] = SNR_local2(u,u0,eps,nit);
toc;

%% Displays the results with similar amplitude
M=max([max(u0(:)),max(u(:)),max(v_loc1(:)),max(v_loc2(:)),max(v_glo(:))]);
m=min([min(u0(:)),min(u(:)),min(v_loc1(:)),min(v_loc2(:)),min(v_glo(:))]);

figure(1);axis equal;imagesc(u0,[m M]);title('Reference image');axis off;colormap gray;axis equal;
figure(2);axis equal;imagesc(u,[m M]);title(sprintf('Image to compare SNR:%1.2f',SNR(u,u0)));axis off;colormap gray;axis equal;

figure(3);axis equal;imagesc(v_glo,[m M]);title(sprintf('Global contrast change, SNR:%1.2f',SNR_glo));axis off;colormap gray;axis equal;
figure(4);axis equal;imagesc(v_loc1,[m M]);title(sprintf('Local contrast change 1, SNR:%1.2f',SNR_loc1));axis off;colormap gray;axis equal;
figure(5);axis equal;imagesc(v_loc2,[m M]);title(sprintf('Local contrast change 2, SNR:%1.2f',SNR_loc2));axis off;colormap gray;axis equal;