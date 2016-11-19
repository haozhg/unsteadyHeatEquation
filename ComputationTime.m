%%
close all
clear all
clc

%%
nx = [128,256,512];
<<<<<<< HEAD
timeSerial = [0.7400,12.0500,194.0800];
timeOMPn1 = [0.8589,13.9019,219.7970];
timeOMPn2 = [0.4600, 6.9109,110.4084];
timeOMPn4 = [0.2403,3.5484,55.5524];
timeOMPn8 = [0.1735,2.0111,28.4757];
timeMPIn1 = [0.8200,12.7100,203.2700];
timeMPIn2 = [ 0.4400,6.4700,102.1600];
timeMPIn4 = [0.2400,3.3300,51.5500];
timeMPIn8 = [0.1700,1.7900,26.3100];
timeMPIn16 = [0.2000,1.2500,14.6600];

%%
figure
loglog(nx,timeSerial,'+-','DisplayName','Serial','LineWidth',2), hold on

loglog(nx,timeOMPn1,'x-','DisplayName','OMP,nthreads=1','LineWidth',2), hold on
loglog(nx,timeOMPn2,'*-','DisplayName','OMP,nthreads=2','LineWidth',2), hold on
loglog(nx,timeOMPn4,'o-','DisplayName','OMP,nthreads=4','LineWidth',2), hold on
loglog(nx,timeOMPn8,'s-','DisplayName','OMP,nthreads=8','LineWidth',2), hold on

loglog(nx,timeMPIn1,'>-','DisplayName','MPI,nthreads=1','LineWidth',2), hold on
loglog(nx,timeMPIn2,'<-','DisplayName','MPI,nthreads=2','LineWidth',2), hold on
loglog(nx,timeMPIn4,'^-','DisplayName','MPI,nthreads=4','LineWidth',2), hold on
loglog(nx,timeMPIn8,'v-','DisplayName','MPI,nthreads=8','LineWidth',2), hold on
loglog(nx,timeMPIn16,'d-','DisplayName','MPI,nthreads=16','LineWidth',2), hold on

xlabel('$nx$','Interpreter','latex'),ylabel('$time/s$','Interpreter','latex')
legend('show'), box on, grid('on')
xlim([100,600])
set(gca,'FontSize',16,'LineWidth',2)
=======
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
>>>>>>> e42861b141236da8b88833f527a2ee1c0c5a4535
set(gcf, 'Color', 'w');
%%
saveas(gcf,'computation_time.png');