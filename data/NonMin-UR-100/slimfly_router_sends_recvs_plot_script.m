%close all;
%% Input Log Files
%TOTAL_ROUTERS = 50;
TOTAL_ROUTERS = 338;
N_COLLECT = 100;
filename = sprintf('slimfly_router_sends_recvs_log.txt');
slimfly_results_log = dlmread(filename,',',0,0);
Data = slimfly_results_log;

%% Plot 3D Representation of Sent Messages Data
figure
hold on
Y = 1:TOTAL_ROUTERS;
X = 0:N_COLLECT-1;
indx = 1;
Z = zeros(length(X),length(Y));
for j=2:2:TOTAL_ROUTERS*2
    Z(:,indx) = Data(:,j);
    indx = indx+1;
end
% surf(X,Y,Z');
% xlabel('% TW End Time');
% ylabel('Router ID');
% zlabel('Router Message Packets Sent');
% title('Instantaneous Number of Messages Sent Per Router')
% figure
% ribbon(X,Z);
% ylabel('% TW End Time');
% xlabel('Router ID');
% zlabel('Router Message Packets Sent');
% title('Instantaneous Number of Messages Sent Per Router')
% figure
subplot(2,2,3);
h = waterfall(X,Y,Z');
colormap(jet);
set( h, 'LineWidth', 4 );
hidden off;
xlabel('% TW End Time');
ylabel('Router ID');
zlabel('Messages Sent');
title('Messages Sent Per Router')
colorbar;
caxis([min(min(Z)), max(max(Z))])
axis([0 N_COLLECT 0 TOTAL_ROUTERS min(min(Z)) max(max(Z))])
hold off

%% Plot 3D Representation of Received Messages Data
%figure
hold on
Y = 1:TOTAL_ROUTERS;
X = 0:N_COLLECT-1;
indx = 1;
Z = zeros(length(X),length(Y));
for j=3:2:TOTAL_ROUTERS*2+1
    Z(:,indx) = Data(:,j);
    indx = indx+1;
end
% surf(X,Y,Z');
% xlabel('% TW End Time');
% ylabel('Router ID');
% zlabel('Router Message Packets Received');
% title('Instantaneous Number of Messages Received Per Router')
% figure
% ribbon(X,Z);
% ylabel('% TW End Time');
% xlabel('Router ID');
% zlabel('Router Message Packets Received');
% title('Instantaneous Number of Messages Received Per Router')
% figure
subplot(2,2,4)
h = waterfall(X,Y,Z');
colormap(jet)
set( h, 'LineWidth', 4 );
hidden off;
xlabel('% TW End Time');
ylabel('Router ID');
zlabel('Messages Received');
title('Messages Received Per Router')
colorbar;
caxis([min(min(Z)), max(max(Z))])
axis([0 N_COLLECT 0 TOTAL_ROUTERS min(min(Z)) max(max(Z))])
hold off

% %% Plot Sent Messages Data
% figure
% hold on
% for j=2:2:TOTAL_ROUTERS*2
%     plot(Data(:,1),Data(:,j))
%     xlabel('% TW End Time');
%     ylabel('Router Message Packets Sent');
%     title('Instantaneous Number of Messages Sent Per Router')
% end
% hold off
% 
% %% Plot Received Messages Data
% figure
% hold on
% for j=3:2:TOTAL_ROUTERS*2
%     plot(Data(:,1),Data(:,j))
%     xlabel('% TW End Time');
%     ylabel('Router Message Packets Received');
%     title('Instantaneous Number of Messages Received Per Router')
% end
% hold off
% 
% %% Plot Total Received Messages at instance of time
% clear run_sum;
% figure
% hold on
% for i=1:N_COLLECT
%     run_sum(i) = sum(Data(i,3:2:TOTAL_ROUTERS*2));
% end
% plot(Data(:,1),run_sum);
% xlabel('% TW End Time');
% ylabel('Router Message Packets Received')
% title('Sum of Message packets Received by all Routers a instance of time');
% 
% %% Plot Total Sent Messages at instance of time
% clear run_sum;
% figure
% hold on
% for i=1:N_COLLECT
%     run_sum(i) = sum(Data(i,2:2:TOTAL_ROUTERS*2));
% end
% plot(Data(:,1),run_sum);
% xlabel('% TW End Time');
% ylabel('Router Message Packets Sent')
% title('Sum of Message packets Sent by all Routers a instance of time');
% 
% %% Plot Running Total Received Messages of All Routers
% figure
% hold on
% run_sum(1) = sum(Data(1,3:2:TOTAL_ROUTERS*2));
% for i=2:N_COLLECT
%     run_sum(i) = run_sum(i-1) + sum(Data(i,3:2:TOTAL_ROUTERS*2));
% end
% plot(Data(:,1),run_sum);
% xlabel('% TW End Time');
% ylabel('Router Message Packets Received')
% title('Running Sum of Message packets Received by all Routers');
% 
% %% Plot Running Total Received Messages
% clear run_sum;
% figure
% hold on
% k=1;
% for j=3:2:TOTAL_ROUTERS*2+1
%     run_sum(1,k) = (Data(1,j));
%     k=k+1;
% end
% for i=2:N_COLLECT
%     k=1;
%     for j=3:2:TOTAL_ROUTERS*2+1
%         run_sum(i,k) = run_sum(i-1,k) + (Data(i,j));
%         k=k+1;
%     end
% end
% for j=1:TOTAL_ROUTERS-1
%     plot(Data(:,1),run_sum(:,j));
% end
% xlabel('% TW End Time');
% ylabel('Router Message Packets Received')
% title('Running Sum of Message packets Received by all Routers');
% 
% figure
% hold on
% Y = 1:TOTAL_ROUTERS;
% X = 0:N_COLLECT-1;
% % indx = 1;
% % Z = zeros(length(X),length(Y));
% % for j=3:2:TOTAL_ROUTERS*2+1
% %     Z(:,indx) = run_sum(:,j);
% %     indx = indx+1;
% % end
% h = waterfall(X,Y,run_sum');
% colormap(jet);
% set( h, 'LineWidth', 4 );
% hidden off;
% xlabel('% TW End Time');
% ylabel('Router ID');
% zlabel('Router Message Packets Received');
% title('3D Running sum of packets received per router')
% 
% %% Plot Running Total Sent Messages
% clear run_sum;
% figure
% hold on
% k=1;
% for j=2:2:TOTAL_ROUTERS*2
%     run_sum(1,k) = (Data(1,j));
%     k=k+1;
% end
% for i=2:N_COLLECT
%     k=1;
%     for j=2:2:TOTAL_ROUTERS*2
%         run_sum(i,k) = run_sum(i-1,k) + (Data(i,j));
%         k=k+1;
%     end
% end
% for j=1:TOTAL_ROUTERS-1
%     plot(Data(:,1),run_sum(:,j));
% end
% xlabel('% TW End Time');
% ylabel('Router Message Packets Sent')
% title('Running Sum of Message packets Sent by all Routers');
% 
% 
% figure
% hold on
% Y = 1:TOTAL_ROUTERS;
% X = 0:N_COLLECT-1;
% % indx = 1;
% % Z = zeros(length(X),length(Y));
% % for j=3:2:TOTAL_ROUTERS*2+1
% %     Z(:,indx) = run_sum(:,j);
% %     indx = indx+1;
% % end
% h = waterfall(X,Y,run_sum');
% colormap(jet);
% set( h, 'LineWidth', 4 );
% hidden off;
% xlabel('% TW End Time');
% ylabel('Router ID');
% zlabel('Router Message Packets Received');
% title('3D Running sum of packets sent per router')
% 
% % clear all