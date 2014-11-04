system('ls | grep report | wc -l > nIter');
niter = load("nIter")-1;
system('rm nIter');
figure;
counter = 1;
cmap = colormap(hsv);
for i=1:1:niter
	clf
	file = strcat('report', mat2str(counter) , '.txt')
	data = load(file);
	multiplier = mod(64, max(data(:,3)));
	for j=1:1:length(data(:,3))
		hold on;
		
		plot(data(j,1),data(j,2), "o" , "color", cmap(mod(data(j,3),63)+1,:),  "markersize"  , 10, "linewidth", 2);

	end
	counter = counter+1;
	pause(1);
	command = cstrcat('rm ', '' , file);
	system(command);
end
