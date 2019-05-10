clear all;
close all;
format compact;
format shortg;

%%
zz = 36; % zahnanzahl
m = 3; % modul
anz_linienstuecke = 5;

%%
alpha_0 = 20/180*pi; % eingriffswinkel
r_0 = (m*zz)/2;
r_b = r_0 * cos(alpha_0);
r_f = r_0 - m;
r_a = r_0 + m;
delta = pi/(2*zz) + tan(alpha_0) - alpha_0;
phi = 2*pi/zz;

%%
axis equal;
hold on;
plot([0;0],[0,0], 'xk');
hold on;
circle(r_0, '-.b');
circle(r_b, '-.k');
circle(r_a, '-b');
circle(r_f, '-k');

th = (0:2*pi/zz:2*pi)';
r = r_a*1.1;
xDiv = r * cos(th);
yDiv = r * sin(th);
hold on;
for i=1:length(th)
    plot([0;xDiv(i)], [0,yDiv(i)], ':m');
end


%%
r = linspace(r_b, r_a, anz_linienstuecke+1)';
u = 1/r_b .* sqrt(r.^2 - r_b.^2);


evolvente(:,1) = r_b .* (cos(u-delta) + u.*sin(u-delta));
evolvente(:,2) = r_b .* (sin(u-delta) - u.*cos(u-delta));

evolventeSpiegel = evolvente;
evolventeSpiegel(:,2) = -evolventeSpiegel(:,2);

%plotMat(evolventeSpiegel);
%plotMat(evolvente);

drehmatrix = [cos(phi), -sin(phi);
              sin(phi), cos(phi) ];


for i = 1:zz
    evolvente = (drehmatrix * evolvente')';
    plotMat(evolvente);

    evolventeSpiegel = (drehmatrix * evolventeSpiegel')';
    plotMat(evolventeSpiegel);
end




















