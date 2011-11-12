function [transformedPoints transformedBoundingBoxes frameOrigins] = ptfkin(robot,q,polyhedra)

dh = robot.dh;
dh(:,3) = q' + robot.offset;
rn = robot.n;

n = size(polyhedra.points,1);
transformedPoints = [polyhedra.points';ones(1,n)];
m = size(polyhedra.boundingBox,1);
transformedBoundingBoxes = [polyhedra.boundingBox';ones(1,m)];
frameOrigins = [zeros(3,rn);ones(1,rn)];

%{
figure(1)
wireplot([transformedPoints(1:3,:)';0 0 0;1 0 0;0 0 0;0 1 0;0 0 0;0 0 1],[polyhedra.bodyIndex;207;209;211])
frames = repmat([0 0 0 1;.2 0 0 1;0 0 0 1;0 .2 0 1;0 0 0 1;0 0 .2 1]',1,7);
figure(2)
wireplot(frames(1:3,:)',(1:2:43)')
%}
for i = rn:-1:1
    
	T =[cos(dh(i,3)) (-sin(dh(i,3)).*cos(dh(i,1))) (sin(dh(i,3)).*sin(dh(i,1))) (dh(i,2).*cos(dh(i,3)));
        sin(dh(i,3)) (cos(dh(i,3)).*cos(dh(i,1))) (-cos(dh(i,3)).*sin(dh(i,1))) (dh(i,2).*sin(dh(i,3)));
        0 sin(dh(i,1)) cos(dh(i,1)) dh(i,4);
        0 0 0 1];
	
	transformedPoints(:,polyhedra.pointFrame>=i) = T * transformedPoints(:,polyhedra.pointFrame>=i);
    transformedBoundingBoxes(:,polyhedra.boundingBoxFrame>=i) = T * transformedBoundingBoxes(:,polyhedra.boundingBoxFrame>=i);
    frameOrigins(:,i:rn) = T * frameOrigins(:,i:rn);
	%{
	frames(:,((6*i)+1):42) = T * frames(:,((6*i)+1):42);
	figure(1)
	wireplot([transformedPoints(1:3,:)';0 0 0;1 0 0;0 0 0;0 1 0;0 0 0;0 0 1],[polyhedra.bodyIndex;207;209;211])
	figure(2)
	wireplot(frames(1:3,:)',(1:2:43)')
    %}
end

transformedPoints = transformedPoints(1:3,:)';
transformedBoundingBoxes = transformedBoundingBoxes(1:3,:)';
frameOrigins = frameOrigins(1:3,:)';

end
