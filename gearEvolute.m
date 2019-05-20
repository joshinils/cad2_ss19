clear all;
close all;
format compact;
format shortg;

%%
zz = 36; % zahnanzahl
m = 3; % modul
anz_linienstuecke = 500;

%%
alpha_0 = 20/180*pi; % eingriffswinkel
r0 = (m*zz)/2;
rb = r0 * cos(alpha_0);
rf = r0 - m;
ra = r0 + m;
delta = pi/(2*zz) + tan(alpha_0) - alpha_0;
phi = 2*pi/zz;

%%
axis equal;
hold on;
plot([0;0],[0,0], 'xk');
hold on;
circle(r0, '-.b');
circle(rb, '-.k');
circle(ra, '-b');
circle(rf, '-m');

th = (0:2*pi/zz:2*pi)';
r = ra*1.1;
xDiv = r * cos(th);
yDiv = r * sin(th);
hold on;
for i=1:length(th)
    plot([0;xDiv(i)], [0,yDiv(i)], ':m');
end


%%
r = linspace(rf, ra, anz_linienstuecke+1)';
u = 1/rb .* sqrt(r.^2 - rb.^2);


evolvente(:,1) = rb .* (cos(u-delta) + u.*sin(u-delta));
evolvente(:,2) = rb .* (sin(u-delta) - u.*cos(u-delta));

evolventeSpiegel = evolvente;
evolventeSpiegel(:,2) = -evolventeSpiegel(:,2);

%plotMat(evolventeSpiegel);
plotMat(evolvente);
drehmatrix = [cos(phi), -sin(phi);
              sin(phi),  cos(phi) ];

          return;

for i = 1:zz
    evolvente = (drehmatrix * evolvente')';
    plotMat(evolvente);
    
    evolventeSpiegel = (drehmatrix * evolventeSpiegel')';
    plotMat(evolventeSpiegel);
end




















