function [polyhedra] = boundBox(polyhedra)
%pts should be an mx3 matrix of verticies given with respect to the local
%reference frames (before running forward kinematics)

pts = polyhedra.points;
bin = polyhedra.bodyIndex;
frin = polyhedra.frameIndex;

bb = zeros(8*(length(frin)-1),3);

j = 1;
for i = 1:(length(frin)-1)
    
    crdmax = max(pts(bin(frin(i)):(bin(frin(i+1))-1),:));
    crdmin = min(pts(bin(frin(i)):(bin(frin(i+1))-1),:));
    
    if crdmax == crdmin
        pts(bin(frin(i)):(bin(frin(i+1))-1),:)
    end
    
    bb(j,:) = crdmax;
    bb(j+1,:) = crdmin;
    bb(j+2,:) = [crdmax(1:2) crdmin(3)];
    bb(j+3,:) = [crdmax(1) crdmin(2:3)];
    bb(j+4,:) = [crdmax(1) crdmin(2) crdmax(3)];
    bb(j+5,:) = [crdmin(1:2) crdmax(3)];
    bb(j+6,:) = [crdmin(1) crdmax(2:3)];
    bb(j+7,:) = [crdmin(1) crdmax(2) crdmin(3)];
    
    j = j + 8;
    
end

polyhedra.boundingBox = bb;
polyhedra.boundingBoxIndex = (1:8:((max(polyhedra.bodyFrame)+1)*8+1))';
polyhedra.boundingBoxFrame = kron(0:max(polyhedra.bodyFrame),ones(1,8))';

end
