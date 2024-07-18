fileName = 'background_nuevo.nmr';
str = fileread(fileName);
background = jsondecode(str);
Fs = 20000;            % Sampling frequency                    
T = 1/Fs;             % Sampling period       
L = 131072;             % Length of signal
t = (0:L-1)*T;        % Time vector
bandpass(background,[1900 2500],Fs);
Y = fft(background);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
f = Fs*(0:(L/2))/L;
 
title("Single-Sided Amplitude Spectrum of X(t)")
xlabel("f (Hz)")
ylabel("|P1(f)|")
figure;
fileName = 'measurement_nuevo.nmr';
str = fileread(fileName);
 measurement= jsondecode(str);
bandpass(measurement,[1900 2500],Fs);
YY = fft(measurement);
PP2 = abs(YY/L);
PP1 = PP2(1:L/2+1);
PP1(2:end-1) = 2*PP1(2:end-1);
figure
ff = Fs*(0:(L/2))/L;

plot(f,P1)
hold on;
plot(ff,PP1) 