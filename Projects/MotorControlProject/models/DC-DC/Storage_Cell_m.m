
Fpwm = 5000;                %Гц
Timesample = 1/(4*Fpwm);    %с

%Номинальное значние Udc%
Udc     = 450;
Udc_min = 380;

%Допустимые пульсации Udc%
dUdc = 0.1; %в долях от 1 - номинального значения Udc

%Номинальное и максимальное значения Us накопителя%
Us     = 320;
Us_max = 380;
Us_min = 100;

Us_discharge = 0.95 * Us_max;

%% Converter parameters
%Inductance
L   = 3e-3;%0.5e-3;
R_L = 1;%0.01

%DC capacitor 
C_dc = 3500e-6;
R_dc = 70;%22.5;

%Store capacitor
C_s = 0.66;
R_s = 10000;

%% Regulators

Tpwm = 1/Fpwm;

%Current PI-regulator for VT1 control (charging U_store)
k_i_1 = L / (2 * 1 * Tpwm);%L / (2 * Udc * Tpwm)
T_i_1 = L / R_L;

%Voltage I-regulator for VT1 control (charging U_store)
%T_u_1 = 32 * (Tpwm * Tpwm) / C_s;

k_u_1 = 1000;
T_u_1 = 2*T_i_1;

%Current PI-regulator for VT2 control (charging U_dc)
k_i_2 = L / (2 * 1 * Tpwm);%(2 * Us * Tpwm);
T_i_2 = L / R_L;

%Voltage I-regulator for VT1 control (charging U_dc)
k_u_2 = 0.15;
T_u_2 = 2*T_i_2;

%% FFT
FFT_F_sample = Fpwm; %Гц
FFT_T_sample = 1/FFT_F_sample;

FFT_T = 1 / 100;

FFT_N = FFT_T / FFT_T_sample;
