function [c I Bary] = gjksubmex(A,B)
%A is an nx3 matrix of points defining the polyhedron
%returns c = 1 if the origin is contained in A

Q = [(A(1,:) - B(1,:)) 1 1];
count = 1;

while count < 50

    [Cart Bary] = PtMinNorm(Q(:,1:3)); 
    
    %%%%%%%%%return TRUE if new closest point is the origin
    if sum(Cart ~= [0 0 0]) == 0       
        c = true;
        I = Q(:,[4 5]);
		Bary = Bary(Bary~=0);
        return
    end
    
    [Q V i j] = IndexandSupport(Q,A,B,Cart,Bary); %LogicIndex and RowIndexAssign are wrongly spitting out 0 indicies
	i = i+1;
	j = j+1;
    
    %%%%%%%%%%return FALSE if the new supporting vertex is no closer to the
    %%%%%%%%%%origin than the current closest point
    dist = -Cart*Cart';
    if (dist + .000001) >= (-Cart*V')
        c = false;
        I = Q(:,[4 5]);
		Bary = Bary(Bary~=0);
        return  
    end
    
    %%%%%%%%%%add the new supporting vertex to Q and continue
    V = [V round([i j])];
	if i==0 || j==0
		i
		j
		error('Improper simplex point index')
	end
    Q = [Q;V];
    count = count + 1;
    
end

error('GJK caught in while loop!')

end