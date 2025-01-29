pkg load statistics
% Generate random lognormally distributed data
rand("state", 0);% Reset the random number generator for reproducibility
%data = lognrnd(log(20000),1,[1,50]);
%data = lognrnd(log(20000),1,[1,1500]);
data = lognrnd(log(20000),1,[1,50000]);

% Create a histogram of the data
hist(data./sum(data), 50); % Adjust the number of bins as needed
%hist(data, 5000); % Adjust the number of bins as needed
%hist(data, 5000); % Adjust the number of bins as needed

% Add labels and title
%title('M=50');
%title('M=1500');
title('M=50000');

% Save the plot as an image file (e.g., PNG)
%print('lognormal_50.png', '-dpng');
%print('lognormal_1500.png', '-dpng');
print('lognormal_50000.png', '-dpng');


