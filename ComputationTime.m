%%
close all
clear all
clc

%%
nx = [128,256,512];
timeSerial = [];
timeOMPn1 = [];
timeOMPn2 = [];
timeOMPn4 = [];
timeOMPn8 = [];
timeMPIn1 = [];
timeMPIn2 = [];
timeMPIn4 = [];
timeMPIn8 = [];
timeMPIn16 = [];

%%
figure
semilogy(nx,timeSerial,'Serial','LineWidth',2), hold on

semilogy(nx,timeOMPn1,'OMP,nthreads=1','LineWidth',2), hold on
semilogy(nx,timeOMPn2,'OMP,nthreads=2','LineWidth',2), hold on
semilogy(nx,timeOMPn4,'OMP,nthreads=4','LineWidth',2), hold on
semilogy(nx,timeOMPn8,'OMP,nthreads=8','LineWidth',2), hold on

semilogy(nx,timeMPIn1,'MPI,nthreads=1','LineWidth',2), hold on
semilogy(nx,timeMPIn2,'MPI,nthreads=2','LineWidth',2), hold on
semilogy(nx,timeMPIn4,'MPI,nthreads=4','LineWidth',2), hold on
semilogy(nx,timeMPIn8,'MPI,nthreads=8','LineWidth',2), hold on
semilogy(nx,timeMPIn16,'MPI,nthreads=16','LineWidth',2), hold on

xlabel('$nx$','Interpreter','latex'),ylabel('$time/s$','Interpreter','latex')
legend('show','Interpreter','latex');
%axis equal, box on
set(gca,'FontSize',20,'LineWidth',2)
set(gcf, 'Color', 'w');
%%
saveas(gcf,'computation_time.png');