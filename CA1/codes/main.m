clc
clear
close all

N=70;
I=10;
l=0.25;
A=12.5*10^-4;

error = 0.01;
E = error + 1;

start_B = 0;
end_B = 2;

while E > error
    B = (start_B + end_B)/2;
    H_iron = B2H_iron(B);
    H_steel = B2H_steel(B);
    equation_error = N*I - (H_iron + H_steel)*l;
    if equation_error < 0
        end_B = B;
    else
        start_B = B;
    end
    E = abs(equation_error); % equation_error < error
    %E = end_B - start_B;    % B_error < error
end

B = (start_B + end_B)/2
H_iron = B2H_iron(B)
H_steel = B2H_steel(B)


