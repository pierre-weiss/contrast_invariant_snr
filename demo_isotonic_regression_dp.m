%% The end of this script requires CVX to be installed http://cvxr.com/cvx/
rng(10);
L=4;
n=3;
tree=make_random_tree(L,n);
N=length(tree);
w=floor(10*rand(N,1))+1;
y=floor(10*rand(N,1))+1;

%% Transforms the tree in a form interpretable for the mex
T=zeros(N,1);
s=zeros(N,1);
for i=1:N
    T(i)=tree{i}.parent;
    s(i)=tree{i}.sign;
end

%% A dynamic programming approach
% x: minimizer of ||sqrt(w).*(x-y)||_2^2 s.t. s_i(x_i-x_j)>=0, (i,j) in E
tic;x=isotonic_regression_tree_mex(T,s,w,y);toc

%% We should make a check that we get the exact solution at this point and launch many runs
% We construct the adjacency matrix
A=zeros(N-1,N);
ind=0;
for i=1:N
    if tree{i}.parent~=0
        ind=ind+1;
        A(ind,i)=tree{i}.sign;
        A(ind,tree{i}.parent)=-tree{i}.sign;
    end
end

% Then we solve the problem: min ||f-y||_2 st Af>=0
cvx_begin quiet 
    cvx_precision best
    variable f_cvx(N)
    minimize norm(sqrt(w).*(f_cvx-y),2)
        subject to
            A*f_cvx>=0;
cvx_end


%% And show the relative error
disp('******************* RESULT ************')
disp(norm(x-f_cvx)/norm(f_cvx));

