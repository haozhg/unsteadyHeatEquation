%%
close all
clear all
clc

%% read data
T = csvread('heat_omp_128.dat');
[nx,~] = size(T);
Tmean = mean(mean(T));
x = linspace(0,pi,nx);
y = linspace(0,pi,nx);
[X, Y] = meshgrid(x,y);

%% visualize data, 3D surface
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
saveas(gcf,'heat_serial_64.png');

%% visualize data, 2D contourf
figure
contourf(X,Y,T,100,'LineStyle','none')
%surf(X,Y,T)
%set(gca, 'XTick', []); set(gca, 'YTick', []);
xlabel('$x$','Interpreter','latex'),ylabel('$y$','Interpreter','latex')
colormap(jet), colorbar
axis equal, box on
set(gca,'FontSize',20,'LineWidth',2)
set(gcf, 'Color', 'w');
saveas(gcf,'heat_serial_64.png');
