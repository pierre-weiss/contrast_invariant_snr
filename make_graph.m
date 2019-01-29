%function [List,A,W,uCC]=make_graph(u)
%
% Given an image u, this function generates :
% - List: a list of level lines LL.
% - A: a sparse matrix with A(k,i)=1 and A(k,j)=-1 if the k-th relationship
% states that LL(i)>LL(j).
% - W: vector of weights indicating the number of pixels in each level line
% - uCC: an image containing the labelling of the level lines.
% 
% Developers: Gabriel Bathie and Pierre Weiss (07/2018).
function [List,A,W,uCC]=make_graph(u)

%% Step 1 : makes list of connected components
n=numel(u);

db_u=double(u);
uCC=idcc_mex(db_u);
List=regionprops(uCC,'PixelIdxList');

index=size(List,1);
W=zeros(index,1);
for i=1:index
    W(i)=numel(List(i).PixelIdxList);
end

nCC=length(List); % number of connected components


%% Step 2 : defines adjacency relationships
I=zeros(4*n,2);

ind=1;
[n1,n2]=size(u);
for i=1:n1
    for j=1:n2-1       
        %% Check horizontal relationships
        if (u(i,j+1)>u(i,j))
            I(ind,1)=uCC(i,j+1);
            I(ind,2)=uCC(i,j);
            ind=ind+1;
        elseif (u(i,j+1)<u(i,j))
            I(ind,1)=uCC(i,j);
            I(ind,2)=uCC(i,j+1);
            ind=ind+1;
        end
    end
end
for i=1:n1-1
    for j=1:n2
        %% Check vertical relationships
        if (u(i+1,j)>u(i,j))
            I(ind,1)=uCC(i+1,j);
            I(ind,2)=uCC(i,j);
            ind=ind+1;
        elseif (u(i,j)>u(i+1,j))
            I(ind,1)=uCC(i,j);
            I(ind,2)=uCC(i+1,j);
            ind=ind+1;
        end
    end
end

ind=ind-1;
I=unique(I(1:ind,:),'rows');
na=size(I,1); % number of adjacency relationships

%% Step 3 : generates matrix
%A=sparse(na,nCC);
i=[(1:na)';(1:na)'];
j=[I(:,1);I(:,2)];
s=[ones(na,1);-ones(na,1)];
A=sparse(i,j,s,na,nCC);

end

