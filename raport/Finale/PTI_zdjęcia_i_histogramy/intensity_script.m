files = dir("photos\*.jpg");

for i = 1:length(files)

clear count g G im k light max n s x y;
im = imread(strcat("photos\", files(i).name));
g = rgb2gray(im);
G = g(:);
s = length(G);
figure(1);
set(gcf, 'Units', 'Normalized', 'OuterPosition', [0 0 1 1]); %wielkość okna

%usun komentarze z 7, 33 i 34 linijki zeby po prawej stronie 
%histogramu było zdjęcie
subplot(1,3,[1,2]); 
histogram(G,'FaceColor', '#ffffff');

[count, n] = histcounts( G, 255 );
max = max(count);
max = max*1.2;

xlim([0 255]);
ylim([0,max]);

grid on;
title('Histogram jasności pikseli','FontSize', 30);
xlabel('Wartość jasności','FontSize',20);
ylabel('Ilośc pikseli','FontSize',20);

x = [0 0 0 0];
y = [0 0 max max];
light = 0.66;
for k =1:1:5
    hold on;
    x = x + [0 51 51 0];
    patch(x,y,'k','FaceAlpha',light);
    x = x + [51 0 0 51];
    light = light *0.66;
end

subplot(1,3,3);
imshow(im);

exportgraphics(gcf, strcat("intensity\", files(i).name(1:length(files(i).name)-4),"_intensity.jpg"))
close;

end