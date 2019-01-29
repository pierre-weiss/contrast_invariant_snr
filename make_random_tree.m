% function tree=make_random_tree(L,n)
%
% Makes a random polytree of depth L with a maximum of n children per node.
%
% Developer: Pierre Weiss, 2018
function tree=make_random_tree(L,n)

tree{1}=[];
tree{1}.numel=1;
tree{1}.parent=0;
tree{1}.index=1;
tree{1}.sign=0;

index=1;
l=1;
tree=add_children(tree,index,l,L,n);

%% Just a display
nodes=zeros(length(tree),1);
for i=1:length(tree)
    nodes(i)=tree{i}.parent;
end
nodes=nodes';
figure(100);

treeplot(nodes);
[x,y] = treelayout(nodes);

for i=1:length(x)
    text(x(i)+0.02,y(i),num2str([i tree{i}.sign]))
end

function tree=add_children(tree,index,l,L,n)

if l<L
    m=floor(rand*n)+1;
    new_indices=tree{1}.numel+1:tree{1}.numel+m;
    tree{index}.children=new_indices;
    tree{1}.numel=tree{1}.numel+m;
    
    for i=new_indices
        tree{i}.parent=index;
        tree{i}.sign=1;%sign(rand-0.5);
        tree{i}.children=[];
        tree=add_children(tree,i,l+1,L,n);
    end
end

