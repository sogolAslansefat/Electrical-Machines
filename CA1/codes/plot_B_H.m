clc
clear
close all

B_H_iron;
B_H_steel;

plot(H_iron,B_iron,'r.-');
hold on
plot(H_steel,B_steel,'b.-');
legend("Iron","Steel");
xlabel('H');
ylabel('B');
xlim([0 5000])
ylim([0 2.2])

