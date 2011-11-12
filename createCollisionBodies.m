function [polyhedra] = createCollisionBodies(robot) 

n = robot.n;
polyhedra.frameIndex = zeros(n+2,1);
polyhedra.frameIndex(1) = 1;
polyhedra.bodyIndex(1) = 1;
polyhedra.points = [];
polyhedra.pointBody = [];
polyhedra.pointFrame = [];
polyhedra.bodyFrame = [];

for frame = 0:n
	m = input(strcat(['How many bodies are rigidly attached to frame ' num2str(frame) '? ']));
	
	for body = 1:m
		%ask for variable name
		var = input(strcat(['Which variable contains the points for body ' num2str(body) '? ']),'s');
		currentPoints = evalin('base', var);
		
		%store points, update indicies and support fields
		polyhedra.points = [polyhedra.points;currentPoints];
		polyhedra.pointBody = [polyhedra.pointBody;body*ones(1,size(currentPoints,1))'];
		polyhedra.pointFrame = [polyhedra.pointFrame;frame*ones(1,size(currentPoints,1))'];
		polyhedra.bodyIndex = [polyhedra.bodyIndex;(size(polyhedra.points,1)+1)];
		polyhedra.bodyFrame = [polyhedra.bodyFrame;frame];
		
		%{
		%specify collision checks
		if frame>0
			columnNames = {'Frame','Body','Run check?'};
			%prepare data columns
			t = uitable('Parent',figure(1),'Data',{framecol bodycol checks},'ColumnName',columnNames,'ColumnEditable',logical([0 0 1]));
			
			while true
				r = input('Specify which previous polyhedra to check against this one. Remember to click off the table. Continue? (y/n) ','s');
				if isequal(r,'y')
					checks = get(t,'Data');
					checks = cell2mat(checks{:,3});
					break
				elseif isequal(r,'n')
					polyhedra = 'Polyhedra entry incomplete.';
					return
				else
					disp('Just answer y or n. ')
				end
			end
		end
		%put new column into matrix
		%}
	end
	
	%update frameIndex
	polyhedra.frameIndex(frame+2,1) = length(polyhedra.bodyIndex);
	disp(' ')
end

polyhedra = boundBox(polyhedra);

end
