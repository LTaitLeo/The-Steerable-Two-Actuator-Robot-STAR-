%% Vector-loop Equations

clear
clc

syms r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11 t3 t4 t5 t6 t7 t8 t9

% r1=1.51;r2=2.322;r3=2.11;r4=1.95;r5=0.94;r6=3;r7=2.11;r8=1.02;r9=2.30;r10=1.78;r11=0.86;% Solidworks model data
r1=39;r2=59;r3=54;r4=49;r5=24;r6=76;r7=54;r8=26;r9=58;r10=45;r11=22;% Solidworks model data in mm
% r1=7.12;r2=10.97;r3=9.97;r4=9.2;r5=4.41;r6=14.21;r7=10;r8=4.8;r9=10.84;r10=8.41;r11=4.04;% Geogebra model data 
% r1=1.47;r2=2.307;r3=2.104;r4=1.978;r5=0.923;r6=2.994;r7=2.126;r8=1.039;r9=2.293;r10=1.790;r11=0.838;% Actual model data

% Upper four-bar

f1=-r2-r3*sin(t3)-r4*sin(t4)-r5*sin(t5);
f2=r1-r3*cos(t3)-r4*cos(t4)-r5*cos(t5);

[t3,t4]=solve(f1,f2,t3,t4);

temp=t3;
t3=temp(1);
if(subs(t3(1),t5,0)/pi*180>-100)% choose between two solutions
    t3=temp(2);
end

temp=t4;
t4=temp(1);
if(subs(t4(1),t5,0)/pi*180<-50)
    t4=temp(2);
end

% Ternary link relations

f3=cos(t7+2*pi-t4)-(r4*r4+r7*r7-r10*r10)/(2*r4*r7);
f4=cos(t8-t5)-(r8*r8+r5*r5-r11*r11)/(2*r5*r8);

t7=solve(f3,t7);
t8=solve(f4,t8);

temp=t7;
t7=temp(1);
if(subs(t7(1),t5,0)/pi*180<0)% choose between two solutions
    t7=temp(2);
end

temp=t8;
t8=temp(1);
if(subs(t8(1),t5,0)/pi*180<0)
    t8=temp(2);
end

% Lower four-bar

f5=r6*sin(t6)-r7*sin(t7)-r8*sin(t8)-r9*sin(t9);
f6=r6*cos(t6)-r7*cos(t7)-r8*cos(t8)-r9*cos(t9);

[t6,t9]=solve(f5,f6,t6,t9);

temp=t6;
t6=temp(1);
if(subs(t6(1),t5,0)/pi*180>10)% choose between two solutions
    t6=temp(2);
end

temp=t9;
t9=temp(1);
if(subs(t9(1),t5,0)/pi*180>0)% choose between two solutions
    t9=temp(2);
end

% Angle solution
T3=[];
T4=[];
T7=[];
T8=[];
T6=[];
T9=[];

for t5_variable=0:0.1:2*pi
    % Substitute the value of input angle into the formula
    % save each value
    T3=[T3 subs(t3,t5,t5_variable)];
    T4=[T4 subs(t4,t5,t5_variable)];
    T7=[T7,subs(t7,t5,t5_variable)];
    T8=[T8,subs(t8,t5,t5_variable)];
    T6=[T6 subs(t6,t5,t5_variable)];
    T9=[T9 subs(t9,t5,t5_variable)];
end

t=linspace(0,2*pi,63);

% plot the figure of each angles with respect to theta 5
figure(1)
% plot(t*180/pi,T6*180/pi);% theta6 angle
plot(t*180/pi,T3*180/pi,t*180/pi,T4*180/pi,t*180/pi,T7*180/pi,t*180/pi,T8*180/pi,t*180/pi,T6*180/pi,t*180/pi,T9*180/pi);
title('angle-\theta_5')
xlabel('\theta_5(degree)')
ylabel('Angle(degree)')
legend('\theta_3','\theta_4','\theta_7','\theta_8','\theta_6','\theta_9')

% Position solution
X1=[];
X2=[];
Y2=[];
V2=[];

for t5_variable=0:0.01:2*pi

    x1=[-r5*cos(t5_variable),-r5*sin(t5_variable)];
    x2=[-r1+r3*cos(subs(t3,t5,t5_variable)),r2+r3*sin(subs(t3,t5,t5_variable))];
    X1=[X1;x1];
    X2=[X2;x2];
 
    y2=[x1(1)-r7*cos(subs(t7,t5,t5_variable)),x1(2)-r7*sin(subs(t7,t5,t5_variable))];
    Y2=[Y2;y2];
end

%  Trajectory position
figure(2)
plot(Y2(:,1),Y2(:,2));
title('trajectory')

%  Trajectory velocity
t=linspace(0,2*pi,628);

for i=1:1:628
    % using position difference instead of differentiation
    V2=[V2 abs((Y2(i+1,1)-Y2(i,1))^2+(Y2(i+1,2)-Y2(i,2))^2)];
end

figure(3)
plot(t*180/pi,V2);
title('trajectory velocity-\theta_5')
xlabel('\theta_5(degree)')
ylabel('velocity')
