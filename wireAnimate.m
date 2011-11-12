function [] = wireAnimate(robot,t,q,polyhedra,mode)

n = size(q,1);
m = size(polyhedra.points,1);
bfpts = zeros(m,3,n);

disp('Running forward kinematics....')

if isequal(mode,'even')
	timeStep = t - circshift(t,1);
	timeStep(1) = 0;
	
	j = 0;
	step = max(timeStep);
	a = 0;
	for i=1:n
		a = a + timeStep(i);
		if a>step || i==n
			j = j+1;
			bfpts(:,:,j) = ptfkin(robot,q(i,:),polyhedra);
			time(j) = timeStep(i);
			a = 0;
		end
	end
	n = j;
	
elseif isequal(mode,'discrete')
	timeStep = t - circshift(t,1);
	timeStep(1) = 0;
	
	j = 0;
	step = .01;
	a = 0;
	for i=1:n
		a = a + timeStep(i);
		if a>step || i==n
			j = j+1;
			bfpts(:,:,j) = ptfkin(robot,q(i,:),polyhedra);
			time(j) = timeStep(i);
			a = 0;
		end
	end
	n = j;
	
elseif isequal(mode,'normal')
	time = t - circshift(t,1);
	time(1) = 0;
	
	for i=1:n
		bfpts(:,:,i) = ptfkin(robot,q(i,:),polyhedra);
	end
	
else
	error('The three valid modes are "even", "normal", and "discrete"');
	
end

time(time<.01) = .01;

disp('Rendering....')
for i=1:n
	wireplot(bfpts(:,:,i),polyhedra.bodyIndex)
	view([-.1 -1 .25])
	pause(time(i))
end

end