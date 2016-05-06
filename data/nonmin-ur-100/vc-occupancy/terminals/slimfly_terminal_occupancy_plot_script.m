clear
%close all;
%% Input Log Files
TOTAL_TERMINALS = 150;
VC_SIZE = 1;
NUM_VC = 1;
N_COLLECT = 100;
Data = zeros(TOTAL_TERMINALS,N_COLLECT,NUM_VC);
for k=0:TOTAL_TERMINALS-1
    filename = sprintf('slimfly_terminal_occupancy_log.%d.txt',k);
    %filename = 'slimfly-results-log-batch-scaling.txt';
    %filename = 'slimfly-results-log-MMS43-Mastiff-Scaling.txt';
    slimfly_results_log = dlmread(filename,',',0,0);
    Data(k+1,:,:) = slimfly_results_log(:,2:end-1);
end
figure
for k=1:50
%5    figure
    hold on
    for j=1:NUM_VC
        plot(1:100,Data(k,:,j))
        xlabel('% TW End Time');
        ylabel('VC Occupancy');
    end
end
hold off

%% Plot 3D Representation of Sent Messages Data
figure
hold on
Y = 1:TOTAL_TERMINALS;
X = 0:N_COLLECT-1;
indx = 1;
Z = zeros(length(X),length(Y));
for j=1:1:TOTAL_TERMINALS
    for i=1:NUM_VC     %starting at i=1 gives all vc0, 2->vc1, 3->vc2, 4->vc3
        Z(:,indx) = Data(j,:,i);
        indx = indx+1;
    end
end
surf(X,Y,Z');
xlabel('% TW End Time');
ylabel('Terminal ID');
zlabel('Occupancy');
title('Terminal VC Occupancy vs Time')
colorbar
figure
ribbon(X,Z);
ylabel('% TW End Time');
xlabel('Terminal ID');
zlabel('Occupancy');
title('Terminal VC Occupancy vs Time')
figure
h = waterfall(X,Y,Z');
colormap(jet);
set( h, 'LineWidth', 4 );
hidden off;
xlabel('% TW End Time');
ylabel('Terminal ID');
zlabel('Occupancy');
title('Terminal VC Occupancy vs Time')
colorbar
hold off
