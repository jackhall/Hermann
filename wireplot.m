function [] = wireplot(v,e)
%v should be an mx3 list of verticies. e should be a list of indicies for
%the beginning of each body

[n m] = size(e);
color = ['b';'g';'r';'c';'m';'k'];

for i = 1:(n-1) %each body
        
    l = length(e(i):(e(i+1)-1));
        
    if l < 4
        
        cell = rem(i,6)+1; %plots lines between any two points on the same body
        
        for j = e(i):(e(i+1)-2) %each point
            for k = (j+1):(e(i+1)-1) %every other point
                    
                x = [v(j,1) v(k,1)];
                y = [v(j,2) v(k,2)];
                z = [v(j,3) v(k,3)];
                plot3(x,y,z,color(cell)) 
                hold on;
                    
            end
        end
        
    else
        
        V = v(e(i):(e(i+1)-1),:);
        TES = delaunayn(V,{'Qt','Qbb','Qc','Qz'}); %plots tetrahedron mesh from Delaunay tesselation
        C = ones(size(TES,1),1) * (rem(i,7) + 1); 
        tetramesh(TES,V,C)
        hold on;
            
    end
        
end

%axis([-2 2 -2 2 -.5 1.5])
axis([-1 1 -1 1 -1 1])
xlabel('x')
ylabel('y')
zlabel('z')
hold off

end