%%
close all
clear all
clc

%% add function
addpath('~/Google Drive/Code-Data-Results/DMDmatlab/Plotfunc');

%% read data
T = csvread('~/Google Drive/Fall2016/APC524/HW4/unsteadyHeatEquation/Temperature128.dat');
[nx,~] = size(T);
Tmean = mean(mean(T));
x = linspace(0,pi,nx);
y = linspace(0,pi,nx);
[X, Y] = meshgrid(x,y);

%% visualize data, surface
figure
%contourf(X,Y,T,1000,'LineStyle','none')
surf(X,Y,T,'LineStyle','none')
%set(gca, 'XTick', []); set(gca, 'YTick', []);
xlabel('$x$','Interpreter','latex'),ylabel('$y$','Interpreter','latex')
zlabel('$T$','Interpreter','latex')
colormap(jet), colorbar
xlim([0,pi]), ylim([0,pi]), zlim([0,1])
%axis equal, box on
set(gca,'FontSize',20,'LineWidth',2)
set(gcf, 'Color', 'w');
%% visualize data, contourf
figure
contourf(X,Y,T,100,'LineStyle','none')
%surf(X,Y,T)
%set(gca, 'XTick', []); set(gca, 'YTick', []);
xlabel('$x$','Interpreter','latex'),ylabel('$y$','Interpreter','latex')
colormap(jet), colorbar
axis equal, box on
set(gca,'FontSize',20,'LineWidth',2)
set(gcf, 'Color', 'w');
%%
export_fig('Temperature128.pdf');

%% check boundary conditon
figure, plot(x,T(1,:),x,T(end,:),'LineWidth',2)
xlim([0,pi])
xlabel('x'), ylabel('T')
legend('T(x,y=0)','T(x,y=\pi)')
box on
set(gca,'FontSize',20,'LineWidth',2)
set(gcf, 'Color', 'w');