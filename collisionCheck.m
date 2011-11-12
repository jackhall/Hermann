function [collisions] = collisionCheck(robot,q,polyhedra,varargin)
%All polyhedra must be convex!
bin = polyhedra.bodyIndex;
frin = polyhedra.frameIndex;

if ~isempty(varargin{1})
	
	collisions = varargin{2};
	t = varargin{1};
	a = length(collisions.c);
	proximityTolerance = .01;
	
	countCheck = a + 1;
	checkBodies = zeros(a+length(bin)^2,4);
	checkFrames = zeros(a+length(frin)^2,2);
	
	if a~=0
		checkBodies(1:a,:) = collisions.bodies; %automatically check active collisions
	end
else
	countCheck = 1;
	checkBodies = zeros(length(bin)^2,4);
	checkFrames = zeros(length(frin)^2,2);
end

%% forward kinematics
[bfpts bbpts] = ptfkin(robot,q,polyhedra);

%% bounding volumes
tempb = (1:8:(length(frin)*8))';

for i = 1:(robot.n-1)
    for j = (i+2):(robot.n+1)
        
        c = gjksubmex(bbpts(tempb(i):(tempb(i+1)-1),:),bbpts(tempb(j):(tempb(j+1)-1),:));
        if c
            ib = [bin(frin(i):(frin(i+1)-1)) (bin((frin(i):(frin(i+1)-1))+1)-1)];
            jb = [bin(frin(j):(frin(j+1)-1)) (bin((frin(j):(frin(j+1)-1))+1)-1)];
            n = size(ib,1)*size(jb,1);
            
            checkBodies(countCheck:(countCheck+n-1),:) = [repmat(ib,size(jb,1),1) kron(jb,ones(size(ib,1),1))];
			checkFrames(countCheck:(countCheck+n-1),:) = repmat([i j],n,1);
            countCheck = countCheck + n;
        end
        
    end
end

%prune checkBodies and check for redundant primitive tests
checkBodies = checkBodies(1:(countCheck-1),:);
checkFrames = checkFrames(1:(countCheck-1),:);
checks = UniqueRows([checkBodies checkFrames]);
checkBodies = checks(:,1:4);
checkFrames = checks(:,5:6);

%% primitive tests
[n m] = size(checkBodies);

if m ~= 0;
	for i = 1:n

		A = bfpts(checkBodies(i,1):checkBodies(i,2),:);
		B = bfpts(checkBodies(i,3):checkBodies(i,4),:);

		[c I Bary] = gjksubmex(A,B); 
		if c
			if isempty(varargin{1})
				collisions = true;
				return
			else
				if i>a %new collision
					if a~=0
						collisions.bodies = [collisions.bodies;checkBodies(i,:)];
						collisions.frames = [collisions.frames;(checkFrames(i,:)-1)];
						collisions.c = [collisions.c;true];
						collisions.proximity = [collisions.proximity;0];
						collisions.time = [collisions.time;t];
						a = length(collisions.c);
					else
						collisions.bodies = checkBodies(i,:);
						collisions.frames = checkFrames(i,:)-1;
						collisions.c = true;
						collisions.proximity = 0;
						collisions.time = t;
						a = 1;
					end
				else
					%if an old collision intersects again
					%the simulation is probably broken here
					collisions.c(i) = true;
					collisions.proximity(i) = 0;
				end
			end
		elseif ~isempty(varargin{1})
			if i<=a %old collision, no longer intersecting
				collisions.c(i) = false;

				closestA = sum(A(I(:,1),:).*repmat(Bary,1,3),1);
				closestB = sum(B(I(:,2),:).*repmat(Bary,1,3),1);
				collisions.proximity(i) = sqrt(sum((closestB-closestA).^2));
			end
		end
	end
end

if isempty(varargin{1})
	collisions = false;
else
	%delete obsolete collisions and collisions that occurred before the
	%current time step
	mask = collisions.proximity<proximityTolerance; %& collisions.time<=t;
	collisions.c = collisions.c(mask);
	collisions.bodies = collisions.bodies(mask,:);
	collisions.frames = collisions.frames(mask,:);
	collisions.proximity = collisions.proximity(mask);
	collisions.time = collisions.time(mask);
end

end