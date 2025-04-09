files = dir("photos\*.jpg");

for i = 1:length(files)

clear count g G im max n;
im = imread(strcat("photos\", files(i).name));
g = rgb2hsv(im);
g = g(:,:,2);
g = g*255;
G = g(:);
figure(1);
set(gcf, 'Units', 'Normalized', 'OuterPosition', [0 0 1 1]); %wielkość okna
subplot(1,3,[1,2]); 
histogram(G,'FaceColor', '#ffffff');

[count, n] = histcounts( G, 255 );
max = max(count);
max = max*1.2;

xlim([0 255]);
ylim([0,max]);

grid on;
title('Histogram nasycenia pikseli','FontSize', 30);
xlabel('Wartość nasycenia','FontSize',20);
ylabel('Ilośc pikseli','FontSize',20);

subplot(1,3,3);
imshow(im);

exportgraphics(gcf, strcat("saturation\", files(i).name(1:length(files(i).name)-4),"_saturation.jpg"))
close;

end
