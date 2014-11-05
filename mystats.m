#!/urs/bin/octave
system('ls | grep /tmp/GibbsSampler/report | wc -l > nIter');
niter = load("nIter")-1;
system('rm nIter');
figure;
counter = 1;
cmap = colormap(hsv);
for i=0:1:niter
	clf
	file   = strcat('/tmp/GibbsSampler/report', mat2str(counter) , '.txt');
	filemu = strcat('/tmp/GibbsSampler/mu', mat2str(counter), '.txt');
	data   = load(file);
	data   = data(1:end-1,:);
	dataMu = load(filemu);
	multiplier = mod(64, max(data(:,3)));
	hold on;
	for j=1:1:length(data(:,3))
		plot(data(j,1),data(j,2), "o" , "color", cmap(mod(data(j,3),63)+1,:),  "markersize"  , 10, "linewidth", 2);
	end
	plot(dataMu(:,1), dataMu(:,2), "o","color", [0 0 0], "markersize",15,"linewidth", 2);
	
	counter = counter+1;
	pause(1);
	command  = cstrcat('rm ', file);
	command2 = cstrcat('rm ', filemu); 
	system(command);
	system(command2);
end

