files = dir("photos\*.jpg");
cont = struct('plik', cell(1,length(files)), 'kontrast', cell(1,length(files)), 'jasnosc', cell(1,length(files)));

for i = 1:length(files)

clear g G l im j k m n R;
im = imread(strcat("photos\", files(i).name)); 
g = rgb2gray(im);
g = single(g);
g=g/255;
G = g(:);
I = mean(G);
[n, m] = size(g);
R =0.0;

for k=1:1:n
    for j = 1:1:m
        R = R + (g(k,j) - I)^2;
    end
end

R = R/(m*n);

R = sqrt(R);

%disp(["Kontrast zdjęcia" R]);
%disp(["Średnia jasność zdjęcia" I]);

cont(i).plik = files(i).name;
cont(i).kontrast = R;
cont(i).jasnosc = I;


end

writetable(struct2table(cont), 'contrast.csv')