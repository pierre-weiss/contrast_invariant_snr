% This is a script to show how the codes can be used for change detection.
% You can play with different images given in the toolbox.
%
% Developper: Pierre Weiss, 2019.

%% Loads two images
u0=double(imread('S2_1.jpg'));
u0=imresize(u0(:,:,2),[256,256]); % Make it gray scale
u0=floor(u0); % quantization to avoid stack overflow

u=double(imread('S1_1.jpg'));
u=imresize(u(:,:,2),[256,256]); % Make it gray scale
u=floor(u); % quantization to avoid stack overflow

%% We can also assess the changes between two different scenes
disp('Local contrast change of type 1')
tic;[v_loc1,SNR_loc1] = SNR_local1(u,u0);toc;
v_diff1=u0-v_loc1;

disp('Local contrast change of type 2 (may take time)')
eps=0;nit=5000;
tic;[v_loc2,SNR_loc2] = SNR_local2(u,u0,eps,nit);toc;
v_diff2=u0-v_loc2;

M=max([max(u0(:)),max(u(:)),max(v_diff1(:)),max(v_diff2(:))]);
m=min([min(u0(:)),min(u(:)),min(v_diff1(:)),min(v_diff2(:))]);

figure(1);axis equal;imagesc(u0,[m M]);title('Reference image');axis off;colormap gray;axis equal;
figure(2);axis equal;imagesc(u,[m M]);title('Image to compare');axis off;colormap gray;axis equal;
figure(3);axis equal;imagesc(v_diff1,[m M]);title(sprintf('Difference with local 1, SNR:%1.2f',SNR_loc1));axis off;colormap gray;axis equal;
figure(4);axis equal;imagesc(v_diff2,[m M]);title(sprintf('Difference with local 2, SNR:%1.2f',SNR_loc2));axis off;colormap gray;axis equal;

