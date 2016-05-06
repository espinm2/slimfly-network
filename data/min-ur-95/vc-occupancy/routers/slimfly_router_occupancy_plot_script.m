%close all;
%% Input Log Files
%TOTAL_ROUTERS = 50;
TOTAL_ROUTERS = 338;
VC_SIZE = 4;
%RADIX = 10;
RADIX = 28;
N_COLLECT = 100;
Data = zeros(TOTAL_ROUTERS,N_COLLECT,RADIX*VC_SIZE);
for k=0:TOTAL_ROUTERS-1
    filename = sprintf('slimfly_router_occupancy_log.%d.txt',k);
    %filename = 'slimfly-results-log-batch-scaling.txt';
    %filename = 'slimfly-results-log-MMS43-Mastiff-Scaling.txt';
    slimfly_results_log = dlmread(filename,',',0,0);
    Data(k+1,:,:) = slimfly_results_log(:,2:end-1);
end


% There should be one k "for loop" for each of the virtual channels
% for k=1:TOTAL_ROUTERS
% %5    figure
%     subplot(2,2,1)
%     hold on
%     for j=1:VC_SIZE:RADIX*VC_SIZE
%         plot(1:100,Data(k,:,j))
%         xlabel('% TW End Time');
%         ylabel('VC Occupancy');
%         title('Virtual Channel 0');
%     end
% end
% hold off
% for k=1:TOTAL_ROUTERS
% %5    figure
%     subplot(2,2,2)
%     hold on
%     for j=2:VC_SIZE:RADIX*VC_SIZE
%         plot(1:100,Data(k,:,j))
%         xlabel('% TW End Time');
%         ylabel('VC Occupancy');
%         title('Virtual Channel 1');
%     end
% end
% hold off
% for k=1:TOTAL_ROUTERS
% %5    figure
%     subplot(2,2,3)
%     hold on
%     for j=3:VC_SIZE:RADIX*VC_SIZE
%         plot(1:100,Data(k,:,j))
%         xlabel('% TW End Time');
%         ylabel('VC Occupancy');
%         title('Virtual Channel 2');
%     end
% end
% hold off
% for k=1:TOTAL_ROUTERS
% %5    figure
%     subplot(2,2,4)
%     hold on
%     for j=4:VC_SIZE:RADIX*VC_SIZE
%         plot(1:100,Data(k,:,j))
%         xlabel('% TW End Time');
%         ylabel('VC Occupancy');
%         title('Virtual Channel 3');
%     end
% end
% hold off

%% Plot 3D Representation of Suplot occupancy per vc
%figure
hold on
RADIX = 19;
%TOTAL_ROUTERS=5;
Y = 1:TOTAL_ROUTERS*(RADIX);
X = 0:N_COLLECT-1;
indx = 1;
Z = zeros(length(X),length(Y));
for j=1:1:TOTAL_ROUTERS
    for i=1:VC_SIZE:RADIX*VC_SIZE     %starting at i=1 gives all vc0, 2->vc1, 3->vc2, 4->vc3
        Z(:,indx) = Data(j,:,i);
        indx = indx+1;
    end
end
%subplot(3,2,3)
h = waterfall(X,Y,Z');
colormap(jet);
set( h, 'LineWidth', 4 );
hidden off;
xlabel('% Time');
ylabel('Router ID');
zlabel('Occupancy');
title('Router VC0 Occupancy 95% Load')
colorbar;
caxis([0, 100])
axis([0 100 0 TOTAL_ROUTERS*(RADIX) 0 100])
fontSize        = 14;
set(gca, 'FontSize', fontSize)

indx = 1;
Z = zeros(length(X),length(Y));
for j=1:1:TOTAL_ROUTERS
    for i=2:VC_SIZE:RADIX*VC_SIZE     %starting at i=1 gives all vc0, 2->vc1, 3->vc2, 4->vc3
        Z(:,indx) = Data(j,:,i);
        indx = indx+1;
    end
end
%subplot(3,2,4)
figure
h = waterfall(X,Y,Z');
colormap(jet);
set( h, 'LineWidth', 4 );
hidden off;
xlabel('% Time');
ylabel('Router ID');
zlabel('Occupancy');
title('Router VC1 Occupancy 95% Load')
colorbar;
caxis([0, 100])
axis([0 100 0 TOTAL_ROUTERS*(RADIX) 0 100])
fontSize        = 14;
set(gca, 'FontSize', fontSize)

% %% Plot 3D Representation of Sent Messages Data
% figure
% hold on
% TOTAL_ROUTERS=5;
% Y = 1:TOTAL_ROUTERS*(RADIX)*VC_SIZE;
% X = 0:N_COLLECT-1;
% indx = 1;
% Z = zeros(length(X),length(Y));
% for j=1:1:TOTAL_ROUTERS
%     for i=1:1:RADIX*VC_SIZE     %starting at i=1 gives all vc0, 2->vc1, 3->vc2, 4->vc3
%         Z(:,indx) = Data(j,:,i);
%         indx = indx+1;
%     end
% end
% surf(X,Y,Z');
% xlabel('% TW End Time');
% ylabel('Router ID');
% zlabel('Occupancy');
% title('Router VC Occupancy vs Time')
% colorbar;
% figure
% ribbon(X,Z);
% ylabel('% TW End Time');
% xlabel('Router ID');
% zlabel('Occupancy');
% title('Router VC Occupancy vs Time')
% figure
% h = waterfall(X,Y,Z');
% colormap(jet);
% set( h, 'LineWidth', 4 );
% hidden off;
% xlabel('% TW End Time');
% ylabel('Router ID');
% zlabel('Occupancy');
% title('Router VC Occupancy vs Time')
% colorbar;
% hold off
% 
% % %% Plot 3D Representation of Received Messages Data
% % figure
% % hold on
% % Y = 1:TOTAL_ROUTERS;
% % X = 0:N_COLLECT-1;
% % indx = 1;
% % Z = zeros(length(X),length(Y));
% % for j=3:2:TOTAL_ROUTERS*2+1
% %     Z(:,indx) = Data(:,j);
% %     indx = indx+1;
% % end
% % surf(X,Y,Z');
% % xlabel('% TW End Time');
% % ylabel('Router ID');
% % zlabel('Router Message Packets Received');
% % title('Instantaneous Number of Messages Received Per Router')
% % figure
% % ribbon(X,Z);
% % ylabel('% TW End Time');
% % xlabel('Router ID');
% % zlabel('Router Message Packets Received');
% % title('Instantaneous Number of Messages Received Per Router')
% % figure
% % h = waterfall(X,Y,Z');
% % colormap(jet)
% % set( h, 'LineWidth', 4 );
% % hidden off;
% % xlabel('% TW End Time');
% % ylabel('Router ID');
% % zlabel('Router Message Packets Received');
% % title('Instantaneous Number of Messages Received Per Router')
% % hold off
% % 
% % %% Plot Sent Messages Data
% % figure
% % hold on
% % for j=2:2:TOTAL_ROUTERS*2
% %     plot(Data(:,1),Data(:,j))
% %     xlabel('% TW End Time');
% %     ylabel('Router Message Packets Sent');
% %     title('Instantaneous Number of Messages Sent Per Router')
% % end
% % hold off
% % 
% % %% Plot Received Messages Data
% % figure
% % hold on
% % for j=3:2:TOTAL_ROUTERS*2
% %     plot(Data(:,1),Data(:,j))
% %     xlabel('% TW End Time');
% %     ylabel('Router Message Packets Received');
% %     title('Instantaneous Number of Messages Received Per Router')
% % end
% % hold off
% % 
% % %% Plot Total Received Messages at instance of time
% % clear run_sum;
% % figure
% % hold on
% % for i=1:N_COLLECT
% %     run_sum(i) = sum(Data(i,3:2:TOTAL_ROUTERS*2));
% % end
% % plot(Data(:,1),run_sum);
% % xlabel('% TW End Time');
% % ylabel('Router Message Packets Received')
% % title('Sum of Message packets Received by all Routers a instance of time');
% % 
% % %% Plot Running Total Received Messages of All Routers
% % figure
% % hold on
% % run_sum(1) = sum(Data(1,3:2:TOTAL_ROUTERS*2));
% % for i=2:N_COLLECT
% %     run_sum(i) = run_sum(i-1) + sum(Data(i,3:2:TOTAL_ROUTERS*2));
% % end
% % plot(Data(:,1),run_sum);
% % xlabel('% TW End Time');
% % ylabel('Router Message Packets Received')
% % title('Running Sum of Message packets Received by all Routers');
% % 
% % %% Plot Running Total Received Messages
% % clear run_sum;
% % figure
% % hold on
% % k=1;
% % for j=3:2:TOTAL_ROUTERS*2
% %     run_sum(1,k) = sum(Data(1,j));
% %     k=k+1;
% % end
% % for i=2:N_COLLECT
% %     k=1;
% %     for j=3:2:TOTAL_ROUTERS*2
% %         run_sum(i,k) = run_sum(i-1,k) + sum(Data(i,j));
% %         k=k+1;
% %     end
% % end
% % for j=1:TOTAL_ROUTERS-1
% %     plot(Data(:,1),run_sum(:,j));
% % end
% % xlabel('% TW End Time');
% % ylabel('Router Message Packets Received')
% % title('Running Sum of Message packets Received by all Routers');